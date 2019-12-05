#!/usr/bin/env python3
"""Displays a "disassembly" of Gecko codes."""
import sys
import math

def printf(fmt, *args):
    print(fmt % args, end='')


def hex2bytes(line:str):
    c = line.find('#')
    if c >= 0: line = line[0:c] # strip comments
    return bytes.fromhex(line.replace(' ', ''))


def bytes2word(val:(bytes, list)):
    return (int(val[0]) << 24) | \
        (int(val[1]) << 16) | \
        (int(val[2]) << 8) | int(val[3])


lines = []
curOffset = 0
curIfCount = 0

def nextLine():
    while True:
        assert len(lines) > 0, "missing next line"
        line = hex2bytes(lines.pop(0).strip())
        if len(line) > 0: return line


def disasmType0(codeType:int, code):
    addr     = (bytes2word(code) & 0x01FFFFFF) + 0x80000000
    val      = bytes2word(code[4:])
    count    = ((code[4] << 16) | code[5]) + 1
    subType  = (code[0] & 0x07) >> 1
    poFlag   = code[0] & 0x10
    P = 'ba' if poFlag == 0 else 'po'

    if subType in (0, 1, 2): # write value
        if   subType == 0: size = 'u8'
        elif subType == 1: size = 'u16'
        else: size = 'u32'
        printf("*(%s*)(%s+", size, P)
        if subType in (0, 1):
            printf("(0x%08X..0x%08X)", addr, addr + count)
        else: printf("0x%08X", addr)

        if   subType == 0: printf(") = 0x%02X\n", val & 0xFF)
        elif subType == 1: printf(") = 0x%04X\n", val & 0xFFFF)
        else: printf(") = 0x%08X\n", val)

    elif subType == 3: # write string
        printf("*(%s+(0x%08X..0x%08X)) =\n", P, addr, addr+val)
        while val > 0:
            line = nextLine()
            printf("  0x")
            for i in range(min(8, val)):
                printf("%02X", line[i])
                if (i & 3) == 3: printf(", 0x")
            printf("\n")
            val -= 8

    elif subType == 4: # write serial
        line    = nextLine()
        size    = line[0] >> 4
        count   = (((line[0] & 0xF) << 8) | line[1]) + 1
        addrInc = (line[2] << 8) | line[3]
        valInc  = bytes2word(line[4:])

        printf("%s+(0x%08X..0x%08X) = ", P, addr, addr+(count*addrInc))
        if   size == 0: printf("0x%02X", val & 0xFF)
        elif size == 1: printf("0x%04X", val & 0xFFFF)
        elif size == 2: printf("0x%08X", val)
        else: printf("INVALID SIZE")
        printf("; addr skip=0x%X, val skip=0x%X\n", addrInc, valInc)

    else:
        printf("unknown code type\n")


def disasmType2(codeType:int, code):
    global curIfCount
    addr     = (bytes2word(code) & 0x01FFFFFF) + 0x80000000
    isEndif  = (addr & 1) != 0
    poFlag   = code[0] & 0x10
    val      = bytes2word(code[4:])
    conds    = ('==', '!=', '> ', '< ')
    cond     = conds[(codeType & 7) >> 1]

    if codeType in (0x20, 0x22, 0x24, 0x26): # 32 bit
        addr &= 0xFFFFFFFC
        val = '0x%08X' % val
    elif codeType in (0x28, 0x2A, 0x2C, 0x2E): # 16 bit
        addr &= 0xFFFFFFFE
        val = '0x%04X' % val

    printf("%sIf *(%s+0x%08X) %s %s\n",
        'Else' if isEndif else '',
        'ba' if poFlag == 0 else 'po',
        addr, cond, val)
    if not isEndif: curIfCount += 1


def disasmType4(codeType:int, code):
    poFlag   = code[0] & 0x10
    subType  = (code[0] & 0x07) >> 1
    dest     = 'ba' if (code[0] & 0x08) == 0 else 'po'
    op       = (code[1] >> 4) & 1
    op       = ' =' if op == 0 else '+='
    useBa    = (code[1] & 0x01) != 0
    useGr    = (code[2] & 0x10) != 0
    gr       = code[3] & 0xF
    addr     = '0x%08X' % bytes2word(code[4:])

    if useBa: addr += '+ba' if poFlag == 0 else '+po'
    if useGr: addr += '+gr%X' % gr

    if   subType == 0: printf("%s %s *(%s)\n", dest, op, addr)
    elif subType == 1: printf("%s %s %s\n", dest, op, addr)
    elif subType == 2: printf("*(%s) %s %s\n", addr, op, dest)
    elif subType == 3:
        offs = (code[2] << 8) | code[3]
        if offs >= 0x8000: offs = '-0x%04X' % (0x10000 - offs)
        else: offs = '+0x%04X' % offs
        printf("%s = &nextCode + %s\n", dest, offs)
    else:
        printf("Unknown code type\n")


