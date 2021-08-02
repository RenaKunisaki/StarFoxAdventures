#!/usr/bin/env python3
"""Read bootstrap.bin and src.elf and produce boot.bin."""
import os
import sys
import struct
from enum import IntEnum, IntFlag


def printf(fmt, *args, **kwargs):
    print(fmt % args, end='', **kwargs)


def readStruct(fmt, file, offset=None):
    """Read struct from file."""
    if offset is not None: file.seek(offset)
    sLen = struct.calcsize(fmt)
    r = struct.unpack(fmt, file.read(sLen))
    if len(r) == 1: return r[0] # grumble
    return r


class ELF:
    """Represents an ELF file. (Only 32-bit big-endian is supported.)"""

    class ProgramHeaderTypes(IntEnum):
        PT_NULL    = 0x00000000
        PT_LOAD    = 0x00000001
        PT_DYNAMIC = 0x00000002
        PT_INTERP  = 0x00000003
        PT_NOTE    = 0x00000004
        PT_SHLIB   = 0x00000005
        PT_PHDR    = 0x00000006
        PT_TLS     = 0x00000007
        PT_LOOS    = 0x60000000
        PT_HIOS    = 0x6FFFFFFF
        PT_LOPROC  = 0x70000000
        PT_HIPROC  = 0x7FFFFFFF
    class SectionHeaderTypes(IntEnum):
        SHT_NULL          = 0x00000000
        SHT_PROGBITS      = 0x00000001
        SHT_SYMTAB        = 0x00000002
        SHT_STRTAB        = 0x00000003
        SHT_RELA          = 0x00000004
        SHT_HASH          = 0x00000005
        SHT_DYNAMIC       = 0x00000006
        SHT_NOTE          = 0x00000007
        SHT_NOBITS        = 0x00000008
        SHT_REL           = 0x00000009
        SHT_SHLIB         = 0x0000000A
        SHT_DYNSYM        = 0x0000000B
        SHT_INIT_ARRAY    = 0x0000000E
        SHT_FINI_ARRAY    = 0x0000000F
        SHT_PREINIT_ARRAY = 0x00000010
        SHT_GROUP         = 0x00000011
        SHT_SYMTAB_SHNDX  = 0x00000012
        SHT_NUM           = 0x00000013
        SHT_LOOS          = 0x60000000
    class SectionHeaderFlags(IntFlag):
        SHF_WRITE            = 0x00000001
        SHF_ALLOC            = 0x00000002
        SHF_EXECINSTR        = 0x00000004
        SHF_MERGE            = 0x00000010
        SHF_STRINGS          = 0x00000020
        SHF_INFO_LINK        = 0x00000040
        SHF_LINK_ORDER       = 0x00000080
        SHF_OS_NONCONFORMING = 0x00000100
        SHF_GROUP            = 0x00000200
        SHF_TLS              = 0x00000400
        SHF_MASKOS           = 0x0FF00000
        SHF_MASKPROC         = 0xF0000000
    class PPCRelType(IntEnum): # Relocation types
        R_PPC_NONE            =  0
        R_PPC_ADDR32          =  1 # 32bit absolute address
        R_PPC_ADDR24          =  2 # 26bit address, 2 bits ignored.
        R_PPC_ADDR16          =  3 # 16bit absolute address
        R_PPC_ADDR16_LO       =  4 # lower 16bit of absolute address
        R_PPC_ADDR16_HI       =  5 # high 16bit of absolute address
        R_PPC_ADDR16_HA       =  6 # adjusted high 16bit
        R_PPC_ADDR14          =  7 # 16bit address, 2 bits ignored
        R_PPC_ADDR14_BRTAKEN  =  8
        R_PPC_ADDR14_BRNTAKEN =  9
        R_PPC_REL24           = 10 # PC relative 26 bit
        R_PPC_REL14           = 11 # PC relative 16 bit
        R_PPC_REL14_BRTAKEN   = 12
        R_PPC_REL14_BRNTAKEN  = 13
        R_PPC_GOT16           = 14
        R_PPC_GOT16_LO        = 15
        R_PPC_GOT16_HI        = 16
        R_PPC_GOT16_HA        = 17
        R_PPC_PLTREL24        = 18
        R_PPC_COPY            = 19
        R_PPC_GLOB_DAT        = 20
        R_PPC_JMP_SLOT        = 21
        R_PPC_RELATIVE        = 22
        R_PPC_LOCAL24PC       = 23
        R_PPC_UADDR32         = 24
        R_PPC_UADDR16         = 25
        R_PPC_REL32           = 26
        R_PPC_PLT32           = 27
        R_PPC_PLTREL32        = 28
        R_PPC_PLT16_LO        = 29
        R_PPC_PLT16_HI        = 30
        R_PPC_PLT16_HA        = 31
        R_PPC_SDAREL16        = 32
        R_PPC_SECTOFF         = 33
        R_PPC_SECTOFF_LO      = 34
        R_PPC_SECTOFF_HI      = 35
        R_PPC_SECTOFF_HA      = 36

    def __init__(self, file):
        self.file = file
        self.progHdrs = []
        self.sectHdrs = []
        self.strTabs  = []
        self.readHeader()
        self.strOffs = None # offset into file for string table

        # read program headers
        for i in range(self.phNum):
            offs = self.phOff + (i * self.phEntSize)
            phdr = self.readProgramHeader(offs)
            self.progHdrs.append(phdr)

        # read section headers
        for i in range(self.shNum):
            offs = self.shOff + (i * self.shEntSize)
            shdr = self.readSectionHeader(offs)
            self.sectHdrs.append(shdr)
            if shdr['type'] == self.SectionHeaderTypes.SHT_STRTAB:
                self.strTabs.append(shdr['offset'])


    def getSectionData(self, shdr):
        """Get the raw data of a section header."""
        self.file.seek(shdr['offset'])
        return self.file.read(shdr['size'])


    def readHeader(self):
        """Read ELF header from file and make sure it's correct and supported."""
        sig = self.file.read(4)
        if sig != b'\x7F' + b'ELF': raise TypeError("Not an ELF file")
        cls, endian, version, abi, abiVer = readStruct('>5b', self.file)

        ok = (cls == 1) and (endian == 2) and (version == 1)
        if not ok:
            raise TypeError("Must be 32-bit big-endian ELFv1 (have %s-bit %s-endian ELFv%d)" % (
                '32' if cls == 1 else ('64' if cls == 2 else ('?%d?' % cls)),
                'little' if endian == 1 else ('big' if endian == 2 else ('?%d?' % endian)),
                version))
        machine = readStruct('>H', self.file, 0x12)
        if machine != 0x14: raise TypeError("Must be PowerPC32 ISA (got 0x%04X)" % machine)

        self.entryPoint, self.phOff, self.shOff, self.flags, self.ehSize, \
        self.phEntSize, self.phNum, self.shEntSize, self.shNum, self.shStrndx = \
            readStruct('>IIIIHHHHHH', self.file, 0x18)


    def printHeader(self):
        """Output the header for debugging."""
        printf("  Entry Point:    0x%08X\n", self.entryPoint)
        printf("  ProgHdr Offset: 0x%08X Size: 0x%08X Count: %d\n",
            self.phOff, self.phEntSize, self.phNum)
        printf("  SectHdr Offset: 0x%08X Size: 0x%08X Count: %d\n",
            self.shOff, self.shEntSize, self.shNum)
        printf("  Flags:          0x%08X\n", self.flags)
        printf("  Header Size:    0x%08X\n", self.ehSize) # file header
        printf("  String Index:   0x%08X\n", self.shStrndx)


    def readProgramHeader(self, offset):
        """Read program header from offset."""
        pType, offs, vAddr, pAddr, fSize, mSize, flags, align = \
            readStruct('>IIIIIIII', self.file, offset)
        return {
            'headerOffset': offset,
            'type':   pType,
            'offset': offs,  # segment offset in file
            'vAddr':  vAddr, # virtual address
            'pAddr':  pAddr, # physical address
            'fSize':  fSize, # size in file
            'mSize':  mSize, # size in memory
            'flags':  flags,
            'align':  align,
        }

    def printProgramHeader(self, phdr):
        """Output program header for debugging."""
        try: typ = self.ProgramHeaderTypes[phdr['type']]
        except KeyError: typ = '0x%08X' % phdr['type']
        printf("  Type: %s\n", typ)
        printf("  File Offs: 0x%08X Size: 0x%08X\n", phdr['offset'], phdr['fSize'])
        printf("  Virt Addr: 0x%08X Size: 0x%08X\n", phdr['vAddr'],  phdr['mSize'])
        printf("  Phys Addr: 0x%08X Algn: 0x%08X\n", phdr['pAddr'],  phdr['align'])
        printf("  Flags: 0x%08X\n", phdr['flags'])


    def readSectionHeader(self, offset):
        """Read section header from offset."""
        shName, shType, flags, addr, offs, size, link, info, align, entSize = \
            readStruct('>IIIIIIIIII', self.file, offset)
        return {
            'headerOffset': offset,
            'nameOffs': shName,
            'type':     shType,
            'flags':    flags,
            'addr':     addr,
            'offset':   offs,
            'size':     size,
            'link':     link,
            'info':     info,
            'align':    align,
            'entSize':  entSize,
        }

    def printSectionHeader(self, shdr):
        """Output section header for debugging."""
        try: typ = self.SectionHeaderTypes(shdr['type']).name
        except KeyError: typ = '0x%08X' % shdr['type']
        flags = []
        for k, v in self.SectionHeaderFlags.__members__.items():
            if shdr['flags'] & v: flags.append(k)
        flags = ', '.join(flags)
        name = self.getString(shdr['nameOffs'], 1)
        if name is None: name = '<none>'

        printf("  Type:     %s\n", typ)
        printf("  Name @    0x%08X: %s\n", shdr['nameOffs'], name)
        printf("  Flags:    0x%08X %s\n", shdr['flags'], flags)
        printf("  MemAddr:  0x%08X\n", shdr['addr'])
        printf("  FileOffs: 0x%08X Size:  0x%08X\n", shdr['offset'], shdr['size'])
        printf("  Link:     0x%08X Info:  0x%08X\n", shdr['link'], shdr['info'])
        printf("  Align:    0x%08X EntSz: 0x%08X\n", shdr['align'], shdr['entSize'])


    def getString(self, offs, tab=0):
        """Read string from string table."""
        if offs == 0: return None
        if tab >= len(self.strTabs): return '<no string table>'

        self.file.seek(self.strTabs[tab] + offs)
        res = b''
        while len(res) < 1000:
            b = self.file.read(1)
            if len(b) < 1 or b == b'\0': break
            res += b
        return res.decode('utf-8')


    def getSymbolTable(self):
        """Retrieve the symbol table data."""
        data = None
        for shdr in self.sectHdrs:
            if shdr['type'] == self.SectionHeaderTypes.SHT_SYMTAB:
                data = self.getSectionData(shdr)
                break
        if data is None: return None

        # we have the raw data, now parse it
        tbl = []
        for i in range(0, len(data), 16):
            nameOffs, val, size, info, other, index = \
                struct.unpack_from('>IIIBBh', data, i)
            tbl.append({
                'nameOffs': nameOffs,
                'name':     self.getString(nameOffs, 0),
                'value':    val,
                'size':     size,
                'info':     info,
                'other':    other,
                'index':    index,
            })
        return tbl

    def printSymbolTable(self):
        """Output symbol table for debugging."""
        symTab = self.getSymbolTable()
        if symTab is None: print("No symbol table")
        else:
            print("Symbol Table:\n  Idx Value    Size     If Ot Index  Name")
            for i, sym in enumerate(symTab):
                name = sym['name']
                if name is None: name = '<none>'
                printf("%5d %08X %08X %02X %02X %6d %s\n", i,
                    sym['value'], sym['size'], sym['info'], sym['other'],
                    sym['index'], name)


    def getRelocationTable(self):
        """Retrieve the relocation table data."""
        data = None
        for shdr in self.sectHdrs:
            if shdr['type'] == self.SectionHeaderTypes.SHT_RELA:
                data = self.getSectionData(shdr)
                break
        if data is None: return None

        # we have the raw data, now parse it
        tbl = []
        for i in range(0, len(data), 12):
            offs, unk, symTabIdx, relType, add = \
                struct.unpack_from('>IHBBi', data, i)
            tbl.append({
                'offset':    offs,
                'unk':       unk,
                'symTabIdx': symTabIdx,
                'relType':   relType,
                'add':       add,
            })
        return tbl

    def printRelocationTable(self):
        """Output relocation table for debugging."""
        relTab = self.getRelocationTable()
        if relTab is None: print("No relocation table")
        else:
            symTab = self.getSymbolTable()
            print("Relocation data:\n  Idx Offset   Unk  Ix Tp Sym")
            for i, entry in enumerate(relTab):
                sym = symTab[entry['symTabIdx']]
                name = sym['name']
                if name is None: name = '<none>'
                printf("%5d %08X %04X %02X %02X %16s %+d\n", i,
                    entry['offset'], entry['unk'], entry['symTabIdx'],
                    entry['relType'], name, entry['add'])


