from __future__ import annotations
import struct

# chars we don't want to put in filenames.
# some of these might not actually be disallowed anywhere
# but are still troublesome.
BAD_DIR_CHARS = ' !#$%&*()/\\?:\'"`~<>.'

def printf(fmt, *args):
    print(fmt % args, end='')

def readStruct(file, fmt:str, offset:int=None):
    """Read a struct from a file."""
    if offset is not None: file.seek(offset, 0)
    size = struct.calcsize(fmt)
    res  = struct.unpack(fmt, file.read(size))
    if len(res) == 1: res = res[0] # grumble
    return res

def writeStruct(file, fmt:str, *items, offset:int=None) -> None:
    """Write a struct to a file."""
    if offset is not None: file.seek(offset, 0)
    file.write(struct.pack(fmt, *items))
