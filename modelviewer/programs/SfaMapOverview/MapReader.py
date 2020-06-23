import logging; log = logging.getLogger(__name__)
import struct

def readStruct(file, fmt, offset=None):
    size = struct.calcsize(fmt)
    if offset is not None: file.seek(offset)
    data = file.read(size)
    r = struct.unpack(fmt, data)
    if len(r) == 1: return r[0] # grumble
    return r


class MapGrid:
    def __init__(self, width, height, originX=0, originY=0):
        self.grid = []
        self.width, self.height = width, height
        self.originX, self.originY = originX, originY
        self.xMin, self.yMin = 999999, 999999
        self.xMax, self.yMax = -self.xMin, -self.yMin
        for y in range(self.height - self.originY):
            for x in range(self.width):
                self.grid.append(None)

    def _checkCoords(self, x, y):
        idx = ((y - self.originY) * self.width) + (x - self.originX)
        if idx < 0 or idx >= len(self.grid):
            raise KeyError("Coords %d, %d out of range for grid %d,%d - %d,%d" % (
                x, y,
                self.originX, self.originY,
                self.width  - self.originX,
                self.height - self.originY))
        return idx

    def set(self, x, y, val):
        idx = self._checkCoords(x, y)
        self.grid[idx] = val
        self.xMin = min(x, self.xMin)
        self.yMin = min(y, self.yMin)
        self.xMax = max(x, self.xMax)
        self.yMax = max(y, self.yMax)

    def get(self, x, y):
        idx = self._checkCoords(x, y)
        return self.grid[idx]


class MapGridCell:
    def __init__(self, x, y, layer, map, link1, link2):
        self.x, self.y = x, y
        self.layer, self.map = layer, map
        self.linked = [link1, link2]

    @classmethod
    def read(cls, file):
        x, y, ly, map, l1, l2 = readStruct(file, '>6h')
        if map < 0: return None
        return MapGridCell(x, y, ly, map, l1, l2)


class MapReader:
    MIN_LAYER = -2
    MAX_LAYER =  2

    def __init__(self, discroot:str):
        self.root = discroot
        self.layer = {}
        for i in range(self.MIN_LAYER, self.MAX_LAYER+1):
            self.layer[i] = MapGrid(1024, 1024, -512, -512)


    def run(self):
        cells = self.readGlobalMap()
        maps  = self.readMapsBin()
        for cell in cells:
            if cell.map >= 0 and cell.map < len(maps):
                map = maps[cell.map]
                for y in range(map['h']):
                    for x in range(map['w']):
                        gx = (x + cell.x) - map['x']
                        gy = (y + cell.y) - map['y']
                        bi = (y * map['w']) + x
                        block = map['blocks'][bi]
                        self.layer[cell.layer].set(gx, gy, {
                            'map':   map,
                            'block': block,
                            'cell':  cell,
                        })
        self.warps = self.readWarpTab()

    def readGlobalMap(self, path='/globalma.bin'):
        cells = []
        with open(self.root+path, 'rb') as globalMa:
            while True:
                cell = MapGridCell.read(globalMa)
                if cell is None: break
                cells.append(cell)
        return cells


    def readMapsBin(self, path='/'):
        entries = []
        try:
            bin = open(self.root+path+'MAPS.bin', 'rb')
            tab = open(self.root+path+'MAPS.tab', 'rb')
            idx = 0
            while True:
                try:
                    tabEntries = readStruct(tab, '>7i')
                except struct.error as ex:
                    # we should be checking for an entry of 0xFFFFFFFF
                    # but that requires reading them one by one
                    break
                if tabEntries[0] < 0: break
                entry = {
                    'tab': tabEntries,
                    'map': idx,
                    'blocks': [],
                }
                entries.append(entry)
                try:
                    bin.seek(tabEntries[0])
                    data = bin.read(8)
                    if data.startswith(b'\xFA\xCE\xFE\xED'):
                        #printf("Map %02d blocks = FACEFEED\n", idx)
                        idx += 1
                        continue
                    w, h, x, y = struct.unpack('>4h', data)
                except struct.error as ex:
                    break
                entry['x'], entry['y'], entry['w'], entry['h'] = x,y,w,h

                bin.seek(tabEntries[1])
                for i in range(w*h):
                    try:
                        block = readStruct(bin, '>I')
                    except struct.error as ex:
                        break
                    unk1  = block >> 31
                    mod   = (block >> 23) & 0xFF
                    sub   = (block >> 17) & 0x3F
                    unk2  = block & 0x1FF
                    entry['blocks'].append({
                        'mod': mod,
                        'sub': sub,
                        'unk': (unk1, unk2)
                    })

                idx += 1
            return entries
        finally:
            bin.close()
            tab.close()


    def readWarpTab(self, path="/WARPTAB.bin"):
        entries = []
        with open(self.root+path, 'rb') as file:
            i = 0
            while True:
                try:
                    x, y, z, ly, ang = readStruct(file, '>3f2h')
                except struct.error: break
                    # there's no end-of-file marker here...
                # for some reason angle is only the high byte of an s16,
                # even though it's stored as a full s16 itself.
                # ie it ranges from -128 to 127.
                entries.append({
                    'x':x, 'y':y, 'z':z,
                    'idx':i, 'layer':ly,
                    'angle':(ang / 128) * 180,
                })
        return entries
