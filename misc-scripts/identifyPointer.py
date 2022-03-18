#!/usr/bin/env python3
"""Given a RAM dump and an address, identify which heap (if any) the address
is in and display its allocation tag.

Command-line parameters:
[-p path]: path to RAM dump file to override hardcoded default.
addr[, addr...]: RAM addresses, in hex (with or without '0x' prefix) to look up.

Output: eg
0x8133FAF0: heap 2 entry 0x02A2 (0x8133F9E0 - 0x8133FC40 size 0x000260 tag 0x8002D91C ObjInstance ID 0x00000819)
<address>: heap <heap idx> entry <entry idx> (<start addr> - <end addr> size <size> tag <tag> <type> ID <ID>)
    address: the user-provided address
    heap idx: which heap
    entry idx: which entry in the heap
    start addr, end addr: memory region of the heap entry containing this address
    size: region size in bytes
    tag: the tag supplied by the game when allocating this entry
    type: the object type, if known
    ID: the entry's unique ID
Amethyst mod patches the allocator to replace the tag (which the game doesn't actually use)
with the address that called the allocator, allowing more precise type identification.
However, this is done in memory at runtime, so some early allocations still have
the original tag.
"""
import sys
import os.path
import math
import struct

RAM_START  = 0x80000000
RAM_SIZE   = 0x01800000
RAM_END    = RAM_START + RAM_SIZE
NUM_HEAPS  = 4
HEAP_TABLE = 0x80340698 & 0x1FFFFFFF
THREAD_QUEUE_HEAD = 0x800000DC
THREAD_QUEUE_TAIL = 0x800000E0

THREADS = {
    0x803A54A0: "thpAudio",
    0x803A6F08: "thpUnk",
    0x803A8348: "thpMovie",
    0x803AB118: "bsod",
    0x803AD848: "main",
}

# since the mod replaces tag with the location of the alloc call,
# we can identify the type of most objects by their tag.
TAGS = {
    0x8002BE20: 'ObjDef',
    0x8002D91C: 'ObjInstance',

    # for non-modded versions and early allocations, the original tags
    0x00000005: "Map Blocks",
    0x00000006: "Texture",
    0x00000009: "Model Data",
    0x0000000A: "Models",
    0x0000000B: "MaybeAudio",
    0x0000000E: "Objects",
    0x00000010: "VOX",
    0x00000011: "Stack", # data type, not call stack
    0x00000017: "Texture Ptrs",
    0x00000018: "Vec3f Array",
    0x0000001A: "ModelStruct",
    0x000000FF: "Unknown 32-byte buffer",
    0x7D7D7D7D: "Data File",
    0x7F7F7FFF: "Compressed File",
    0xFFFF00FF: "IntersectPoint/Savegame",
    0xFFFFFFFF: "Savegame",
}

def printf(fmt, *args, **kwargs):
    print(fmt % args, end='', **kwargs)

def readStruct(file, fmt, offset=None):
    size = struct.calcsize(fmt)
    if offset is not None: file.seek(offset)
    data = file.read(size)
    r = struct.unpack(fmt, data)
    if len(r) == 1: return r[0] # grumble
    return r

def validPtr(addr):
    return RAM_START <= addr < RAM_END

def ptrToOffset(addr):
    return addr - RAM_START

def scanHeap(ram, idx, addr):
    """Search specified heap for specified address."""
    totalSize, usedSize, totalBlocks, usedBlocks, data = \
        readStruct(ram, '>5I', HEAP_TABLE + (idx * 0x14))
    data = ptrToOffset(data)
    for i in range(usedBlocks):
        entry = readStruct(ram, '>IIHHHHIII', data + (i * 0x1C))
        loc, size = entry[0], entry[1]
        tag, uniqueId = entry[6], entry[8]
        #printf("Heap %d entry 0x%04X addr 0x%08X - 0x%08X size 0x%08X tag 0x%08X ID 0x%08X\n",
        #    idx, i, loc, loc+size, size, tag, uniqueId)
        if loc <= addr and (loc + size) > addr:
            return entry, i
    return None, None


def scanThreads(ram, addr):
    """Search thread stacks for specified address."""
    # make list of threads
    threads = set()

    def _readThreads(base, offset): # this is dumb
        pThread = readStruct(ram, '>I', ptrToOffset(base))
        while validPtr(pThread):
            threads.add(pThread)
            pThread = readStruct(ram, '>I', ptrToOffset(pThread)+offset)
    _readThreads(THREAD_QUEUE_HEAD, 0x2FC) # head
    _readThreads(THREAD_QUEUE_HEAD, 0x300) # tail
    _readThreads(THREAD_QUEUE_TAIL, 0x2FC) # head
    _readThreads(THREAD_QUEUE_TAIL, 0x300) # tail

    # and since that queue only contains active threads and there's
    # apparently no way to iterate *all* threads...
    for addr in THREADS.keys(): threads.add(addr)

    # check against each thread's stack
    for thread in threads:
        stackEnd, stackStart = readStruct(ram, '>II', ptrToOffset(thread)+0x304)
        name = THREADS.get(thread, '?')
        #printf("Thread %08X stack %08X - %08X: %s\n",
        #    thread, stackStart, stackEnd, name)
        if addr >= stackStart and addr < stackEnd:
            return thread
    return None


def checkAddr(ram, addr):
    addr = int(addr, 16)
    if addr < 0x8000_0000 or addr >= 0x8180_0000:
        printf("0x%08X: invalid\n", addr)
        return
    for i in range(NUM_HEAPS):
        entry, entryIdx = scanHeap(ram, i, addr)
        if entry:
            tag = TAGS.get(entry[6], '<unknown>')
            printf("0x%08X: heap %d entry 0x%04X (0x%08X - 0x%08X size 0x%06X tag 0x%08X %s ID 0x%08X)\n",
                addr, i, entryIdx, entry[0], entry[0]+entry[1], entry[1],
                entry[6], tag, entry[8])
            return
    thread = scanThreads(ram, addr)
    if thread:
        printf("0x%08X: on stack of thread 0x%08X\n", addr, thread)
    printf("0x%08X: not found\n", addr)

def main(addrs, dumpPath):
    with open(dumpPath, 'rb') as ram:
        for addr in addrs:
            checkAddr(ram, addr)



if __name__ == '__main__':
    args = list(sys.argv[1:])
    if len(args) > 0:
        dumpPath = "/home/rena/.local/share/dolphin-emu/Dump/mem1.raw"
        if args[0] in ('-p', '--path'):
            args.pop(0) # remove
            dumpPath = args.pop(0)
        main(args, dumpPath)
    else:
        print("Usage: %s address [memDumpPath]" % sys.argv[0])
