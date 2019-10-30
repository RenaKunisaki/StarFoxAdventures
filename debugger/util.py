def printf(fmt, *args):
    print(fmt % args, end='')

def hexdump(data, base=0):
    print("Offset           +0 +1 +2 +3  +4 +5 +6 +7  +8 +9 +A +B  +C +D +E +F")
    for offs in range(0, len(data), 16):
        line = ["%08X [%04X]  " % (offs+base, offs)]
        for c in range(16):
            try: b = data[offs+c]
            except IndexError: break
            line.append('%02X ' % b)
            if c & 3 == 3: line.append(' ')
        print(''.join(line))
