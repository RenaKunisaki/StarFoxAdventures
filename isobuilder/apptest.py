import os
import os.path
import sys
import inspect
import tempfile
from gciso import GCISO

class AppTest:
    """Tests for this app."""
    # TODO proper Python unit test framework

    def __init__(self):
        self.isoPath   = None
        self.filesPath = None
        self.tempDir   = None
        self.iso       = None

    def run(self, isoPath:str, filesPath:str):
        """Perform self test.

        isoPath: path to a GameCube ISO.
        filesPath: path to directory containing files extracted
            from said ISO.

        The files are not modified during the test.
        """
        self.isoPath   = isoPath
        self.filesPath = filesPath

        print("Self test: reading ISO...")
        self.iso = GCISO().readFile(isoPath)
        self.tempDirObj = tempfile.TemporaryDirectory()
        self.tempDir = self.tempDirObj.name
        #self.tempDir = './test'

        #print("extract to temp dir", self.tempDir)
        print("Extracting...")
        self.iso.extract(self.tempDir)
        self._verifyExtractedFiles()
        self._verifyNoExtraFiles()
        self._verifySystemFiles()
        print("Test completed")


    def _verifyExtractedFiles(self):
        print("Verify extracted files...")
        for file in self.iso.files:
            target = os.path.join(self.tempDir, 'files', '.'+file.path)
            if file.isDir:
                if not os.path.isdir(target):
                    raise RuntimeError("Directory not extracted: %s" % str(file.path))
            else:
                if not os.path.isfile(target):
                    raise RuntimeError("File not extracted: %s" % str(file.path))

                # verify data
                self._verifyExtractedFileData(file)
        print("Verify extracted files: OK")


    def _verifyExtractedFileData(self, file):
        origPath = os.path.join(self.filesPath, 'files', '.'+file.path)
        newPath  = os.path.join(self.tempDir, 'files', '.'+file.path)
        self._verifyFileData(origPath, newPath)
        #print("OK: %s" % str(file.path))


    def _verifyFileData(self, origPath, newPath):
        offs = 0
        try: origFile = open(origPath, 'rb')
        except FileNotFoundError:
            raise RuntimeError("Extra file extracted: %s" % str(file.path))
        try: newFile = open(newPath, 'rb')
        except FileNotFoundError:
            raise RuntimeError("File not extracted: %s" % str(file.path))

        while True:
            origData = origFile.read(4096)
            newData  = newFile .read(4096)
            if len(origData) < len(newData):
                raise RuntimeError("File extracted incorrectly: %s (extra data at 0x%X)" % (
                    origPath, offs + len(origData)
                ))
            elif len(origData) > len(newData):
                raise RuntimeError("File extracted incorrectly: %s (truncated at 0x%X)" % (
                    origPath, offs + len(newData)
                ))
            if len(origData) == 0: break
            if origData != newData:
                dOffs = 0
                for i in range(len(origData)):
                    if origData[i] != newData[i]:
                        dOffs = i
                        break
                raise RuntimeError("File extracted incorrectly: %s (at 0x%X: 0x%02X should be 0x%02X)" % (
                    origPath,
                    offs + dOffs, newData[dOffs], origData[dOffs]
                ))
            offs += len(origData)


    def _verifyNoExtraFiles(self):
        print("Verify no extra files...")
        self._verifyNoExtraFilesInDir('/')
        print("Verify no extra files: OK")


    def _verifyNoExtraFilesInDir(self, path, _depth=0):
        assert _depth < 10, "Maximum depth exceeded"
        fullPath = os.path.join(self.tempDir, 'files', '.'+path)
        for name in os.listdir(fullPath):
            fullName = os.path.join(fullPath, name)
            origName = os.path.join(self.filesPath, 'files', '.'+path, name)
            relPath  = os.path.join(path, name)
            #print("check", fullName, "vs", origName)
            if os.path.isdir(fullName):
                if not os.path.isdir(origName):
                    raise RuntimeError("Extra directory: %s" % relPath)
                self._verifyNoExtraFilesInDir(relPath)
            else:
                if not os.path.isfile(origName):
                    raise RuntimeError("Extra file: %s" % relPath)


    def _verifySystemFiles(self):
        print("Verify system files...")
        files = ('apploader.img', 'bi2.bin', 'boot.bin', 'fst.bin', 'main.dol')
        for name in files:
            origPath = os.path.join(self.filesPath, 'sys', name)
            newPath  = os.path.join(self.tempDir, 'sys', name)
            self._verifyFileData(origPath, newPath)
