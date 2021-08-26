#!/usr/bin/env python3
import os
import os.path
import sys
import inspect
import hashlib
import tempfile
from iso.gciso import GCISO
from iso.binaryfile import BinaryFile
from iso.openingbnr import OpeningBnr
from apptest import AppTest


def printf(fmt, *args):
    print(fmt % args, end='')

class App:
    def __init__(self):
        self._overwrite = False # overwrite an existing target ISO file
        self._gameName  = None # overwrite game name in ISO

    def help(self):
        """Display help text."""
        methods = [
            (func, getattr(self, func))
            for func in dir(self)
            if callable(getattr(self, func))
            and not func.startswith('_')
        ]
        for name, method in sorted(methods, key = lambda it: it[0]):
            doc = method.__doc__.split('\n')
            printf("%s: %s\n", name, doc[0])
            for line in doc[1:]:
                line = line.strip()
                if line != '': print('    '+line)


    def test(self, isoPath:str, filesPath:str):
        """Perform self test.

        isoPath: path to a GameCube ISO.
        filesPath: path to directory containing files extracted
            from said ISO.

        The files are not modified during the test.
        """
        AppTest().runAllTests(isoPath, filesPath)


    def overwrite(self):
        """Overwrite target file instead of replacing it.

        Use this option if you want to retain the padding in empty regions.
        """
        self._overwrite = True


    def setName(self, name:str):
        """Set the game title in the target ISO."""
        self._gameName = name


    def readIso(self, path:str):
        """Read an ISO file and print information about it."""
        iso = GCISO().readFile(path)
        iso.dump()


    def extractIso(self, isoPath:str, outPath:str):
        """Extract files from ISO."""
        iso = GCISO().readFile(isoPath)
        iso.extract(outPath)


    def buildIso(self, filesPath:str, outPath:str):
        """Build ISO from files."""
        iso = GCISO()
        iso.readDir(filesPath)
        if self._gameName is not None: iso.bootBin.setGameName(self._gameName)
        if self._overwrite: iso.writeToFile(BinaryFile(outPath, 'r+b'))
        else: iso.writeToFile(outPath)


    def patchIso(self, isoPath:str, destPath:str, files:str):
        """Build a modified ISO given an original ISO and a set of
        replacement files.

        isoPath:  path to original ISO.
        destPath: path to write new ISO.
        files:    path to directory containing replacement files.
        """
        iso = GCISO().readFile(isoPath)
        if self._gameName is not None: iso.bootBin.setGameName(self._gameName)
        filePath = os.path.join(files, 'files')
        self._doReplaceIsoFiles(iso, filePath, filePath)
        iso.readSystemFilesFromDir(os.path.join(files, 'sys'))
        # XXX do we need some way to delete files?
        if self._overwrite:
            try: out = BinaryFile(destPath, 'r+b')
            except FileNotFoundError: out = BinaryFile(destPath, 'wb')
            iso.writeToFile(out)
        else: iso.writeToFile(destPath)


    def _doReplaceIsoFiles(self, iso:GCISO, basePath:str, files:str, _depth=0):
        assert _depth < 10, "Maxmimum depth exceeded"
        #print("replace files from", files)
        names = list(filter(lambda n: n not in ('.', '..'),
            os.listdir(files)))
        for name in names:
            path = os.path.join(files, name)
            isoPath = path
            if isoPath.startswith(basePath):
                isoPath = isoPath[len(basePath):] # keep leading slash

            #print("add or replace file", path, isoPath, "base", basePath)
            if os.path.isdir(path):
                iso.createDir(isoPath)
                self._doReplaceIsoFiles(iso, basePath, path, _depth=_depth+1)
            else:
                iso.addOrReplaceFile(path, isoPath)

    def extractBanner(self, banner:str, outImage:str, outText:str):
        """Extract opening.bnr file.

        banner:   path to banner file to extract.
        outImage: path to write image to.
        outText:  path to write text to.
        """
        bnr = OpeningBnr(0, banner)
        if outImage != '': bnr.toImage().save(outImage)
        if outText  != '': bnr.toXml(outText)

    def buildBanner(self, banner:str, image:str, text:str):
        """Create opening.bnr file.

        banner:   path to banner file to write to.
        image:    path to read image from.
        text:     path to read text from.
        """
        bnr = OpeningBnr()
        bnr.fromImage(image)
        bnr.fromXml(text)
        bnr.writeToFile(banner)


def main(*args):
    app = App()
    if len(args) < 1:
        app.help()
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
        fArgs = args[1:nArg+1]
        args  = args[nArg+1:]

        if len(fArgs) < nArg:
            msg = [method]
            for name in sig.parameters:
                msg.append(name)
            print("Usage:", ' '.join(msg))
            return 1

        func(*fArgs)
    return 0

if __name__ == '__main__':
    sys.exit(main(*sys.argv[1:]))
