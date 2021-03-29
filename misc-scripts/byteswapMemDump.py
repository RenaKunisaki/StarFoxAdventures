#!/usr/bin/env python3
"""Given a memory dump that I managed to tear out of an emulator,
swap the words in it and truncated it to 8MB.
"""
import sys

with open(sys.argv[1], 'rb') as inFile:
    with open(sys.argv[2], 'wb') as outFile:
        for i in range(0, 8*1024*1024, 4):
            b = inFile.read(4)
            outFile.write(b[::-1])
