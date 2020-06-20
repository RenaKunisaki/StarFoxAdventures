import struct
import os
import os.path
from binaryfile import BinaryFile
from iso import ISO
from dol import DOL
from appldr import Appldr
from bi2bin import Bi2Bin
from bootbin import BootBin
from fstbin import FstBin


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
        self.bootBin      = BootBin(file=file, offset=offset)
        self.bi2bin       = Bi2Bin (file=file, offset=offset+0x0440)
        self.appldr       = Appldr (file=file, offset=offset+0x2440)
        self.fstbin       = FstBin (file=file,
            offset=offset+self.bootBin.fstOffs,
            size=self.bootBin.fstSize)
        self.mainDol      = DOL(file=file,
            offset=offset+self.bootBin.mainDolOffs,
            path='main.dol', isSystem=True)

        # XXX debug monitor?
        self.files = self.fstbin.files
        return self


    def readDir(self, path:str):
        """Read extracted directory."""
        self.bootBin.readFile(path+'/sys/boot.bin')
        self.bi2bin .readFile(path+'/sys/bi2.bin')
        self.appldr .readFile(path+'/sys/apploader.img')
        self.mainDol.readFile(path+'/sys/main.dol')
        self.files = []
        self._addDir(path+'/files/')


    def _addDir(self, path:str, basePath:str='/', _depth=0):
        assert _depth < 10, "Maxmimum depth exceeded"
        for name in os.listdir(path):
            if name in ('.', '..'): continue
            fullPath = path+'/'+name
            fullBase = basePath+'/'+name
            if   os.path.isdir(fullPath):
                self._addDir(fullPath, fullBase, _depth+1)
            elif os.path.isfile(fullPath):
                self.addFile(fullPath, fullBase)
            else: print("Skipping object:", fullPath)


    def addFile(self, path:str, name:str):
        """Add a file to the ISO.

        path: local file path.
        name: ISO file path.
        """
        self.files.append(IsoFile(path, name))


    def readFst(self, path:str, offset:int=0):
        """Read filesystem table (fst.bin)."""
        raise NotImplementedError # XXX do we need this?


    def extract(self, path:str):
        """Extract ISO contents to directory."""
        # create target dirs
        os.makedirs(os.path.join(path,'sys'), exist_ok=True)
        os.makedirs(os.path.join(path,'files'), exist_ok=True)
        for file in filter(lambda f: f.isDir, self.files):
            name = file.path
            if name.startswith('/'): name = '.'+name
            target = os.path.join(path, 'files', name)
            #print("mkdir", target)
            os.makedirs(target, exist_ok=True)

        # extract system files
        self.bootBin.writeToFile(os.path.join(path,'sys','boot.bin'))
        self.bi2bin .writeToFile(os.path.join(path,'sys','bi2.bin'))
        self.fstbin .writeToFile(os.path.join(path,'sys','fst.bin'))
        self.appldr .writeToFile(os.path.join(path,'sys','apploader.img'))
        self.mainDol.writeToFile(os.path.join(path,'sys','main.dol'))

        # extract files
        for file in filter(lambda f: not f.isDir, self.files):
            if '..' in file.path:
                raise NameError("Invalid file path: %s" % str(file.path))
            name = file.path
            if name.startswith('/'): name = '.'+name
            target = os.path.join(path, 'files', name)
            #print("extract", target)
            out = BinaryFile(target, 'wb')
            file.writeToFile(out)


    def dump(self):
        """Dump to console."""
        self.bootBin.dump()
        self.bi2bin .dump()
        self.appldr .dump()
        self.mainDol.dump()
        self.fstbin .dump()
