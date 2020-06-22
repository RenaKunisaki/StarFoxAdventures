import struct
from binaryfile import BinaryFile
from isofile import IsoFile

class FstBin(IsoFile):
    """fst.bin file; contains ISO directory structure."""

    def __init__(self, offset:int=0, size:int=0, file:BinaryFile=None,
    files=None):
        super().__init__("fst.bin", isDir=False, offset=offset,
            size=size, file=file, fileOffs=offset)
        self._entries  = [] # internal list
        self.files     = [] # IsoFile
        self._strTable = []
        self.isSystem  = True

        if file is not None:
            self.readFile(file, offset)
        elif files is not None:
            self.build()


    def readFile(self, file:(str,BinaryFile)="fst.bin", offset:int=0):
        """Read fst.bin file."""
        #print("Read FST from %s 0x%X" % (path, offset))

        if type(file) is str:
            file = BinaryFile(file, 'rb', offset=offset)
        else: file.seek(offset)
        root = self._readEntry(file)
        if not root['isDir']:
            raise NotADirectoryError("Root directory is not a directory")

        self._entries.append(root)
        nFiles = root['nextIdx']
        root['path'] = '/'
        strTabOffs = nFiles * 0x0C

        # read entries
        for i in range(nFiles - 1):
            entry = self._readEntry(file)
            self._entries.append(entry)

        # read names
        self._strTable = []
        for i, entry in enumerate(self._entries):
            file.seek(strTabOffs + entry['nameOffs'] + offset)
            name = file.readString()
            self._strTable.append(bytes(name, 'utf-8') + b'\0')
            entry['name'] = name


        # read directory structure
        self._readFstDir(0, '')

        # convert to IsoFile
        for entry in self._entries:
            parent = entry.get('parent', None)
            if parent is not None: parent = self.files[parent['idx']]
            if entry['isDir']:
                f = IsoFile(entry['path'], isDir=True, parent=parent)
            else:
                f = IsoFile(entry['path'], parent=parent, isDir=False,
                    fileOffs = entry['fileOffs'],
                    offset   = entry['fileOffs'],
                    size     = entry['fileSize'],
                    file     = file)
            self.files.append(f)

        return self


    def _readEntry(self, file):
        try:
            offs     = file.tell()
            nameOffs = file.readu32()
            fileOffs = file.readu32()
            size     = file.readu32()
        except struct.error:
            raise RuntimeError("Corrupted FST")
        if ((nameOffs >> 24) & 0xFF) > 1:
            raise ValueError("Corrupted FST entry @ %08X: %08X %08X %08X" % (
                offs, nameOffs, fileOffs, size))
        entry    = {
            'isDir':   (nameOffs >> 24) != 0,
            'nameOffs': nameOffs & 0xFFFFFF,
            'idx':      len(self._entries),
        }
        if entry['isDir']:
            entry['parentIdx'] = fileOffs # parent directory idx
            entry['nextIdx']   = size # next idx which is NOT in this dir
        else:
            entry['fileOffs'] = fileOffs
            entry['fileSize'] = size
        return entry


    def _readFstDir(self, idx, path, _depth=0):
        assert _depth < 10, "Maximum depth exceeded"
        entry = self._entries[idx]
        #entry['path'] = "%s/%s" % (path, entry['name'])
        idx += 1
        while idx < entry['nextIdx'] and idx < len(self._entries):
            f = self._entries[idx]
            f['path'] = "%s/%s" % (path, f['name'])
            f['parent'] = entry
            if f['isDir']: idx = self._readFstDir(idx, f['path'], _depth+1)
            else: idx += 1
        return idx


    def build(self):
        self._entries = []
        strTabOffs = 0
        strTable   = {} # str => offset
        self._strTable = []
        for i, file in enumerate(self.files):
            name = file.path.split('/')[-1]
            if name in strTable: nameOffs = strTable[name]
            else:
                # XXX we're re-using previous string table entries
                # when the same filename appears multiple times.
                # the original Star Fox Adventures does NOT do this
                # and actually has duplicate entries.
                # Does that matter? Should we be making duplicate
                # entries too?
                # (if not, we could even possibly go further and reuse
                # partial strings, eg "foobar.bin" and "bar.bin", but
                # would that ever come up in practice?)
                nameOffs = strTabOffs
                strTable[name] = strTabOffs
                strTabOffs += len(name) + 1 # for null
                self._strTable.append(bytes(name, 'utf-8') + b'\0')

            entry = {
                'isDir':    file.isDir,
                'nameOffs': nameOffs,
            }
            if i == 0: # root
                entry['parentIdx'] = 0
                entry['nextIdx']   = len(self.files)
            elif file.isDir:
                entry['parentIdx'] = 0
                entry['nextIdx']   = len(self.files)
                if file.parent is not None:
                    #print("file %s parent %s" % (name, file.parent))
                    entry['parentIdx'] = self.files.index(file.parent)
                for j in range(i+1, len(self.files)):
                    if not self.files[j].isDescendantOf(file):
                        entry['nextIdx'] = j
                        break
            else:
                entry['fileOffs']  = file.offset
                entry['fileSize']  = file.size
            self._entries.append(entry)


    def writeToFile(self, file:(str,BinaryFile), chunkSize:int=4096):
        """Write this file's content to disk."""
        if type(file) is str: file = BinaryFile(file, 'wb')

        start = file.tell()
        for i, entry in enumerate(self._entries):
            if entry['isDir']: file.writeStruct('III',
                entry['nameOffs'] | 0x01000000,
                entry['parentIdx'],
                entry['nextIdx'])
            else: file.writeStruct('III',
                entry['nameOffs'],
                entry.get('fileOffs', 0),
                entry.get('fileSize', 0))
        file.write(b''.join(self._strTable))
        size = file.tell() - start
        # pad to 0x20
        ext = size & 0x1F
        if ext != 0: file.write(bytes(0x20 - ext))
        self.size = file.tell() - start


    def dump(self):
        """Dump to console."""
        print("fst.bin @ 0x%08X:" % self.offset)
        print(" File Offset   Size     Prnt Next T Path")
        for i, file in enumerate(self._entries):
            print(" %04X %08X %08X %04X %04X %s \"%s\"" % (i,
                file.get('fileOffs', 0),
                file.get('fileSize', 0),
                file.get('parentIdx', 0),
                file.get('nextIdx', 0),
                'D' if file['isDir'] else 'F',
                file['path']
            ))
