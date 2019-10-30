#!/usr/bin/env python3
# generate a directory listing recursively with size and data preview.
import os
import os.path

def listFile(path):
    size = os.stat(path).st_size
    with open(path, 'rb') as file:
        data = file.read(16)
    s = []
    for c in data:
        if c >= 0x20 and c <= 0x7E: s.append(chr(c))
        else: s.append('.')
    print("%s|%9d|%s|%s" % (path, size, ''.join(s),
        ''.join(map(lambda b: '%02X ' % b, data))))

def listDir(path, _depth=0):
    assert _depth < 5
    for name in os.listdir(path):
        if name != "." and name != "..":
            fullPath = os.path.join(path, name)
            if os.path.isdir(fullPath):
                listDir(fullPath, _depth+1)
            elif os.path.isfile(fullPath):
                listFile(fullPath)

listDir('discroot')