def disasmType6(codeType:int, code):
    subType = (code[0] & 0x07) >> 1
    cond    = code[1] >> 4
    count   = (code[2] << 8) | code[3]
    P       = code[7] & 0xF

    if   cond == 0: cond = 'if(Exec==True) '
    elif cond == 1: cond = 'if(Exec==False) '
    elif cond == 2: cond = ''
    else: cond = 'INVALID CONDITION '

    if   subType == 0: printf("Set Repeat[0x%X] = 0x%04X\n", P, count)
    elif subType == 1: printf("Do  Repeat[0x%X]\n", P)
    elif subType == 2: printf("%sReturn[0x%X]\n", cond, P)
    elif subType == 3: printf("%sGoto[0x%X]\n", cond, P)
    elif subType == 4: printf("%sGosub[0x%X]\n", cond, P)
    else: printf("Unknown code type\n")


def disasmType8(codeType:int, code):
    subType = (code[0] & 0x07) >> 1
    poFlag  =  code[0] & 0x10
    useBa   = (code[1] & 0x01) != 0
    gr      =  code[3] & 0x0F
    addr    = '0x%08X' % bytes2word(code[4:])

    size = code[1] >> 4
    if   size == 0: size = "u8"
    elif size == 1: size = "u16"
    elif size == 2: size = "u32"
    else: size = "???"

    P = 'ba' if poFlag == 0 else 'po'
    if useBa: addr += '+' + P

    if subType == 0:
        op = ' =' if (code[1] & 0x10) == 0 else '+='
        printf("gr%X %s %s\n", gr, op, addr)

    elif subType == 1:
        printf("gr%X = *(%s*)(%s)\n", gr, size, addr)

    elif subType == 2:
        count = ((code[2] << 4) | (code[3] >> 4)) + 1
        printf("*(%s*)%s = gr%X (repeat 0x%X)\n", size, addr, gr, count)

    elif subType == 3 or subType == 4:
        ops = ('+', '*', '|', '&', '^', '<<', '>>',
            'ROL', 'ASR', 'FADDS', 'FMULS',
            '???', '???', '???', '???', '???')

        if subType == 4: addr = 'gr%F' % (code[7] & 0xF)

        if code[1] & 0x01: gr = '*gr%X' % gr
        else: gr = 'gr%X' % gr
        if code[1] & 0x02: addr = '*(%s)' % addr
        op = ops[code[1] >> 4]

        printf("%s = (%s %s %s)\n", gr, gr, op, addr)

    elif subType == 5 or subType == 6:
        count = (code[1] << 8) | code[2]
        grN   = code[3] >> 4
        grK   = code[3] & 0xF
        addr  = '0x%08X' % bytes2word(code[4:])
        grN   = P if grN == 0xF else ('*gr%X' % grN)
        grK   = P if grK == 0xF else ('*gr%X' % grK)
        printf("copy 0x%04X bytes from ", count)
        if subType == 5: printf("%s+%s to %s\n", grN, addr, grK)
        else:            printf("%s to %s+%s\n", grN, grK, addr)

    else: printf("Unknown code type")


def disasmTypeA(codeType:int, code):
    global curIfCount
    subType = (code[0] & 0x07) >> 1
    poFlag  = code[0] & 0x10

    addr      = (bytes2word(code) & 0x01FFFFFC) + 0x80000000
    isEndif   = (code[3] & 1) != 0
    isCounter = (codeType & 0x08) != 0
    poFlag    = code[0] & 0x10
    conds     = ('==', '!=', '> ', '< ')
    cond      = conds[(codeType & 7) >> 1]
    grK, grN  = code[4] >> 4, code[4] & 0xF
    grN       = ('%08X+%s' % (addr, P)) if grN == 0xF else ('gr%X' % grN)
    grK       = ('%08X+%s' % (addr, P)) if grK == 0xF else ('gr%X' % grK)

    if isCounter:
        mask  = (~((code[4] << 8) | code[5])) & 0xFFFF
        check = (code[6] << 8) | code[7]
        cnt   = ((code[1] & 0xF) << 12) | (code[2] << 4) | (code[3] >> 4)
        reset = (code[3] & 0x08)
        printf("%sIf16 (0x%04X & 0x%04X) %s Count(0x%04X)",
            'Else' if isEndif else '', check, mask, cond, cnt)
        if reset: printf(" and reset count")
        printf("\n")

    else:
        mask = (~((code[6] << 8) | code[7])) & 0xFFFF
        printf("%sIf16 (*(%s) & 0x%04X) %s (*(%s) & 0x%04X)\n",
            'Else' if isEndif else '',
            grN, mask, cond, grK, mask)

    if not isEndif: curIfCount += 1


