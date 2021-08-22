#!/usr/bin/env python3
"""Pad the end of a file to ensure its size is a multiple of 32."""
import os
import sys

args = list(sys.argv[1:])
while len(args):
    path = args.pop(0)
    try:
        with open(path, 'r+b') as file:
            file.seek(0, 2) # to end
            pad = file.tell() % 32
            if pad > 0: file.write(b'\0' * (32 - pad))
    except IsADirectoryError: pass
