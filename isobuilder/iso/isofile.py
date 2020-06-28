import os
from .binaryfile import BinaryFile

class IsoFile:
    """A file in an ISO."""

    def __init__(self, path:str, isDir:bool=False, offset:int=0,
    size:int=0, file:BinaryFile=None, fileOffs:int=0, parent=None,
    isSystem=False):
        """Create file in ISO.

        path:     file path within ISO.
        isDir:    whether this represents a directory (True) or file (False).
        offset:   file data offset within ISO.
        size:     file data size in bytes.
        file:     ISO or data file to read from.
        fileOffs: offset to read data from within file.
        parent:   parent IsoFile.
        isSystem: whether this is a system file (extracted to /sys
            instead of /files).
        """
        self.path     = path
        self.isDir    = isDir
        self.offset   = offset
        self.size     = size
        self.file     = file
        self.fileOffs = fileOffs
        self.parent   = parent
        self.isSystem = False # is system file?


    def __str__(self):
        return '<ISO %s "%s" @ 0x%x>' % (
            'Directory' if self.isDir else 'File',
            self.path, id(self))


    def isDescendantOf(self, file):
        """Check if this file is a child or descendant of the given file."""
        return (self.parent == file) or (self.parent is not None and self.parent.isDescendantOf(file))


    def replaceWith(self, file:(str,BinaryFile), offset:int=0, size:int=None):
        """Replace this file with one from the local disk.

        file:   file to replace with.
        offset: offset within file to read from.
        size:   data size. if None, will use file size - offset.
        """
        #if type(file) is str: file = BinaryFile(file, 'rb')
        self.file     = file
        self.fileOffs = offset
        if size is None:
            if type(file) is str:
                size = os.stat(file).st_size - offset
            else:
                file.seek(offset)
                size = file.size - offset
        self.size = size


    def writeToFile(self, file:(str,BinaryFile), chunkSize:int=4096):
        """Write this file's content to disk."""
        if self.isDir: raise IsADirectoryError(self)
        if not self.file:
            raise FileNotFoundError("no source file to read from")

        inFile = self.file
        if type(inFile) is str: inFile = BinaryFile(inFile, 'rb')

        if type(file) is str: file = BinaryFile(file, 'wb')
        #print("file %s at 0x%08X size 0x%08X in %s" % (
        #    self.path, self.offset, self.size, file))
        inFile.seek(self.fileOffs)
        for i in range(0, self.size, chunkSize):
            size = min(chunkSize, self.size - i)
            data = inFile.read(size)
            if len(data) != size:
                raise RuntimeError("Expected 0x%X bytes but got 0x%X at 0x%X in %s" % (
                    size, len(data), inFile.tell(), self.path))
            file.write(data)
