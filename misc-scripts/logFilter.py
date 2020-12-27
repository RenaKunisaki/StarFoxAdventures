#!/usr/bin/env python3
"""Monitor Dolphin log output, filter out repeated and extraneous messages,
and output the result.
"""
import os
import sys
import time
#import xml.etree.ElementTree as ET

# 00:07:880 Core/HLE/HLE_OS.cpp:85 N[OSREPORT]: 8002e030->8002e018| LOADED OBJECT LGTPointLig

prevMsg, prevCount, prevTime = None, 0, None
msgCount = {}
frameMsgCount = 0
tStart = time.time()
MAX_COUNT = 1000

def parseLine(line):
    global prevMsg, prevCount, prevTime, frameMsgCount
    tNow = '%8d' % int((time.time() - tStart) * 1000)
    fields = line.split(maxsplit=4)
    if len(fields) < 5 or 'OSREPORT' not in fields[2]: return
    addr, msg = fields[3], fields[4]
    msg = msg.strip()

    if msg.startswith('>> '): # special case
        # these are printed every frame, so filter down to every second instead
        if msg == '>> START': pass
        elif msg == '>> END': frameMsgCount = (frameMsgCount+1) % 60
        elif frameMsgCount == 0: print("%s %s %s" % (tNow, addr[0:8], msg))
    else:
        if prevMsg == msg:
            prevCount += 1
            return
        else:
            if prevMsg is not None and prevCount > 1:
                print("%s 00000000 Previous message repeated %d times" % (
                    prevTime, prevCount))
            prevMsg, prevTime, prevCount = msg, tNow, 1

        if msg not in msgCount: msgCount[msg] = 0
        msgCount[msg] += 1
        if msgCount[msg] >  MAX_COUNT: return
        print("%s %s %s" % (tNow, addr[0:8], msg))
        if msgCount[msg] == MAX_COUNT:
            print("%s 00000000 Previous message seen %d times, ignoring it from now on" % (
                tNow, MAX_COUNT))


print("       0 00000000 Log starting at " + time.ctime())

# read the lines
for line in sys.stdin:
    if '---- Shutting down ----' in line:
        if prevMsg is not None and prevCount > 1:
            print("%s 00000000 Previous message repeated %d times" % (
                prevTime, prevCount))
        break
    parseLine(line)

print("%8d 00000000 Log ending at %s" % (int(time.time() - tStart) * 1000, time.ctime()))
