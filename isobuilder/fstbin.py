import struct
from binaryfile import BinaryFile
from isofile import IsoFile

class FstBin(IsoFile):
    """fst.bin file; contains ISO directory structure."""

    def __init__(self, offset:int=0, size:int=0, file:BinaryFile=None):
        super().__init__("fst.bin", isDir=False, offset=offset,
            size=size, file=file)
        self._entries = [] # internal list
        self.files    = [] # IsoFile
        self.isSystem = True

        if file is not None:
            self.readFile(file, offset)


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
        for i, entry in enumerate(self._entries):
            file.seek(strTabOffs + entry['nameOffs'] + offset)
            entry['name'] = file.readString()


        # read directory structure
        self._readFstDir(0, '')

        # convert to IsoFile
        for entry in self._entries:
            if entry['isDir']:
                f = IsoFile(entry['path'], isDir=True)
            else:
                f = IsoFile(entry['path'], isDir=False,
                    offset=entry['fileOffs'],
                    size=entry['fileSize'],
                    file=file)
            self.files.append(f)

        return self


    def _readEntry(self, file):
        offs     = file.tell()
        nameOffs = file.readu32()
        fileOffs = file.readu32()
        size     = file.readu32()
        if ((nameOffs >> 24) & 0xFF) > 1:
            raise ValueError("Corrupted FST entry @ %08X: %08X %08X %08X" % (
                offs, nameOffs, fileOffs, size))
        entry    = {
            'isDir':   (nameOffs >> 24) != 0,
            'nameOffs': nameOffs & 0xFFFFFF,
        }
        if entry['isDir']:
            entry['parentIdx'] = fileOffs
            entry['nextIdx']   = size
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
            if f['isDir']: idx = self._readFstDir(idx, f['path'], _depth+1)
            else: idx += 1
        return idx


    def dump(self):
        """Dump to console."""
        print("fst.bin:")
        print(" File Offset   Size     T Path")
        for i, file in enumerate(self._entries):
            print(" %4d %08X %08X %s \"%s\"" % (i,
                file.get('fileOffs', 0),
                file.get('fileSize', 0),
                'D' if file['isDir'] else 'F',
                file['path']
            ))