def main():
    args = list(sys.argv[1:])
    if len(args) < 2:
        print("Usage: elf2bin.py buildDir outPath")
        sys.exit(1)

    inPath  = args.pop(0)
    outPath = args.pop(0)
    outBin  = open(os.path.join(outPath, 'boot.bin'),     'wb')
    inBin   = open(os.path.join(inPath,  'bootstrap.bin'),'rb')
    inElf   = open(os.path.join(inPath,  'src.elf'),      'rb')
    elf     = ELF(inElf)

    # copy bootstrap
    while True:
        data = inBin.read(1024)
        outBin.write(data)
        if len(data) < 1024: break
    inBin.close()
    while outBin.tell() & 0xF: outBin.write(b'\0') # align
    outOffs = outBin.tell()

    # print headers
    #elf.printHeader()
    #for i, phdr in enumerate(elf.progHdrs):
    #    printf("Program Header #%d @ 0x%08X:\n", i, phdr['headerOffset'])
    #    elf.printProgramHeader(phdr)
    #for i, shdr in enumerate(elf.sectHdrs):
    #    printf("Section Header #%d @ 0x%08X:\n", i, shdr['headerOffset'])
    #    elf.printSectionHeader(shdr)
    #elf.printSymbolTable()
    #elf.printRelocationTable()

    # get global offset table
    gotOffs, gotSize = 0, 0
    for shdr in elf.sectHdrs:
        name = elf.getString(shdr['nameOffs'], 1)
        if name == '.got':
            # 0x54 is ELF header size. XXX replace with the file offset of the section.
            gotOffs = (shdr['offset'] - 0x54)
            gotSize = shdr['size'] >> 2
            break

    # write name for debug
    #name, ext = os.path.splitext(os.path.basename(inPath))
    #name = bytes(name[0:16], 'utf-8').ljust(16, b'\0')
    #outBin.write(name)

    # write tab entry
    #outTab.write(struct.pack('>I', outBin.tell()))

    # get entry point
    # for some reason ld can't find the _start symbol even though it exists,
    # so we need to look it up in the symbol table.
    entryPoint = 0
    symTab = elf.getSymbolTable()
    for sym in symTab:
        if sym['name'] == '_start':
            entryPoint = sym['value']
            break
    printf('[*] Entry point: 0x%08X\n', entryPoint)

    # write header (GOT offset, GOT size/4, entry point, bootstrap size)
    outBin.seek(4)
    outBin.write(struct.pack('>IIII',
        gotOffs+outOffs-8, # -4 for lwzu, -4 for ???
        gotSize, entryPoint+outOffs, outOffs))
    outBin.seek(outOffs)
    printf("[*] GOT Offs=0x%08X, Size=0x%08X, bootstrap=0x%08X\n",
        gotOffs+outOffs, gotSize, outOffs)

    # write code
    for phdr in elf.progHdrs:
        elf.file.seek(phdr['offset'])
        outBin.write(elf.file.read(phdr['fSize']))

        # write padding for empty areas.
        # since the patches stay in memory all the time anyway, this method
        # greatly simplifies the code at the expense of some disc space.
        outBin.write(b'\0' * (phdr['mSize'] - phdr['fSize']))

        # align
        while outBin.tell() & 0xF: outBin.write(b'\0')

    # write end of tab
    #outTab.write(struct.pack('>I', 0xFFFFFFFF))
    #while outTab.tell() < 32: outTab.write(b'\0') # enforce minimum size

    outBin.close()
    #outTab.close()


if __name__ == '__main__':
    main()
