#!/usr/bin/env python3
# iterate a directory and report which files are present in which
# subdirectories. output as CSV.
import os
import os.path

knownFiles = {}
knownDirs  = {}

def listDir(path, _depth=0):
    assert _depth < 5
    knownDirs[path] = True
    for name in os.listdir(path):
        if name != "." and name != "..":
            fullPath = os.path.join(path, name)
            if os.path.isdir(fullPath):
                if _depth < 1:
                    listDir(fullPath, _depth+1)
            elif os.path.isfile(fullPath):
                if name not in knownFiles:
                    knownFiles[name] = {}
                knownFiles[name][path] = True

listDir('../discroot')

print("file;" + (";".join(knownDirs.keys())))
for name, file in knownFiles.items():
    row = [name]
    for dirName in knownDirs.keys():
        row.append('Y' if dirName in file else ' ')
    print(';'.join(row))
