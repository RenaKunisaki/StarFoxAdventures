#!/usr/bin/env python3
import os
import os.path
import sys
import inspect
import struct
import re
import hashlib
from PIL import Image
from binaryfile import BinaryFile
from tabfile import TabFile
from zlb import Zlb
from dplzo import DPLZO
from sfatexture import SfaTexture, ImageFormat

def printf(fmt, *args, **kw):
    print(fmt % args, end='', flush=True, **kw)

class App:
    # XXX don't hardcode these
    MAX_TEX0_ID = 0x35E
    MAX_TEX1_ID = 0x900

    def __init__(self):
        pass

    def _getMethods(self):
        return [
            (func, getattr(self, func))
            for func in dir(self)
            if callable(getattr(self, func))
            and not func.startswith('_')
        ]


    def help(self):
        """Display detailed help text."""
        methods = self._getMethods()
        for name, method in sorted(methods, key = lambda it: it[0]):
            doc = getattr(method, '__doc__', "(no documentation)")
            doc = [line for line in doc.split('\n') if line.strip() != '']
            printf("%s: %s\n\n", name, '\n'.join(doc))


    def usage(self):
        """Display summary help text."""
        methods = self._getMethods()
        names = []
        for name, method in sorted(methods, key = lambda it: it[0]):
            names.append(name)
        print("Usage: %s method" % sys.argv[0])
        print("  method:", ', '.join(names))


    def compress(self, inPath, outPath, inOffset=0):
        """Compress a ZLB file."""
        if type(inOffset) is str: inOffset = int(inOffset, 0)
        file = BinaryFile(inPath, 'rb', offset=inOffset)
        with open(outPath, 'wb') as outFile:
            outFile.write(Zlb(file).compress())


    def decompress(self, inPath, outPath, inOffset=0):
        """Decompress a ZLB file."""
        if type(inOffset) is str: inOffset = int(inOffset, 0)
        file = BinaryFile(inPath, 'rb', offset=inOffset)
        with open(outPath, 'wb') as outFile:
            outFile.write(Zlb(file).decompress())


    def compressLZO(self, inPath, outPath, inOffset=0):
        """Compress a LZO file."""
        if type(inOffset) is str: inOffset = int(inOffset, 0)
        file = BinaryFile(inPath, 'rb', offset=inOffset)
        with open(outPath, 'wb') as outFile:
            outFile.write(DPLZO(file).compress())


    def decompressLZO(self, inPath, outPath, inOffset=0):
        """Decompress a LZO file."""
        if type(inOffset) is str: inOffset = int(inOffset, 0)
        file = BinaryFile(inPath, 'rb', offset=inOffset)
        with open(outPath, 'wb') as outFile:
            outFile.write(DPLZO(file).decompress())


    def listTable(self, inPath):
        """List entries in a TAB file."""
        file = BinaryFile(inPath, 'rb')
        tbl  = TabFile(file)
        print("Item C Flg Offset  (C=compressed?)")
        for i, entry in enumerate(tbl.getEntries()):
            printf("%5d: %s  %02X %06X\n", i,
                'Y' if entry['compressed'] else '-',
                entry['flags'], entry['offset'])

    def dumpTexture(self, path, outPath, raw=False, offset=0):
        """Dump texture from file."""
        if type(raw) is str: raw = (raw == '1')
        if type(offset) is str: offset = int(offset, 0)
        file = BinaryFile(path, 'rb', offset=offset)
        head = file.readStruct('3b')
        file.seek(0)
        if head in (b'ZLB', b'DIR', b'\xFA\xCE\xFE'):
            data = Zlb(file).decompress()
        else:
            size = file.readu32(offset=0x44)
            file.seek(0)
            data = file.read(size+0x60)
        if raw:
            with open(outPath, 'wb') as outFile:
                outFile.write(data)
        else:
            tex = SfaTexture.fromData(data)
            printf("Texture size: %dx%d, fmt %s, %d frames\n",
                tex.width, tex.height, tex.format.name,
                tex.nFrames)
            tex.image.save(outPath)


    def dumpTextures(self, path, outPath, raw=False):
        """Dump textures from files at `path` to dir `outPath`.

        path: eg "warlock/TEX0" (no extension)
        """
        if type(raw) is str: raw = (raw == '1')
        file = BinaryFile(path+'.bin', 'rb')
        tbl  = BinaryFile(path+'.tab', 'rb') # don't use TabFile
            # because it's not quite the same format here
        idx   = -1
        csvLines = [
            ['idx', 'frame', 'fmt', 'flags']
        ]
        while True:
            idx += 1
            entry = tbl.readu32()
            if entry == 0xFFFFFFFF: break
            #if entry == 0x01000000: continue # skip
            flags =  entry >> 30
            count = (entry >> 24) & 0x3F
            offs  = (entry & 0xFFFFFF) * 2
            if flags == 0: continue # XXX meaning of these?
            #printf("%04X %06X %X %02X\n", idx, offs, flags, count)

            try:
                file.seek(offs)
                if count > 1:
                    offsets = file.readu32(count=count)
                else: offsets = (0,)
                #printf("%04X %02X %02X %06X %s\n", idx, flags, count, offs,
                #    ' '.join(map(lambda n: '%08X'%n, offsets)))

                for i, offset in enumerate(offsets):
                    file.seek(offs + offset)
                    data = Zlb(file).decompress()
                    tex  = SfaTexture.fromData(data)

                    if raw:
                        name = "%s/%04X.%02X.tex" % (outPath, idx, i)
                        with open(name, 'wb') as outFile:
                            outFile.write(data)
                    else:
                        #printf("%04X.%02X: %3dx%3d %2dfrm %s\n", idx, i,
                        #    tex.width, tex.height, tex.nFrames, tex.format.name)
                        name = "%s/%04X.%02X.png" % (outPath, idx, i)
                        tex.image.save(name)

                    csvLines.append([
                        '%04X' % idx, i, tex.format.name, flags])
            except Exception as ex:
                printf("%04X ERROR: %s\n", idx, ex)
        # while
        name = path.split('/')[-1]
        with open(f'{outPath}/{name}.csv', 'wt') as file:
            for line in csvLines:
                file.write(';'.join(map(str, line)) + '\n')


    def packTexture(self, path, outPath, format, nFrames):
        """Pack image at `path` to texture file."""
        nFrames = int(nFrames)
        assert nFrames > 0, "nFrames must be at least 1"
        format = ImageFormat[format]
        img = Image.open(path)
        tex = SfaTexture.fromImage(img, fmt=format, nFrames=nFrames)
        with open(outPath, 'wb') as file:
            tex.writeToFile(file)


    def packMultiTexture(self, format, *paths):
        """Pack images at `paths` to texture file.

        Last path is output file. Others are frames in order.
        """
        assert len(paths) > 1, "Usage: packMultiTexture format inPath [inPath...] outPath"
        format = ImageFormat[format]
        images = []
        paths  = list(paths) # make mutable
        outPath = paths.pop()
        for path in paths:
            images.append(Image.open(path))
        tex = SfaTexture.fromImageSet(images, fmt=format)
        with open(outPath, 'wb') as file:
            tex.writeToFile(file)


    def packTextures(self, path, outPath, which):
        """Pack images in `path` to TEXn.bin, TEXn.tab files, where n=which."""
        textures = {} # ID => tex
        # get list of files to pack
        # XXX change to read CSV file.
        for name in os.listdir(path):
            if re.match(r'^[0-9a-fA-F]+\.[0-9a-fA-F]+\.', name):
                fields = name.split('.')
                tid = int(fields[0], 16) # texture ID
                mid = int(fields[1], 16) # frame ID
                if tid not in textures: textures[tid] = {}
                textures[tid][mid] = name

        printf("Packing %d textures to %s.bin/tab\n", len(textures), outPath)

        # write out bin and tab files
        binFile = BinaryFile(outPath+'.bin', 'wb')
        tabFile = BinaryFile(outPath+'.tab', 'wb')
        maxId = self.MAX_TEX0_ID if str(which) == '0' else self.MAX_TEX1_ID
        for tid in range(maxId):
            if tid in textures:
                printf("%04X... ", tid)
                frames = textures[tid]
                offs   = binFile.tell()
                nFrames = len(frames)
                printf("%2d frames, %08X  ", nFrames, offs)
                tabFile.writeu32(
                    0x80000000 | (offs>>1) | (nFrames << 24))

                frameData = []
                for iFrame in range(nFrames):
                    name = frames[iFrame]
                    fPath = os.path.join(path, name)
                    if name.endswith('.tex') or name.endswith('.bin'):
                        with open(fPath, 'rb') as file:
                            data = file.read()
                    else: # image file
                        fields = name.split('.')
                        fmt = ImageFormat[fields[2]]
                        img = Image.open(fPath)
                        tex = SfaTexture.fromImage(img, fmt=format, nFrames=nFrames)
                        data = tex.toData()
                    data = Zlb(None).compress(data)
                    pad = len(data) & 0x3
                    if pad: data += b'\0' * (4 - pad)
                    frameData.append(data)

                # write the frame offsets
                if nFrames > 1:
                    frameOffs = 4 * (nFrames+1)
                    for data in frameData:
                        binFile.writeu32(frameOffs)
                        frameOffs += len(data)
                    binFile.writeu32(frameOffs)

                # write the data
                for data in frameData:
                    binFile.write(data)

                # align to 32 bytes - required by game
                pad = binFile.tell() & 0x1F
                if pad: binFile.write(b'\0' * (32 - pad))

                printf("OK\n")
            else:
                tabFile.writeu32(0x01000000)
        # write size of last item and terminator
        tabFile.writeu32(binFile.tell() >> 1)
        tabFile.writeu32(0xFFFFFFFF)
        tabFile.writeu32(0xCFA2) # XXX what is this? never read?
        tabFile.writeu32(0, 0, 0, 0, 0, 0, 0)
        binFile.close()
        tabFile.close()


    def _dumpRaw(self, binPath, tabPath, outPath, ignoreFlags=False,
    offsMask=0x0FFFFFFF, offsShift=0, nameFunc=None):
        """Dump raw data listed in table, where the next entry is used
        to calculate the size.
        """
        try: file = BinaryFile(binPath+'.bin', 'rb')
        except FileNotFoundError: file = BinaryFile(binPath+'.BIN', 'rb')
        try: tbl  = BinaryFile(tabPath+'.tab', 'rb')
        except FileNotFoundError: tbl  = BinaryFile(tabPath+'.TAB', 'rb')

        if nameFunc is None: nameFunc = lambda idx, data: '%04X.bin' % idx

        idx   = 0
        entry = tbl.readu32()
        while True:
            if entry == 0xFFFFFFFF: break
            eNext = tbl.readu32()
            flags = entry >> 24
            if flags != 0 or ignoreFlags:
                offs = entry & offsMask
                if offsShift >= 0: offs = offs >> offsShift
                else: offs = offs << -offsShift
                try:
                    file.seek(offs)
                    head = file.read(3)
                    file.seek(offs)
                    if head in (b'ZLB', b'DIR', b'\xFA\xCE\xFE'):
                        data = Zlb(file).decompress()
                    else:
                        oNext = eNext & offsMask
                        if offsShift >= 0: oNext = oNext >> offsShift
                        else: oNext = oNext << -offsShift
                        data = file.read(oNext - offs)

                    name = '%s/%s' % (outPath, nameFunc(idx,data))
                    #printf("%04X %02X %06X %06X %s\n", idx, flags, offs, len(data), name)
                    with open(name, 'wb') as outFile:
                        outFile.write(data)
                except Exception as ex:
                    printf("%04X %s ERROR: %s\n", idx, outPath, ex)
            idx += 1
            entry = eNext


    def dumpModels(self, path, outPath):
        """Dump models from files at `path` to dir `outPath`.

        path: eg "warlock/MODELS" (no extension)
        """
        # XXX eventually decode them too
        self._dumpRaw(path, path, outPath)

    def dumpAnimations(self, path, outPath):
        """Dump animations from files at `path` to dir `outPath`.

        path: eg "warlock/ANIM" (no extension)
        """
        self._dumpRaw(path, path, outPath)

    def dumpAnimCurv(self, path, outPath):
        """Dump animation curves from files at `path` to dir `outPath`.

        path: eg "warlock/ANIMCURV" (no extension)
        """
        self._dumpRaw(path, path, outPath)

    def dumpVoxMap(self, path, outPath):
        """Dump Voxmap data from files at `path` to dir `outPath`.

        path: eg "warlock/VOXMAP" (no extension)
        """
        self._dumpRaw(path, path, outPath)

    def dumpMod(self, path, outPath):
        """Dump modXX data from files at `path` to dir `outPath`.

        path: eg "warlock/mod16" (no extension)
        """
        self._dumpRaw(path+'.zlb', path, outPath,
            nameFunc=lambda idx, data: struct.unpack_from('12s', data, 0xA4)[0].strip(b'\0').decode('utf-8')+'.bin')


    def dumpMap(self, path, outPath):
        """Dump all of a map's files at `path` to dir `outPath`.

        path: eg "warlock"
        """
        J = os.path.join
        outPath = os.path.realpath(outPath)
        dirs = ('animations', 'animcurv', 'mod', 'models', 'TEX0', 'TEX1', 'voxmap')
        for d in dirs:
            os.makedirs(J(outPath, d), exist_ok=True)
        self.dumpAnimations(J(path, 'ANIM'),     J(outPath, 'animations'))
        self.dumpAnimCurv  (J(path, 'ANIMCURV'), J(outPath, 'animcurv'))
        self.dumpModels    (J(path, 'MODELS'),   J(outPath, 'models'))
        self.dumpTextures  (J(path, 'TEX0'),     J(outPath, 'TEX0'))
        self.dumpTextures  (J(path, 'TEX1'),     J(outPath, 'TEX1'))
        self.dumpVoxMap    (J(path, 'VOXMAP'),   J(outPath, 'voxmap'))
        for name in os.listdir(path):
            if re.match(r'^mod[0-9a-fA-F]+\.zlb', name):
                self.dumpMod(J(path, name.split('.')[0]), J(outPath, 'mod'))


    def dumpAllMaps(self, path, outPath):
        """Dump all files of every map from `path` to `outpath`.

        path: disc root.
        """
        errors = {} # map name => Exception
        def isMap(name):
            return os.path.isfile(os.path.join(path, name, 'MODELS.bin'))
        maps = list(filter(isMap, os.listdir(path)))
        for i, name in enumerate(maps):
            try:
                printf("Extracting %3d/%3d: %s... ", i+1, len(maps), name)
                self.dumpMap(os.path.join(path, name),
                    os.path.join(outPath, name))
                print("OK")
            except Exception as ex:
                errors[name] = ex
                print("Failed")
        for name, ex in errors.items():
            print("Failed extracting:", name, ex)


    def _analyzeMapDump(self, path, base, _files={}, _depth=0):
        assert _depth < 5, "Maximum depth exceeded"
        for name in os.listdir(path):
            fullPath = os.path.join(path, name)
            if os.path.isdir(fullPath):
                self._analyzeMapDump(fullPath, base, _files, _depth+1)
            elif os.path.isfile(fullPath):
                #print("Checking", fullPath)
                if name.endswith('.csv'):
                    pass # XXX
                    # need to examine texture CSVs and check if
                    # any were stored with a different format
                    # or have a different number or frames.
                else:
                    hash = hashlib.md5()
                    with open(fullPath, 'rb') as file:
                        hash.update(file.read())
                    hash = hash.hexdigest()
                    fName = fullPath[len(base):]
                    fName = os.path.join(*(fName.split(os.path.sep)[1:]))
                    if fName not in _files: _files[fName] = {}
                    _files[fName][hash] = fullPath
            else:
                print("Skipping non-file, non-directory object:", fullPath)
        return _files


    def analyzeMapDump(self, path):
        """Examine all files dumped from maps in `path`.

        Tests whether any maps have different versions.
        """
        files = self._analyzeMapDump(path, path)
        names = list(sorted(files.keys()))
        for name in names:
            hashes = files[name]
            if len(hashes) > 1:
                # XXX identify which one isn't the same as most.
                # also, this won't catch if a file isn't present
                # in some maps.
                print(name)
                for hash, path in hashes.items():
                    print('', hash, path)


    def listAnimations(self, discRoot, modelId):
        """List the animation IDs used by given model."""
        modelId = int(modelId, 0)
        modAnimTab = BinaryFile(discRoot+'/MODANIM.TAB', 'rb')
        modAnimOffs = modAnimTab.readu16(modelId << 1)
        nextOffs = modAnimTab.readu16((modelId+1) << 1)
        nAnims = (nextOffs - modAnimOffs) >> 1
        modAnimBin = BinaryFile(discRoot+'/MODANIM.BIN', 'rb')
        animIds = modAnimBin.readBytes(nAnims*2, modAnimOffs)
        animIds = struct.unpack('>%dh' % nAnims, animIds)
        printf("%4d animations; MODANIM.BIN 0x%06X - 0x%06X, min 0x%04X max 0x%04X\n",
            nAnims, modAnimOffs, modAnimOffs+(nAnims*2),
            max(0, min(animIds)), max(animIds))
        for i in range(0, nAnims, 8):
            printf('%04X: %s\n', i,
            ' '.join(map(lambda v: '%04X' % (v&0xFFFF), animIds[i:i+8])))


    def readRomList(self, path, discRoot):
        """Read romlist.zlb file."""
        file = BinaryFile(path, 'rb', offset=0)
        data = Zlb(file).decompress()

        # read OBJINDEX.bin
        objIndex = []
        with open(discRoot+'/OBJINDEX.bin', 'rb') as objIdxFile:
            entries = objIdxFile.read()
            for i in range(0, len(entries), 2):
                it = struct.unpack_from('>H', entries, i)[0] # grumble
                objIndex.append(it)

        # read OBJECTS.tab
        objsTab = []
        with open(discRoot+'/OBJECTS.tab', 'rb') as objTabFile:
            entries = objTabFile.read()
            for i in range(0, len(entries), 4):
                it = struct.unpack_from('>I', entries, i)[0] # grumble
                objsTab.append(it)

        # read OBJECTS.bin to get names
        objNames = []
        with open(discRoot+'/OBJECTS.bin', 'rb') as objBinFile:
            for offs in objsTab:
                objBinFile.seek(offs + 0x91)
                name = objBinFile.read(11).decode('utf-8').replace('\0', '')
                objNames.append(name)

        offs, idx = 0, 0
        printf("Idx  Offs Type Obj  ObjName     Sz MapStates------X Fl 06 07      X        Y        Z   UniqueID SeqData\n")
        while offs < len(data):
            typ, length, states3, flags, states5, b6, b7, x, y, z, uniqueId = \
                struct.unpack_from('>hBBBBBBfffI', data, offs)
            if typ >= 0:
                if typ < len(objIndex): realTyp = objIndex[typ]
                else: realTyp = "????"
            else: realTyp = -typ
            if type(realTyp) != str:
                try: name = objNames[realTyp]
                except: name = "?"
                realTyp = '%04X' % realTyp
            else: name = "?"

            states = []
            for i in range(8):
                states.append('-' if (states3 >> i) & 1 else ('%X' % (i+1)))
            for i in range(8):
                states.append('-' if (states5 >> (7-i)) else ('%X' % ((i+9) & 0xF)))
            states = ''.join(states)

            # idx offs typ realTyp name sz states flags 06 07 X Y Z ID
            printf("%04X %04X %04X %s %-11s %02X %s %02X %02X %02X %+8.2f %+8.2f %+8.2f %08X ",
                idx, offs, typ, realTyp, name, length, states, flags, b6, b7, x, y, z,
                uniqueId)
            for i in range(0x18, length*4, 4): # SeqData
                printf("%02X%02X%02X%02X ", *data[offs+i: offs+i+4])
            printf("\n")

            if length == 0: break
            offs += length * 4
            idx  += 1

    def toLZO(self, path, tabPath, outBinPath, outTabPath):
        """Convert a file's contents to LZO.

        path: input file to convert.
        tabPath: table for input file.
        outBinPath: output path for .bin file.
        outTabPath: output path for .tab file.
        """
        inFile  = BinaryFile(path,    'rb')
        tabFile = BinaryFile(tabPath, 'rb')
        outBin  = BinaryFile(outBinPath, 'wb')
        outTab  = BinaryFile(outTabPath, 'wb')
        inZlb   = Zlb(inFile)
        outLzo  = DPLZO(outBin)
        while True:
            offs    = tabFile.readu32()
            if offs == 0xFFFFFFFF: break
            high = offs & 0xFF000000
            inFile.seek(offs & 0xFFFFFF)
            data = inZlb.decompress()
            pad  = len(data) & 0xF
            if pad: data += b'\0' * (16-pad)
            outTab.writeu32(outBin.tell() | high)
            outBin.write(outLzo.compress(data))
        outTab.writeu32(outBin.tell()) # XXX verify
        outTab.writeu32(0xFFFFFFFF)
        # XXX should move the tab code to a common method...

    def toDIR(self, path, tabPath, outBinPath, outTabPath):
        """Convert a file's contents to uncompressed DIR format.

        path: input file to convert.
        tabPath: table for input file.
        outBinPath: output path for .bin file.
        outTabPath: output path for .tab file.
        """
        inFile  = BinaryFile(path,    'rb')
        tabFile = BinaryFile(tabPath, 'rb')
        outBin  = BinaryFile(outBinPath, 'wb')
        outTab  = BinaryFile(outTabPath, 'wb')
        inZlb   = Zlb(inFile)
        offs    = tabFile.readu32()
        while True:
            nextOffs = tabFile.readu32()
            if nextOffs == 0xFFFFFFFF: break
            high = offs & 0xFF000000
            offs &= 0xFFFFFF
            size = (nextOffs & 0xFFFFFF) - offs
            if size > 0:
                inFile.seek(offs)
                data = inZlb.decompress()
                header = b'DIR\0' + struct.pack('>3I', 0, len(data), len(data))
                pad  = len(data) & 0xF
                if pad: data += b'\0' * (16-pad)
            outTab.writeu32(outBin.tell() | high)
            if size > 0: outBin.write(header + data)
            offs = nextOffs
        outTab.writeu32(outBin.tell()) # XXX verify
        outTab.writeu32(0xFFFFFFFF)

    def toFakeLZO(self, path, tabPath, outBinPath, outTabPath):
        """Convert a file's contents to "fake LZO" format.

        path: input file to convert.
        tabPath: table for input file.
        outBinPath: output path for .bin file.
        outTabPath: output path for .tab file.
        """
        inFile  = BinaryFile(path,    'rb')
        tabFile = BinaryFile(tabPath, 'rb')
        outBin  = BinaryFile(outBinPath, 'wb')
        outTab  = BinaryFile(outTabPath, 'wb')
        inZlb   = Zlb(inFile)
        offs    = tabFile.readu32()
        while True:
            nextOffs = tabFile.readu32()
            if nextOffs == 0xFFFFFFFF: break
            high = offs & 0xFF000000
            offs &= 0xFFFFFF
            size = (nextOffs & 0xFFFFFF) - offs
            if size > 0:
                inFile.seek(offs)
                data = inZlb.decompress()
                header = b'LZO\0' + struct.pack('>3I', 1, len(data), len(data)) + b'Rena'
                pad  = len(data) & 0xF
                if pad: data += b'\0' * (16-pad)
            outTab.writeu32(outBin.tell() | high)
            if size > 0: outBin.write(header + data)
            offs = nextOffs
        outTab.writeu32(outBin.tell()) # XXX verify
        outTab.writeu32(0xFFFFFFFF)

    def textoLZO(self, path, tabPath, outBinPath, outTabPath):
        """Convert a texture file's contents to LZO.

        path: input file to convert.
        tabPath: table for input file.
        outBinPath: output path for .bin file.
        outTabPath: output path for .tab file.
        """
        inFile  = BinaryFile(path,    'rb')
        tabFile = BinaryFile(tabPath, 'rb')
        outBin  = BinaryFile(outBinPath, 'wb')
        outTab  = BinaryFile(outTabPath, 'wb')
        inZlb   = Zlb(inFile)
        outLzo  = DPLZO(outBin)
        tabCksum = 0
        while True:
            offs    = tabFile.readu32()
            if offs == 0xFFFFFFFF: break
            high = offs & 0xFF000000
            if high & 0x80000000:
                offs = (offs & 0xFFFFFF) * 2
                nFrames = (high >> 24) & 0x3F
                inFile.seek(offs)

                # read frame values
                if nFrames > 1: frameData = inFile.readu32(nFrames+1)
                else: frameData = []

                data = outLzo.compress(inZlb.decompress())
                tabVal = (outBin.tell() >> 1) | (high & 0xC0000000) | ((nFrames-1) << 24)
                if nFrames > 1: outBin.writeu32(*frameData)
                outBin.write(data)
                while outBin.tell() & 0xF: outBin.writeu8(0)

            else: # don't use this texture
                tabVal = 0x01000000
            tabCksum += ((tabVal >> 24) & 0xFF)
            tabCksum += ((tabVal >> 16) & 0xFF)
            tabCksum += ((tabVal >>  8) & 0xFF)
            tabCksum += ( tabVal        & 0xFF)
            outTab.writeu32(tabVal)
        outTab.writeu32(0xFFFFFFFF)
        tabCksum += (0xFF * 4)
        outTab.writeu32(tabCksum & 0xFFFFFFFF)
        for i in range(7):
            outTab.writeu32(0)

    def textoDIR(self, path, tabPath, outBinPath, outTabPath):
        """Convert a texture file's contents to uncompressed DIR format.

        path: input file to convert.
        tabPath: table for input file.
        outBinPath: output path for .bin file.
        outTabPath: output path for .tab file.
        """
        inFile  = BinaryFile(path,       'rb')
        tabFile = BinaryFile(tabPath,    'rb')
        outBin  = BinaryFile(outBinPath, 'wb')
        outTab  = BinaryFile(outTabPath, 'wb')
        inZlb   = Zlb(inFile)
        tabCksum = 0
        while True:
            offs    = tabFile.readu32()
            if offs == 0xFFFFFFFF: break
            high = offs & 0xFF000000
            if high & 0x80000000:
                offs = (offs & 0xFFFFFF) * 2
                nFrames = (high >> 24) & 0x3F
                inFile.seek(offs)

                # read frame values
                if nFrames > 1: frameData = inFile.readu32(nFrames+1)
                else: frameData = []

                data = inZlb.decompress()
                #data = b'\0\0\0\0' + b'\0\0\0\0' + b'\0\0\0\0' + data
                dl = len(data)
                header = b'DIR\0' + struct.pack('>3I', 0, dl, dl)
                header += b'\0\0\0\0' + b'\0\0\0\0' + b'\0\0\0\0' + b'\0\0\0\0'
                data = header + data
                tabVal = (outBin.tell() >> 1) | high
                if nFrames > 1: outBin.writeu32(*frameData)
                outBin.write(data)
                while outBin.tell() & 0xF: outBin.writeu8(0)

            else: # don't use this texture
                tabVal = 0x01000000
            tabCksum += ((tabVal >> 24) & 0xFF)
            tabCksum += ((tabVal >> 16) & 0xFF)
            tabCksum += ((tabVal >>  8) & 0xFF)
            tabCksum += ( tabVal        & 0xFF)
            outTab.writeu32(tabVal)
        outTab.writeu32(0xFFFFFFFF)
        tabCksum += (0xFF * 4)
        outTab.writeu32(tabCksum & 0xFFFFFFFF)
        for i in range(7):
            outTab.writeu32(0)

    def modelstoDIR(self, path, tabPath, outBinPath, outTabPath):
        """Convert a model file's contents to uncompressed DIR format.

        path: input file to convert.
        tabPath: table for input file.
        outBinPath: output path for .bin file.
        outTabPath: output path for .tab file.
        """
        inFile  = BinaryFile(path,       'rb')
        tabFile = BinaryFile(tabPath,    'rb')
        outBin  = BinaryFile(outBinPath, 'wb')
        outTab  = BinaryFile(outTabPath, 'wb')
        inZlb   = Zlb(inFile)
        tabCksum = 0
        while True:
            offs    = tabFile.readu32()
            if offs == 0xFFFFFFFF: break
            high = offs & 0xFF000000
            if high & 0x10000000:
                #printf("Model %08X\r\n", offs)
                offs = offs & 0xFFFFFF
                inFile.seek(offs)

                data = inFile.read(4)
                if data == b'\xFA\xCE\xFE\xED':
                    while data not in (b'ZLB\0', b'DIR\0', b'\xE0\xE0\xE0\xE0', b'\xF0\xF0\xF0\xF0'):
                        outBin.write(data)
                        data = inFile.read(4)
                inFile.seek(offs)

                data = inZlb.decompress()
                header = b'DIR\0' + struct.pack('>3I', 0, len(data), len(data))
                data = header + data
                tabVal = outBin.tell() | high
                outBin.write(data)
                while outBin.tell() & 0xF: outBin.writeu8(0)

            else: # don't use this model
                tabVal = 0x00000000
            tabCksum += ((tabVal >> 24) & 0xFF)
            tabCksum += ((tabVal >> 16) & 0xFF)
            tabCksum += ((tabVal >>  8) & 0xFF)
            tabCksum += ( tabVal        & 0xFF)
            outTab.writeu32(tabVal)

        tabVal = outBin.tell()
        outTab.writeu32(tabVal)
        tabCksum += ((tabVal >> 24) & 0xFF)
        tabCksum += ((tabVal >> 16) & 0xFF)
        tabCksum += ((tabVal >>  8) & 0xFF)
        tabCksum += ( tabVal        & 0xFF)

        outTab.writeu32(0xFFFFFFFF)
        tabCksum += (0xFF * 4)
        outTab.writeu32(tabCksum & 0xFFFFFFFF)
        for i in range(7):
            outTab.writeu32(0)


    def modelstoFakeLZO(self, path, tabPath, outBinPath, outTabPath):
        """Convert a model file's contents to "fake LZO" format.

        path: input file to convert.
        tabPath: table for input file.
        outBinPath: output path for .bin file.
        outTabPath: output path for .tab file.
        """
        inFile  = BinaryFile(path,       'rb')
        tabFile = BinaryFile(tabPath,    'rb')
        outBin  = BinaryFile(outBinPath, 'wb')
        outTab  = BinaryFile(outTabPath, 'wb')
        inZlb   = Zlb(inFile)
        tabCksum = 0
        while True:
            offs    = tabFile.readu32()
            if offs == 0xFFFFFFFF: break
            high = offs & 0xFF000000
            if high & 0x10000000:
                #printf("Model %08X\r\n", offs)
                offs = offs & 0xFFFFFF
                inFile.seek(offs)

                data = inFile.read(4)
                if data == b'\xFA\xCE\xFE\xED':
                    while data not in (b'ZLB\0', b'DIR\0', b'\xE0\xE0\xE0\xE0', b'\xF0\xF0\xF0\xF0'):
                        outBin.write(data)
                        data = inFile.read(4)
                inFile.seek(offs)

                data = inZlb.decompress()
                header = b'ZLB\0' + struct.pack('>3I', 1, len(data), len(data)) + b'Rena'
                data = header + data
                tabVal = outBin.tell() | high
                outBin.write(data)
                while outBin.tell() & 0xF: outBin.writeu8(0)

            else: # don't use this model
                tabVal = 0x00000000
            tabCksum += ((tabVal >> 24) & 0xFF)
            tabCksum += ((tabVal >> 16) & 0xFF)
            tabCksum += ((tabVal >>  8) & 0xFF)
            tabCksum += ( tabVal        & 0xFF)
            outTab.writeu32(tabVal)

        tabVal = outBin.tell()
        outTab.writeu32(tabVal)
        tabCksum += ((tabVal >> 24) & 0xFF)
        tabCksum += ((tabVal >> 16) & 0xFF)
        tabCksum += ((tabVal >>  8) & 0xFF)
        tabCksum += ( tabVal        & 0xFF)

        outTab.writeu32(0xFFFFFFFF)
        tabCksum += (0xFF * 4)
        outTab.writeu32(tabCksum & 0xFFFFFFFF)
        for i in range(7):
            outTab.writeu32(0)

    def mapstoFakeLZO(self, path, tabPath, outBinPath, outTabPath):
        """Convert MAPS.BIN file's contents to "fake LZO" format.

        path: input file to convert.
        tabPath: table for input file.
        outBinPath: output path for .bin file.
        outTabPath: output path for .tab file.
        """
        inFile  = BinaryFile(path,       'rb')
        tabFile = BinaryFile(tabPath,    'rb')
        outBin  = BinaryFile(outBinPath, 'wb')
        outTab  = BinaryFile(outTabPath, 'wb')
        inZlb   = Zlb(inFile)
        offs    = tabFile.readu32()
        while offs != 0xFFFFFFFF:
            nextOffs = tabFile.readu32()
            inLen    = nextOffs - offs
            inFile.seek(offs)

            tabVal = outBin.tell()
            outTab.writeu32(tabVal)

            while offs < nextOffs:
                inFile.seek(offs)
                data = inFile.read(4)
                if len(data) == 0: break
                elif data == b'\xFA\xCE\xFE\xED':
                    rawLen, zlbOffs, compLen = inFile.readStruct('3I')
                    dOut = data + struct.pack('>3I', rawLen+0x28, zlbOffs, rawLen+4)
                    printf("Write FACEFEED at %08X: %s\r\n", outBin.tell(),
                        dOut.hex())
                    outBin.write(dOut)
                    for i in range(zlbOffs):
                        outBin.writeu16(inFile.readu16())
                    offs += len(data)
                elif data == b'ZLB\0':
                    inFile.seek(offs)
                    data = inZlb.decompress()
                    dl   = len(data) + 4
                    outBin.write(b'LZO\0' + struct.pack('3I', 0, 0, dl) + b'Rena' + data)
                else:
                    outBin.write(data)
                offs = inFile.tell()
            offs = nextOffs

        tabVal = outBin.tell()
        outTab.writeu32(tabVal)
        outTab.writeu32(0xFFFFFFFF)
        for i in range(3):
            outTab.writeu32(0)


def main(*args):
    app = App()

    if len(args) < 1:
        app.usage()
        return 0

    while len(args) > 0:
        method = args[0]
        if method.startswith('--'): method = method[2:]

        func = getattr(app, method, None)
        if func is None or not callable(func):
            print("Unknown operation:",method)
            return 1

        sig   = inspect.signature(func)
        nArg  = len(sig.parameters)
        isVararg = any(map(
            lambda p: p.kind == inspect.Parameter.VAR_POSITIONAL, sig.parameters.values()))
        if isVararg:
            fArgs = args[1:]
            args  = []
        else:
            fArgs = args[1:nArg+1]
            args  = args[nArg+1:]

        if len(fArgs) < nArg:
            msg = [method]
            for param in sig.parameters.values():
                if param.kind == inspect.Parameter.VAR_POSITIONAL:
                    msg.append(param.name + '...')
                else: msg.append(param.name)
            print("Usage:", ' '.join(msg))
            return 1

        func(*fArgs)
    return 0


if __name__ == '__main__':
    sys.exit(main(*sys.argv[1:]))