def disasmTypeC(codeType:int, code):
    global curIfCount
    subType = (code[0] & 0x07) >> 1
    addr    = (bytes2word(code) & 0x01FFFFFC) + 0x80000000
    val     = bytes2word(code[4:])
    poFlag  = code[0] & 0x10
    P       = 'ba' if poFlag == 0 else 'po'

    if subType == 0:
        printf("Exec ASM (0x%08X lines):\n", val)
        for i in range(val):
            printf("  0x%02X%02X%02X%02X, 0x%02X%02X%02X%02X,\n",
                *nextLine())

    elif subType == 1:
        printf("Insert ASM (0x%08X lines) @0x%08X+%s:\n", val, addr, P)
        for i in range(val):
            printf("  0x%02X%02X%02X%02X, 0x%02X%02X%02X%02X,\n",
                *nextLine())

    # subtype 2 doesn't exist

    elif subType == 3:
        printf("Insert branch to 0x%08X at 0x%08X+%s\n",
            val, addr, P)

    # subtype 4, 5 don't exist

    elif subType == 6: printf("Toggle Code Exec\n")
    elif subType == 7:
        isEndif = (code[3] & 1) != 0
        vMin    = (code[4] << 8) | code[5]
        vMax    = (code[6] << 8) | code[7]
        printf("%sIf %s >= 0x%04X0000 && %s < 0x%04X0000\n",
            'Else' if isEndif else '', P, vMin, P, vMax)
        if not isEndif: curIfCount += 1


def disasmTypeE(codeType:int, code):
    global curIfCount
    subType = (code[0] & 0x07) >> 1
    ba, po = (code[4] << 8) | code[5], (code[6] << 8) | code[7]

    if subType == 0 or subType == 1:
        if subType == 0:
            printf("End")
            curIfCount = 0
        else:
            curIfCount -= code[3] # XXX -1? does this count as an If?
            printf("EndIf(x%d)", code[3])
            if code[1] & 0x10:
                printf("; Else")
                curIfCount += 1

        if ba: printf("; ba=0x%04X0000", ba)
        if po: printf("; po=0x%04X0000", po)
        printf("\n")

    # following checks against codeType, not subType.
    elif codeType == 0xF0:
        printf("End of Codes\n")
        curIfCount = 0

    elif codeType == 0xF2 or codeType == 0xF4:
        addr    = (bytes2word(code) & 0x01FFFFFC) + 0x80000000
        poFlag  = code[0] & 0x04
        P       = 'ba' if poFlag == 0 else 'po'
        count  = code[4]
        cksum  = (code[5] << 8) | code[6]
        nLines = code[7]
        if count >= 0x80:
            addrEnd = addr
            addr = addr - ((0x100 - count)*2)
        else: addrEnd = addr + (count*2)

        printf("Exec ASM (0x%02X lines) if XOR of bytes %s+(0x%08X..0x%08X) == 0x%04X\n",
            nLines, P, addr, addrEnd, cksum)
        for i in range(nLines):
            printf("  0x%02X%02X%02X%02X, 0x%02X%02X%02X%02X,\n",
                *nextLine())

    elif codeType == 0xF6:
        nLines = code[3]
        vMin, vMax = (code[4] << 8) | code[5], (code[6] << 8) | code[7]
        if vMin == 0: vMin = 0x80003000
        else: vMin = vMin << 16
        vMax <<= 16

        printf("If values found in memory 0x%08X..0x%08X:\n", vMin, vMax)
        for i in range(nLines):
            printf("  0x%02X%02X%02X%02X, 0x%02X%02X%02X%02X,\n",
                *nextLine())
        curIfCount += 1

    else:
        printf("Unknown code type\n")



def disasm(code:bytes):
    global curIfCount, curOffset
    codeType = int(code[0]) & 0xEE

    if curIfCount < 0:
        printf("WARNING: curIfCount == %d\n", curIfCount)
        curIfCount = 0
    printf("[%08X] %02X%02X%02X%02X %02X%02X%02X%02X: ",
        curOffset, *code)
    printf("  " * curIfCount)

    tp = codeType >> 4
    if   tp in (0x00, 0x01): disasmType0(codeType, code)
    elif tp in (0x02, 0x03): disasmType2(codeType, code)
    elif tp in (0x04, 0x05): disasmType4(codeType, code)
    elif tp in (0x06, 0x07): disasmType6(codeType, code)
    elif tp in (0x08, 0x09): disasmType8(codeType, code)
    elif tp in (0x0A, 0x0B): disasmTypeA(codeType, code)
    elif tp in (0x0C, 0x0D): disasmTypeC(codeType, code)
    elif tp in (0x0E, 0x0F): disasmTypeE(codeType, code)
    else: printf("unknown code type\n")
    curOffset += 8


if len(sys.argv) > 1:
    inFile = open(sys.argv[1], 'rt')
else:
    inFile = sys.stdin

for line in inFile:
    lines.append(line)

while len(lines) > 0:
    disasm(nextLine())

if curIfCount != 0:
    printf("WARNING: ending with curIfCount == %d\n", curIfCount)
