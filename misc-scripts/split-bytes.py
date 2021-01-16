#!/usr/bin/env python3
"""Given a byte string, split it into rows."""
import os
import sys

def showUsage(msg):
    if msg is not None: print(msg)
    print("""Usage: split-bytes.py [options...] [infile] [outfile]
    options:
        -cN:  split to N columns
        -dNN: byte NN (hex) ends a row (can be used multiple times)
        -h:   show this message
        -n:   do not add spaces
    If `infile` is missing or '-', read from stdin.
    If `outfile` is missing or '-', write to stdout.
    Non-hex digits are ignored.
    """)
    sys.exit(1 if msg is not None else 0)


def main():
    inFile, outFile = sys.stdin, sys.stdout
    rowLen, rowDelim, sep = None, set(), ' '

    try:
        args = list(sys.argv[1:])
        while len(args) > 0 and args[0].startswith('-') and len(args[0]) > 1:
            arg = args.pop(0)
            c   = arg[1]
            if   c == 'c': rowLen = int(arg[2:])
            elif c == 'd': rowDelim.add(int(arg[2:], 16))
            elif c == 'h': return showUsage(None)
            elif c == 'n': sep = ''
            else: return showUsage("Invalid option.")
    except Exception as ex:
        return showUsage("Invalid parameter.")

    if len(args) > 0:
        f = args.pop(0)
        if f != '-': inFile  = open(f, 'rt')
    if len(args) > 0:
        f = args.pop(0)
        if f != '-': outFile = open(f, 'wt')

    byte = ''
    curLen = 0
    while True:
        c = inFile.read(1)
        if len(c) == 0: break
        if c in '0123456789abcdefABCDEF':
            byte += c
            if len(byte) == 2:
                curLen += 1
                val = int(byte, 16)
                el = sep
                if (val in rowDelim) or (curLen >= rowLen):
                    el = '\n'
                    curLen = 0
                outFile.write(byte+el)
                byte = ''
    outFile.write('\n')


if __name__ == '__main__':
    main()
