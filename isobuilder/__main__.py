#!/usr/bin/env python3
import os
import os.path
import sys
import inspect
import hashlib
import tempfile
from iso.gciso import GCISO
from apptest import AppTest


def printf(fmt, *args):
    print(fmt % args, end='')

class App:
    def help(self):
        """Display help text."""
        methods = [
            (func, getattr(self, func))
            for func in dir(self)
            if callable(getattr(self, func))
            and not func.startswith('_')
        ]
        for name, method in sorted(methods, key = lambda it: it[0]):
            printf("%s: %s\n", name, method.__doc__)


    def test(self, isoPath:str, filesPath:str):
        """Perform self test.

        isoPath: path to a GameCube ISO.
        filesPath: path to directory containing files extracted
            from said ISO.

        The files are not modified during the test.
        """
        AppTest().runAllTests(isoPath, filesPath)


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
        iso.writeToFile(outPath)


    def patchIso(self, isoPath:str, destPath:str, files:str):
        """Build a modified ISO given an original ISO and a set of
        replacement files.

        isoPath:  path to original ISO.
        destPath: path to write new ISO.
        files:    path to directory containing replacement files.
        """
        iso = GCISO().readFile(isoPath)
        filePath = os.path.join(files, 'files')
        self._doReplaceIsoFiles(iso, filePath, filePath)
        iso.readSystemFilesFromDir(os.path.join(files, 'sys'))
        # XXX do we need some way to delete files?
        iso.writeToFile(destPath)


    def _doReplaceIsoFiles(self, iso:GCISO, basePath:str, files:str, _depth=0):
        assert _depth < 10, "Maxmimum depth exceeded"
        names = list(filter(lambda n: n not in ('.', '..'),
            os.listdir(files)))
        for name in names:
            path = os.path.join(basePath, files, name)
            isoPath = path
            if isoPath.startswith(basePath):
                isoPath = isoPath[len(basePath):] # keep leading slash

            if os.path.isdir(path):
                iso.createDir(isoPath)
                self._doReplaceIsoFiles(iso, basePath, path, _depth=_depth+1)
            else:
                iso.addOrReplaceFile(path, isoPath)


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
