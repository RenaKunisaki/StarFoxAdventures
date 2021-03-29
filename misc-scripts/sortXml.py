#!/usr/bin/env python3
"""Sort types.xml and symbols.xml alphabetically and split into multiple files."""

import sys
import os
import os.path
import subprocess
import xml.etree.ElementTree as ET

_keyFunc = lambda e: e.get('name', '').lower().replace('_', '')

def doTypesXml(inPath, outPath):
    """Sort/split types.xml."""
    inFile = ET.parse(inPath)

    outCats = {}
    for elem in list(sorted(inFile.getroot(), key=_keyFunc)):
        cat = elem.get('category', '')
        if cat not in outCats:
            outCats[cat] = ET.Element('types')
        outCats[cat].append(elem)

    for name, root in outCats.items():
        while name.startswith('/'): name = name[1:]
        path = os.path.join(outPath, os.path.dirname(name))
        os.makedirs(path, exist_ok=True)
        if name == '': name = 'root'
        xmlPath = os.path.join(outPath, name+'.xml')
        ET.ElementTree(root).write(xmlPath+'.tmp')
        text = subprocess.run(['xmllint', '--format', xmlPath+'.tmp'],
            capture_output=True, encoding='utf-8').stdout
        with open(xmlPath, 'wt') as file:
            file.write(text)
        os.remove(xmlPath+'.tmp')


def doGenericXml(inPath, outPath):
    """Sort a generic XML document by the 'name' attribute."""
    inFile  = ET.parse(inPath)
    outRoot = ET.Element(inFile.getroot().tag)
    outDoc  = ET.ElementTree(outRoot)
    for elem in list(sorted(inFile.getroot(), key=_keyFunc)):
        outRoot.append(elem)
    ET.ElementTree(outRoot).write(outPath)


def printUsage():
    print("Usage: sortXml.py mode inFile outPath")
    print("Mode: types, or generic")
    sys.exit(1)

def main():
    args = list(sys.argv[1:])
    if len(args) < 3: printUsage()

    mode    = args.pop(0)
    inPath  = args.pop(0)
    outPath = args.pop(0)
    if   mode == 'types':   doTypesXml  (inPath, outPath)
    elif mode == 'generic': doGenericXml(inPath, outPath)
    else:
        print("Invalid mode")
        printUsage()


if __name__ == '__main__':
    main()
