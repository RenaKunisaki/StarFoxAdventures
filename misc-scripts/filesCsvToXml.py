#!/usr/bin/env python3
"""Convert file list CSV to XML."""
import sys
import os
import os.path
import xml.etree.ElementTree as ET
import csv

root = ET.Element('files')
with open(sys.argv[1]) as inFile:
    reader = csv.reader(inFile)
    for row in reader:
        path, size, ascii, hex, comment = row
        if path.startswith('discroot/'):
            path = path[9:]
        eFile = ET.SubElement(root, 'file', {
            'path': path,
            'size': size,
        })
        comment = comment.strip()
        if comment != '':
            ET.SubElement(eFile, 'description').text = comment

print(ET.tostring(root, encoding='unicode'))
