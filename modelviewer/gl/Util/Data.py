import struct

def dumpHex(data, length=None):
    dump = []
    if length is None: length = len(data)
    for i in range(0, length, 16):
        line = "%04X " % i
        for j in range(16):
            if (j&3) == 0: line += ' '
            line += "%02X " % data[i+j]

        for f in struct.unpack_from('4f', data, i):
            line += '  %8.2f' % f
        dump.append(line)
    return '\n'.join(dump)
