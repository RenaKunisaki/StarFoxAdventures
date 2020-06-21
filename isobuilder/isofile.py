from binaryfile import BinaryFile

class IsoFile:
    """A file in an ISO."""

    def __init__(self, path:str, isDir:bool=False, offset:int=0, size:int=0, file:BinaryFile=None, parent=None, isSystem=False):
        """Create file in ISO.

        path:   file path within ISO.
        isDir:  whether this represents a directory (True) or file (False).
        offset: file data offset within ISO.
        size:   file data size in bytes.
        file:   ISO file to read from.
        parent: parent IsoFile.
        isSystem: whether this is a system file (extracted to /sys
            instead of /files).
        """
        self.path     = path
        self.isDir    = isDir
        self.offset   = offset
        self.size     = size
        self.file     = file
        self.parent   = parent
        self.isSystem = False # is system file?


    def __str__(self):
        return '<ISO %s "%s" @ 0x%x>' % (
            'Directory' if self.isDir else 'File',
            self.path, id(self))


    def isDescendantOf(self, file):
        """Check if this file is a child or descendant of the given file."""
        return (self.parent == file) or (self.parent is not None and self.parent.isDescendantOf(file))


    def writeToFile(self, file:(str,BinaryFile), chunkSize:int=4096):
        """Write this file's content to disk."""
        if self.isDir: raise IsADirectoryError(self)
        if not self.file:
            raise FileNotFoundError("no ISO file to read from")

        inFile = self.file
        if type(inFile) is str: inFile = BinaryFile(inFile, 'rb')

        if type(file) is str: file = BinaryFile(file, 'wb')
        #print("file %s at 0x%08X size 0x%08X in %s" % (
        #    self.path, self.offset, self.size, file))
        inFile.seek(self.offset)
        for i in range(0, self.size, chunkSize):
            size = min(chunkSize, self.size - i)
            data = inFile.read(size)
            if len(data) != size:
                raise RuntimeError("Expected 0x%X bytes but got 0x%X at 0x%X in %s" % (
                    size, len(data), inFile.tell(), self.path))
            file.write(data)
