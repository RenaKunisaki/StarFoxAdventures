#!/usr/bin/env python3
"""Read GLOBALMA.bin and generate a map grid image."""
import sys
import math
import struct
from PIL import Image, ImageDraw, ImageFont


def printf(fmt, *args, **kwargs):
    print(fmt % args, end='', **kwargs)

def readStruct(file, fmt, offset=None):
    size = struct.calcsize(fmt)
    if offset is not None: file.seek(offset)
    data = file.read(size)
    r = struct.unpack(fmt, data)
    if len(r) == 1: return r[0] # grumble
    return r

def _mapIdToColor(id):
    n = id + 1
    id = 0
    for i in range(8): id |= ((n >> i) & 1) << i
    r = ((id >> 1) & 3) / 3
    g = ((id >> 3) & 7) / 7
    b = ((id >> 6) & 3) / 3
    return int(r*255), int(g*255), int(b*255)

def _mapIdToHtml(id):
    r, g, b = _mapIdToColor(id)
    return '#%02X%02X%02X' % (r, g, b)


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
        self.maps = self.readMapsBin()

        # plot maps on grid
        for cell in cells:
            if cell.map >= 0 and cell.map < len(self.maps):
                map = self.maps[cell.map]
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
            else:
                printf("Map %r is not in MAPS.bin\n", cell.map)

        self.warps = self.readWarpTab()
        self.mapInfo = self.readMapInfo()
        for i, layer in self.layer.items():
            self.layerToImage(layer, i)
        #    self.printLayer(layer, i)

        #with open('globalmap.html', 'wt') as outFile:
        #    outFile.write('<html><head>')
        #    outFile.write('<link rel="stylesheet" href="globalmap.css" />')
        #    outFile.write('</head><body>')
        #    for i, layer in self.layer.items():
        #        outFile.write('<h1>Layer %d</h1>' % i)
        #        outFile.write(self.layerToHtml(layer, i))
        #    outFile.write('</body></html>')


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
                printf("MAPS.tab %02X = %08X %08X %08X %08X %08X %08X %08X\n",
                    idx, *tabEntries)
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
                        printf("Map %02d blocks = FACEFEED\n", idx)
                        idx += 1
                        continue
                    w, h, x, y = struct.unpack('>4h', data)
                except struct.error as ex:
                    printf("Error reading MAPS.bin entry at 0x%X: %s\n",
                        bin.tell(), ex)
                    break
                entry['x'], entry['y'], entry['w'], entry['h'] = x,y,w,h
                #printf("Map %02X rect is %d, %d, %dx%d\n", idx, x, y, w, h)

                bin.seek(tabEntries[1])
                for i in range(w*h):
                    try:
                        block = readStruct(bin, '>I')
                    except struct.error as ex:
                        printf("Error reading block entry at 0x%X: %s\n",
                            bin.tell(), ex)
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
                    'cx':math.floor(x/640), 'cz':math.floor(z/640),
                    'idx':i, 'layer':ly,
                    'angle':(ang / 128) * 180,
                })
        return entries


    def readMapInfo(self, path="/MAPINFO.bin"):
        maps = []
        with open(self.root+path, 'rb') as file:
            while True:
                try:
                    name, typ, unk2, unk3 = readStruct(file, '>28s2bh')
                except struct.error: break
                maps.append({
                    'name': name.replace(b'\0', b'').decode('utf-8'),
                    'type': typ,
                    'unk2': unk2,
                    'unk3': unk3,
                })
        return maps


    def printLayer(self, layer, id):
        printf("Layer %d is %d, %d - %d, %d\n     ", id,
            layer.xMin, layer.yMin,
            layer.xMax, layer.yMax)
        for x in range(layer.xMin, layer.xMax+1):
            printf("%2d", abs(x))
        printf("\n")
        for y in range(layer.yMin, layer.yMax+1):
            printf("%4d│", y)
            for x in range(layer.xMin, layer.xMax+1):
                cell = layer.get(x, y)
                col  = (x & 1) ^ (y & 1)
                col  = 19 if col == 0 else 17
                printf("\x1B[48;5;%d;38;5;8m", col)
                if cell is None: printf("  ")
                else:
                    map   = cell['map']
                    block = cell['block']
                    if block['mod'] == 0xFF:
                        printf("%02X", map['map'])
                    else: printf("\x1B[38;5;15m%02X", map['map'])
            printf("\x1B[0m\n")


    def layerToImage(self, layer, id):
        scale = 16
        lw    = layer.xMax - layer.xMin
        lh    = layer.yMax - layer.yMin
        w     = lw * scale
        h     = lh * scale
        img   = Image.new('RGBA', (w+1, h+1), (32, 32, 32, 255))
        draw  = ImageDraw.Draw(img)
        maps  = {}
        fnt   = ImageFont.truetype(
            '/usr/share/fonts/TTF/DejaVuSans.ttf', 13)

        # plot the grid
        for y in range(layer.yMin, layer.yMax+1):
            for x in range(layer.xMin, layer.xMax+1):
                px   = (x-layer.xMin) * scale
                py   = (lh-(y-layer.yMin)) * scale
                cell = layer.get(x, y)
                if cell is not None:
                    map   = cell['map']
                    block = cell['block']
                    r, g, b = _mapIdToColor(map['map'])
                    #r, g, b = 192, 192, 192
                    draw.rectangle(
                        (px, py, px+scale, py+scale), # x0 y0 x1 y1
                        fill=(r, g, b, 255),
                        outline=(128, 128, 128, 255))
                    if block['mod'] == 0xFF:
                        draw.rectangle(
                            (px, py, px+scale, py+scale), # x0 y0 x1 y1
                            fill=(r//2, g//2, b//2, 255),
                            outline=(128, 128, 128, 255))
                        draw.line((px, py, px+scale, py+scale),
                            fill=(128, 128, 128, 255), width=1)

                    mapId = map['map']
                    if mapId not in maps:
                        maps[mapId] = {
                            'xMin':x, 'yMin':y, 'xMax':x, 'yMax':y}
                    else:
                        m = maps[mapId]
                        m['xMin'] = min(m['xMin'], x)
                        m['yMin'] = min(m['yMin'], y)
                        m['xMax'] = max(m['xMax'], x)
                        m['yMax'] = max(m['yMax'], y)
                else:
                    draw.rectangle(
                        (px, py, px+scale, py+scale), # x0 y0 x1 y1
                        fill=None,
                        outline=(64, 64, 64, 255))

        # draw warps
        for i, warp in enumerate(self.warps):
            if warp['layer'] == id:
                angle = warp['angle'] - 90
                wx, wy = warp['cx'], warp['cz']
                wx = (wx - layer.xMin) * scale
                wy = (lh-(wy-layer.yMin)) * scale
                draw.pieslice((
                    wx, wy, wx+scale, wy+scale
                ), angle-20, angle+20,
                    fill=(255, 0, 0, 128),
                    outline=(255, 255, 255, 255),
                )
                #draw.text((wx+scale, wy), "%02X" % i,
                #    fill=(255, 255, 255, 255),
                #    font=fnt, stroke_width=1, stroke_fill=(0, 0, 0, 128))



        # put map names over cells
        for mapId, map in maps.items():
            x0, y0 = map['xMin'], map['yMin']-1
            x1, y1 = map['xMax']+1, map['yMax']
            px0 = (x0-layer.xMin) * scale
            py0 = (lh-(y0-layer.yMin)) * scale
            px1 = (x1-layer.xMin) * scale
            py1 = (lh-(y1-layer.yMin)) * scale
            pw  = px1 - px0
            ph  = py1 - py0
            #r, g, b = _mapIdToColor(mapId)
            r, g, b = 192, 192, 192
            draw.rectangle((px0, py0, px1, py1),
                outline=(r, g, b, 255))

            # draw name
            text = "%02X %s" % (mapId, self.mapInfo[mapId]['name'])
            size = draw.textsize(text, font=fnt, stroke_width=1)

            tx = px0 + (pw/2) - (size[0]/2)
            ty = py0 + (ph/2) - (size[1]/2)
            if tx < 0: tx = 0
            # HACK
            if mapId == 0x37: ty += 16
            #if ty < 0: ty = py0 + 2
            #draw.text((tx, ty), text, fill=(r, g, b, 255),
            #    font=fnt, stroke_width=1, stroke_fill=(0, 0, 0, 128))

        img.save("layer%d.png" % id, "PNG")



    def layerToHtml(self, layer, id):
        elems = ['<table><tr><th></th>']
        for x in range(layer.xMin, layer.xMax+1):
            elems.append('<th>%d</th>' % x)
        elems.append('</tr>')

        for y in range(layer.yMax, layer.yMin-1, -1):
            elems.append('<tr><th>%d</th>' % y)
            for x in range(layer.xMin, layer.xMax+1):
                # this is slow but who cares
                warpIdx = -1
                for i, warp in enumerate(self.warps):
                    if (warp['layer'] == id
                    and warp['cx'] == x and warp['cz'] == y):
                        warpIdx = i
                        break

                cell = layer.get(x, y)
                if cell is None:
                    if warpIdx >= 0:
                        title = "%d, %d: no map\nWarp #%02X" % (
                            x, y, warpIdx)
                        elems.append('<td class="empty warp" title="%s">W%02X</td>' % (
                            title, warpIdx))
                    else: elems.append('<td class="empty"></td>')
                else:
                    map   = cell['map']
                    block = cell['block']
                    cls   = 'map%02X' % map['map']
                    style = 'background:'+_mapIdToHtml(map['map'])
                    if block['mod'] == 0xFF:
                        cls += ' oob'
                        title = "%d, %d: out of bounds\n%02X %s" % (
                            x, y, map['map'],
                            self.mapInfo[map['map']]['name'])
                    else:
                        title = "%d, %d (%d, %d): mod%d.%d\n%02X %s" % (
                            x, y, x*640, y*640, block['mod'], block['sub'],
                            map['map'], self.mapInfo[map['map']]['name'])

                    if warpIdx >= 0:
                        cls += ' warp'
                        title += "\nWarp #%02X" % warpIdx
                        warpIdx = 'W%02X' % warpIdx
                    else: warpIdx = ''
                    text = '%02X %s\n%3d.%2d' % (
                        map['map'], warpIdx, block['mod'], block['sub']
                    )

                    if cell['cell'].linked[0] != -1:
                        cls += ' link'
                        links = []
                        link1 = cell['cell'].linked[0]
                        link2 = cell['cell'].linked[1]
                        if link1 >= 0:
                            links.append('%02X %s' % (link1, self.mapInfo[link1]['name']))
                        if link2 >= 0:
                            links.append('%02X %s' % (link2, self.mapInfo[link2]['name']))
                        title += '\nLinked: ' + ', '.join(links)

                    elems.append(
                        '<td class="%s" style="%s" title="%s">%s</td>' % (
                            cls, style, title, text))
            elems.append('</tr>')
        elems.append('</table>')
        return ''.join(elems)


if __name__ == '__main__':
    if len(sys.argv) > 1:
        m = MapReader(sys.argv[1])
        m.run()
    else:
        print("Usage: %s disc-root-path" % sys.argv[0])
