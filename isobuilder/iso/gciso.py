import struct
import os
import os.path
from .binaryfile import BinaryFile
from .           import ISO
from .isofile    import IsoFile
from .dol        import DOL
from .appldr     import Appldr
from .bi2bin     import Bi2Bin
from .bootbin    import BootBin
from .fstbin     import FstBin


class GCISO(ISO):
    """Represents GameCube ISO file."""

    def __init__(self):
        self.bootBin      = None
        self.bi2bin       = None
        self.appldr       = None
        self.fstbin       = None
        self.debugMonitor = None
        self.mainDol      = None
        self.files        = []


    def readFile(self, path:str, offset:int=0):
        """Read ISO file."""
        file = BinaryFile(path, 'rb')

        print("Read boot.bin from 0x%08X" % offset)
        self.bootBin = BootBin(file=file, offset=offset)

        print("Read bi2.bin  from 0x%08X" % (offset+0x0440))
        self.bi2bin  = Bi2Bin (file=file, offset=offset+0x0440)

        print("Read appldr   from 0x%08X" % (offset+0x2440))
        self.appldr  = Appldr (file=file, offset=offset+0x2440)

        print("Read fst.bin  from 0x%08X" % (offset+self.bootBin.fstOffs))
        self.fstbin  = FstBin (file=file, size=self.bootBin.fstSize,
            offset=offset+self.bootBin.fstOffs)

        print("Read main.dol from 0x%08X" % (offset+self.bootBin.mainDolOffs))
        self.mainDol = DOL(file=file, path='main.dol', isSystem=True,
            offset=offset+self.bootBin.mainDolOffs)

        # XXX debug monitor?
        print("Read files    from 0x%08X" % (offset+self.bootBin.mainDolOffs+self.mainDol.size))
        self.files = self.fstbin.files
        return self


    def readDir(self, path:str):
        """Read extracted directory."""
        self.readSystemFilesFromDir(os.path.join(path, 'sys'))
        self.files = []
        self._addDir(path+'/files/')
        self.fstbin = FstBin(files=self.files)


    def readSystemFilesFromDir(self, path):
        bootBin = os.path.join(path, 'boot.bin')
        bi2bin  = os.path.join(path, 'bi2.bin')
        appldr  = os.path.join(path, 'apploader.img')
        mainDol = os.path.join(path, 'main.dol')
        F       = os.path.isfile

        if F(bootBin): self.bootBin = BootBin(file=bootBin)
        if F(bi2bin):  self.bi2bin  = Bi2Bin (file=bi2bin)
        if F(appldr):  self.appldr  = Appldr (file=appldr)
        if F(mainDol): self.mainDol = DOL    (file=mainDol,
            isSystem=True, path='main.dol')


    def _addDir(self, path:str, basePath:str='/', parent=None, _depth=0):
        assert _depth < 10, "Maxmimum depth exceeded"
        dirFile = IsoFile(basePath, isDir=True, parent=parent)
        self.files.append(dirFile)

        # for some reason these have to be in a certain order.
        # sorting this list breaks everything.
        # even Dolphin won't show the banner image or recognize
        # that there are any files.
        names = list(os.listdir(path))

        # add dirs first
        for name in names:
            if name in ('.', '..'): continue
            fullPath = path+'/'+name
            fullBase = basePath+'/'+name
            if   os.path.isdir(fullPath):
                self._addDir(fullPath, fullBase,
                    parent=dirFile, _depth=_depth+1)

        # now add files
        for name in names:
            if name in ('.', '..'): continue
            fullPath = path+'/'+name
            fullBase = basePath+'/'+name
            if os.path.isfile(fullPath):
                file = self.addFile(fullPath, fullBase)
                file.parent = dirFile


    def addFilesFromFst(self, fst:FstBin, path:str):
        """Add files listed in the given FST."""
        self.files = []
        self.readSystemFilesFromDir(os.path.join(path, 'sys'))
        for file in fst.files:
            if file.isDir:
                self.files.append(IsoFile(file.path, isDir=True))
            else:
                self.addFile(os.path.join(path, 'files', '.'+file.path),
                    file.path)

        # set parents
        filesByPath = {}
        for file in self.files:
            filesByPath[file.path] = file

        for i, file in enumerate(self.files):
            oldFile = fst.files[i]
            if oldFile.parent is not None:
                #print("file %s parent %s" % (file, oldFile.parent))
                file.parent = filesByPath[oldFile.parent.path]

        self.fstbin = FstBin(files=self.files)


    def addFile(self, path:str, name:str) -> IsoFile:
        """Add a file to the ISO.

        path: local file path.
        name: ISO file path.
        """
        #print("add file", path, "name", name)
        file = IsoFile(name, file=path, size=os.stat(path).st_size)
        self.files.append(file)
        return file


    def addOrReplaceFile(self, path:str, name:str):
        """Add a file to the ISO, or replace one at the same path.

        path: local file path.
        name: ISO file path.
        """
        for i, file in enumerate(self.files):
            if file.path == name:
                print("Replace:", name)
                file.replaceWith(path)
                return
        print("Add file: ", name)
        file = self.addFile(path, name)
        file.parent = self.getFile(os.path.dirname(name))


    def createDir(self, name:str):
        """Ensure the specified directory exists.

        name: ISO file path.
        """
        file = self.getFile(name)
        if file:
            if file.isDir: return
            raise NotADirectoryError("File %s already exists and is not a directory" % name)
        print("Add dir:  ", name)
        file = IsoFile(name, isDir=True)
        file.parent = self.getFile(os.path.dirname(name))
        self.files.append(file)


    def getFile(self, path:str) -> IsoFile:
        """Retrieve a file from the ISO.

        path: ISO file path.
        Returns the IsoFile, or None if not found.
        """
        for file in self.files:
            if file.path == path: return file
        return None


    def extract(self, path:str):
        """Extract ISO contents to directory."""
        # create target dirs
        os.makedirs(os.path.join(path,'sys'), exist_ok=True)
        os.makedirs(os.path.join(path,'files'), exist_ok=True)

        # extract system files
        print("Extract boot.bin...")
        self.bootBin.writeToFile(os.path.join(path,'sys','boot.bin'))
        print("Extract bi2.bin...")
        self.bi2bin .writeToFile(os.path.join(path,'sys','bi2.bin'))
        print("Extract fst.bin...")
        self.fstbin .writeToFile(os.path.join(path,'sys','fst.bin'))
        print("Extract apploader.img...")
        self.appldr .writeToFile(os.path.join(path,'sys','apploader.img'))
        print("Extract main.dol...")
        self.mainDol.writeToFile(os.path.join(path,'sys','main.dol'))

        # extract directories
        dirs = list(filter(lambda f: f.isDir, self.files))
        for i, file in enumerate(dirs):
            name = file.path
            if name.startswith('/'): name = '.'+name
            target = os.path.join(path, 'files', name)
            print("\r\x1B[KExtract dir  %5d/%5d: %s" % (
                i+1, len(dirs), target), end='', flush=True)
            os.makedirs(target, exist_ok=True)
        print('')

        # extract files
        files = list(filter(lambda f: not f.isDir, self.files))
        for i, file in enumerate(files):
            if '..' in file.path:
                raise NameError("Invalid file path: %s" % str(file.path))
            name = file.path
            if name.startswith('/'): name = '.'+name
            target = os.path.join(path, 'files', name)
            print("\r\x1B[KExtract file %5d/%5d: %s" % (
                i+1, len(files), target), end='', flush=True)
            out = BinaryFile(target, 'wb')
            file.writeToFile(out)
        print('')


    def writeToFile(self, file:(str,BinaryFile), chunkSize:int=4096):
        """Write ISO to disk."""
        if type(file) is str: file = BinaryFile(file, 'wb')

        # Sort files by path, with the exception that '/'
        # must sort before '.' (producing eg: "/foo", "/foo/bar.bin",
        # "/foo.bin"), since the FST requires that entries directly
        # follow their parents.
        # If we don't do this, the game will silently fail to load
        # any files that aren't in order.
        print("Building FST...")
        self.files.sort(key=lambda f: f.path.lower().replace('/', '\x01'))
        self.fstbin.files = self.files
        self.fstbin.build()

        # right now, boot.bin and fst.bin contain outdated
        # offsets, but we'll fix that later.

        print("Write boot.bin at 0x%08X" % file.tell())
        self.bootBin.writeToFile(file, chunkSize=chunkSize)

        print("Write bi2.bin  at 0x%08X" % file.tell())
        self.bi2bin .writeToFile(file, chunkSize=chunkSize)

        print("Write appldr   at 0x%08X" % file.tell())
        self.appldr .writeToFile(file, chunkSize=chunkSize)
        self._alignTo(file, 2048)

        print("Write main.dol at 0x%08X" % file.tell())
        self.mainDol.writeToFile(file, chunkSize=chunkSize)
        self._alignTo(file, 256) # XXX fishy

        fstOffs = file.tell()
        self.fstbin.files = self.files
        self.fstbin.build()
        print("Write fst.bin  at 0x%08X" % file.tell())
        self.fstbin .writeToFile(file, chunkSize=chunkSize)
        # XXX why this padding? Dolphin refuses to see any files
        # if we don't have this.
        # Is there another reference to 0x378000 we missed?
        # I don't see any in the system files, except possibly
        # main.dol, but that shouldn't affect Dolphin.
        # In fact, the original ISO has what appears to be random
        # noise as padding here (well, not here, because its string
        # table is larger due to less optimization).
        # I can't find any reference to, or structre in, that data.
        # it may well be completely random.
        file.writePadding(0x8000)
        self._alignTo(file, 0x8000)

        fileOffs = file.tell()
        print("Write files    at 0x%08X" % fileOffs)
        files = list(filter(lambda f: not f.isDir, self.files))
        for i, f in enumerate(files):
            offset = file.tell()
            f.offset = offset
            print("\r\x1B[KWrite %5d/%5d at 0x%08X size 0x%08X: %s" % (
                i, len(files), offset, f.size, f.path),
                end='', flush=True)
            f.writeToFile(file, chunkSize=chunkSize)
            self._alignTo(file, 4)
        print('')

        # now go back and update the FST
        # now that we know the file offsets.
        print("Updating FST...")
        self.fstbin.files = self.files
        self.fstbin.build()
        file.seek(fstOffs)
        self.fstbin .writeToFile(file, chunkSize=chunkSize)

        # and update the file offset in boot.bin
        # now that we know what it is.
        print("Updating boot.bin...")
        self.bootBin.fileOffset = fileOffs
        self.bootBin.fstSize    = self.fstbin.size
        self.bootBin.maxFstSize = max(self.fstbin.size, self.bootBin.maxFstSize)
        #self.bootBin.unk430 = 0x80400000 - self.fstbin.size
        file.seek(self.bootBin.offset)
        self.bootBin.writeToFile(file, chunkSize=chunkSize)


    def _alignTo(self, file:BinaryFile, size:int):
        """Write padding to file until aligned to size."""
        pos = file.tell()
        cnt = pos % size
        if cnt > 0: file.write(bytes(size - cnt))


    def dump(self):
        """Dump to console."""
        self.bootBin.dump()
        self.bi2bin .dump()
        self.appldr .dump()
        self.mainDol.dump()
        self.fstbin .dump()
