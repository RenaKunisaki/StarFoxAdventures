from binaryfile import BinaryFile

class TabFile:
    def __init__(self, file:(BinaryFile, str)):
        if type(file) is str: file = BinaryFile(file)
        self.file = file

    def getEntries(self):
        result = []
        for i in range(0, self.file.size, 4):
            entry = self.file.readu32()
            flags = entry >> 24
            result.append({
                'compressed': (flags & 0x80) != 0,
                'flags':       flags & 0x7F,
                'offset':     entry & 0xFFFFFF,
            })
        return result
