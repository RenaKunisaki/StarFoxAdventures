import struct
from util import printf
from structs import ObjectFileStruct, GameObject, HeapStruct, HeapEntry, ModelFileHeader, Model

ROW_COLOR = (19, 17)
fileTable = {
    'AMAP':     { 'bin': (0x32,     ), 'tab': (0x31,     ) },
    'AMBIENT':  { 'bin': (0x05,     ), 'tab': (0x04,     ) },
    'ANIM':     { 'bin': (0x30, 0x4A), 'tab': (0x2F, 0x49) },
    'ANIMCURV': { 'bin': (0x0D, 0x55), 'tab': (0x0E, 0x56) },
    'AUDIO':    { 'bin': (0x01,     ), 'tab': (0x00,     ) },
    'BITTABLE': { 'bin': (0x33,     ), 'tab': None         },
    'BLOCKS':   { 'bin': (0x25, 0x47), 'tab': (0x26, 0x48) },
    'CACHEFON': { 'bin': (0x11, 0x12), 'tab': None         },
    'CAMACTIO': { 'bin': (0x0B,     ), 'tab': None         },
    'DLLS':     { 'bin': (0x42,     ), 'tab': (0x43,     ) },
    'DLLSIMPO': { 'bin': (0x44,     ), 'tab': None         },
    'ENVFXACT': { 'bin': (0x57,     ), 'tab': None         },
    'FONTS':    { 'bin': (0x10,     ), 'tab': None         },
    'GAMETEXT': { 'bin': (0x13,     ), 'tab': (0x14,     ) },
    'globalma': { 'bin': (0x15,     ), 'tab': None         },
    'HITS':     { 'bin': (0x28,     ), 'tab': (0x29,     ) },
    'LACTIONS': { 'bin': (0x0C,     ), 'tab': None         },
    'MAPINFO':  { 'bin': (0x1F,     ), 'tab': None         },
    'MAPS':     { 'bin': (0x1D,     ), 'tab': (0x1E,     ) },
    'MODELIND': { 'bin': (0x2C,     ), 'tab': None         },
    'MODELS':   { 'bin': (0x2B, 0x46), 'tab': (0x2A, 0x45) },
    'MODLINES': { 'bin': (0x37,     ), 'tab': (0x38,     ) },
    'MPEG':     { 'bin': (0x09,     ), 'tab': (0x08,     ) },
    'MUSIC':    { 'bin': (0x07,     ), 'tab': (0x06,     ) },
    'MUSICACT': { 'bin': (0x0A,     ), 'tab': None         },
    'OBJECTS':  { 'bin': (0x3E,     ), 'tab': (0x3D,     ) },
    'OBJEVENT': { 'bin': (0x40,     ), 'tab': None         },
    'OBJHITS':  { 'bin': (0x41,     ), 'tab': None         },
    'OBJINDEX': { 'bin': (0x3F,     ), 'tab': None         },
    'OBJSEQ':   { 'bin': (0x3B,     ), 'tab': (0x3C,     ) },
    'OBJSEQ2C': { 'bin': None,         'tab': (0x0F,     ) },
    'PREANIM':  { 'bin': (0x51,     ), 'tab': (0x52,     ) },
    'SAVEGAME': { 'bin': (0x39,     ), 'tab': (0x3A,     ) },
    'SCREENS':  { 'bin': (0x18,     ), 'tab': (0x19,     ) },
    'SFX':      { 'bin': (0x03,     ), 'tab': (0x02,     ) },
    'TABLES':   { 'bin': (0x16,     ), 'tab': (0x17,     ) },
    'TEX0':     { 'bin': (0x23, 0x4D), 'tab': (0x24, 0x4E) },
    'TEX1':     { 'bin': (0x20, 0x4B), 'tab': (0x21, 0x4C) },
    'TEXPRE':   { 'bin': (0x4F,     ), 'tab': (0x50,     ) },
    'TEXTABLE': { 'bin': (0x22,     ), 'tab': None         },
    'TRKBLK':   { 'bin': None,         'tab': (0x27,     ) },
    'VOXMAP':   { 'bin': (0x1B, 0x54), 'tab': (0x1A, 0x53) },
    'VOXOBJ':   { 'bin': (0x36,     ), 'tab': (0x35,     ) },
    'WARPTAB':  { 'bin': (0x1C,     ), 'tab': None         },
    'WEAPONDA': { 'bin': (0x34,     ), 'tab': None         },
}

mapNames = (
    "animtest",
    "animtest_01",
    "animtest_02",
    "arwing",
    "dragrock",
    "animtest_05",
    "dfptop",
    "volcano",
    "animtest_08",
    "mazecave",
    "dragrockbot",
    "dfalls",
    "swaphol",
    "shipbattle",
    "nwastes",
    "warlock",
    "shop",
    "animtest_11",
    "crfort",
    "swapholbot",
    "wallcity",
    "lightfoot",
    "cloudtreasure",
    "animtest_17",
    "clouddungeon",
    "mmpass",
    "darkicemines",
    "animtest_1B",
    "desert",
    "animtest_1D",
    "icemountain",
    "animtest_1F",
    "animtest_20",
    "animtest_21",
    "darkicemines2",
    "bossgaldon",
    "animtest_24",
    "insidegal",
    "magiccave",
    "dfshrine",
    "mmshrine",
    "ecshrine",
    "gpshrine",
    "dbshrine",
    "nwshrine",
    "worldmap",
    "animtest_2E",
    "capeclaw",
    "dbay",
    "animtest_31",
    "cloudrace",
    "bossdrakor",
    "animtest_34",
    "bosstrex",
    "linkb",
    "cloudjoin",
    "arwingtoplanet",
    "arwingdarkice",
    "arwingcloud",
    "arwingcity",
    "arwingdragon",
    "gamefront",
    "linklevel",
    "greatfox",
    "linka",
    "linkc",
    "linkd",
    "linke",
    "linkf",
    "linkg",
    "linkh",
    "linkj",
    "linki",
)

class Game:
    """Connection to SFA."""
    def __init__(self, client):
        self.client = client
        self._bitTable = None

    def showObject(self, addr):
        obj = GameObject(self.client, addr)
        obj.printSelf()

    def readObject(self, addr):
        return GameObject(self.client, addr)

    def listLoadedObjects(self):
        # read ptr to object list
        cnt, ptr = self.client.read(0x803dcb84, ">II")
        if ptr == 0:
            print("objPtr is NULL")
            return
        printf("objPtr = %08X nObjs = %d\n", ptr, cnt)
        printf("\x1B[1mObj│Address │Name       │ID  │Def │ModelPtr│XPos     │YPos     │ZPos     │Ch│Seq\x1B[0m\n")
        for i in range(cnt):
            pObj = self.client.read(ptr + (i*4), ">I")
            if pObj == 0:
                printf("NULL at object %d\n", i)
                break

            obj = GameObject(self.client, pObj)
            printf("\x1B[48;5;%dm%3d│%08X│%-11s│%04X│%04X│%08X│%+9.2f│%+9.2f│%+9.2f│%2d│%08X %d\x1B[0m\n",
                ROW_COLOR[i&1],
                i, pObj, obj.name,
                obj.objId, obj.defNo, obj.models, obj.pos[0], obj.pos[1], obj.pos[2],
                obj.nChildren, obj.seq, obj.curSeq)

    def listObjModels(self, addr):
        obj = self.readObject(addr)
        pModels = self.client.read(obj.models, ">%dI" % obj.file.nModels)
        for i in range(obj.file.nModels):
            model = Model(self.client, pModels[i])
            printf("\nModel %d @ 0x%08X:\n", i, pModels[i])
            model.printSelf()

    def showHeap(self):
        printf("\x1B[1m#│Data    │Size    │Used    │Free    │Slots   │UsedSlot│FreeSlot\x1B[0m\n")
        heaps = []
        for i in range(5):
            hs = HeapStruct(self.client, 0x803406A0 + (i*HeapStruct.SIZE))
            heaps.append(hs)
            printf("\x1B[48;5;%dm%d│%08X│%08X│%08X│%08X│%08X│%08X│%08X\x1B[0m\n",
                ROW_COLOR[i&1], i,
                hs.data, hs.size, hs.usedSize, hs.size-hs.usedSize,
                hs.avail, hs.used, hs.avail-hs.used)

        for i, heap in enumerate(heaps):
            if heap.data != 0:
                printf("\n\x1B[1mHeap %d:\x1B[0m\n", i)
                printf("\x1B[1mIdx │Address │Size    │Unk8│Idx │Stack   │Unk14   │Unk18   │Tag\x1B[0m\n")
                for j in range(heap.used):
                    entry = HeapEntry(self.client, heap.data + (j * HeapEntry.SIZE))
                    printf("\x1B[48;5;%dm%4d│%08X│%08X│%04X│%04X│%04X%04X│%08X│%08X│%08X %s\x1B[0m\n",
                    ROW_COLOR[j&1], j,
                    entry.loc, entry.size, entry.unk08, entry.idx,
                    entry.stack[0], entry.stack[1], entry.unk14, entry.unk18,
                    entry.tag, HeapEntry._tags.get(entry.tag, "<unknown>"))

            else:
                printf("\x1B[1mHeap %d: \x1B[0mnull\n", i)

    def heapStats(self):
        printf("\x1B[1m#│Data    │Size    │End     │Used    │Free    │Slots   │UsedSlot│FreeSlot│FreeB│FreeS\x1B[0m\n")
        heaps = []
        for i in range(5):
            hs = HeapStruct(self.client, 0x803406A0 + (i*HeapStruct.SIZE))
            heaps.append(hs)
        for i, hs in enumerate(heaps):
            printf("\x1B[48;5;%dm%d│%08X│%08X│%08X│%08X│%08X│%08X│%08X│%08X│%4d%%│%4d%%\x1B[0m\n",
                ROW_COLOR[i&1], i,
                hs.data, hs.size, hs.data + hs.size,
                hs.usedSize, hs.size-hs.usedSize,
                hs.avail, hs.used, hs.avail-hs.used,
                (1-(hs.usedSize/hs.size))*100 if hs.size != 0 else 0,
                (1-(hs.used/hs.avail))*100 if hs.avail != 0 else 0)


    def listFiles(self):
        filePtrs  = self.client.read(0x8035f3e8, '>88I')
        fileSizes = self.client.read(0x8035f0a8, '>88I')
        mapIds    = self.client.read(0x8035f548, '>88H')

        printf("\x1B[1m        " +
            "║Slot 1 .bin                        "+
            "║Slot 1 .tab                        "+
            "║Slot 2 .bin                        "+
            "║Slot 2 .tab\nFileName" +
            "║ RamAddr│FileSize│ Map             "+
            "║ RamAddr│FileSize│ Map             "+
            "║ RamAddr│FileSize│ Map             "+
            "║ RamAddr│FileSize│ Map\x1B[0m\n")

        rowIdx = 0
        for name, file in fileTable.items():
            bin, tab = file['bin'], file['tab']
            if bin is None: bin = ()
            if tab is None: tab = ()
            printf("\x1B[48;5;%dm%-8s║", ROW_COLOR[rowIdx&1], name)
            for i in range(2):
                binIdx = bin[i] if i < len(bin) else None
                tabIdx = tab[i] if i < len(tab) else None

                if binIdx is not None:
                    mapId = mapIds[binIdx]
                    mapName = mapNames[mapId] if mapId < len(mapNames) else "-"
                    printf("\x1B[38;5;%dm%8X│%8X│%04X %-12s\x1B[38;5;7m║",
                        8 if filePtrs[binIdx] == 0 else 7,
                        filePtrs[binIdx], fileSizes[binIdx],
                         mapId, mapName)
                else:
                    printf("--------│--------│---- ------------║")

                if tabIdx is not None:
                    mapId = mapIds[tabIdx]
                    mapName = mapNames[mapId] if mapId < len(mapNames) else "-"
                    printf("\x1B[38;5;%dm%8X│%8X│%04X %-12s\x1B[38;5;7m║",
                        8 if filePtrs[tabIdx] == 0 else 7,
                        filePtrs[tabIdx], fileSizes[tabIdx],
                        mapId, mapName)
                else:
                    printf("--------│--------│---- ------------║")

            printf("\x1B[0m\n")
            rowIdx += 1


    def getCurMap(self):
        name, typ, field_1d, field_1e = self.client.read(0x816a8ba0, ">28s2bh")
        id = self.client.read(0x8035f592, ">h")
        try: name = name.decode('shift-jis')
        except UnicodeDecodeError: name = '???'
        lol = name.find('\0')
        if lol >= 0: name = name[0:lol]
        return {
            'name': name,
            'id':   id,
            'type': typ,
            'field_1d': field_1d,
            'field_1e': field_1e,
        }

    def warpToMap(self, mapId, charId=None):
        if charId is not None:
            self.client.write(0x803a32c8, struct.pack('b', charId))
        # XXX what is second param?
        self.client.callFunc(0x800552e8, mapId, 0)


    def getDebugLog(self):
        bufEnd = self.client.read(0x803dbc14, '>I')
        size   = bufEnd - 0x803aa018
        if size <= 0: return ''
        data   = self.client.read(0x803aa018, size)

        # parse control codes
        res = []
        i = 0
        while i < len(data):
            b = data[i]
            i += 1
            if b == 0x81: # set color
                r, g, b, a = data[i:i+4]
                i += 4
                res.append("\x1B[38;2;%d;%d;%dm" % (r, g, b))
            elif b == 0x82: # set position
                y, x = struct.unpack_from('>hh', data, i)
                i += 4
                res.append("\x1B[%d;%dH" % (y, x))
            elif b == 0x83 or b == 0x84:
                res.append("<UNK %02X>" % b)
            elif b == 0x85: # set BG color?
                r, g, b, a = data[i:i+4]
                i += 4
                res.append("\x1B[48;2;%d;%d;%dm" % (r, g, b))
            elif b == 0x86 or b == 0x87:
                x, y = data[i:i+2]
                i += 2
                res.append("<UNK %02X %02X%02X>" % (b, x, y))
            else:
                res.append(chr(b))
        return ''.join(res)

    def _readBitTable(self):
        if self._bitTable is None:
            # read BITTABLE.BIN from memory
            ptr = self.client.read(0x8035f3e8 + (0x33 * 4), '>I')
            data = self.client.read(ptr, '>7840I')
            self._bitTable = []
            for val in data:
                self._bitTable.append({
                    'offset':   val >> 16, # bit offset into table
                    'tblIdx':  (val >> 14) & 3, # which table
                    'unk13':   (val >> 13) & 1, # unk03 is valid
                    'nBits':  ((val >>  8) & 0x1F)+1,
                    'unk03':    val        & 0xFF, # a task text index?
                })

    def _calcGameBitAddr(self, bit):
        self._readBitTable()

        # get the address of the table
        entry = self._bitTable[bit]
        tblIdx = entry['tblIdx']
        if tblIdx == 0: pTable = 0x803a4198 # temp bits
        else:
            pSaveGame = self.client.read(0x803dcae0, '>I')
            if pSaveGame == 0:
                return entry, None
            #printf("saveGame = 0x%08X\n", pSaveGame)
            offsets = (0x564, 0x24, 0x5D8)
            pTable = pSaveGame + offsets[tblIdx-1]

        addr = pTable #+ (entry['offset'] >> 3)
        #printf("Tbl #%d @0x%08X bit #0x%04X -> byte 0x%02X bit %d: 0x%08X, unk 0x%02X %d\n",
        #    tblIdx, pTable, entry['offset'], entry['offset'] >> 3,
        #    entry['offset'] & 7, addr,
        #    entry['unk03'], entry['unused'])

        return entry, addr

    def getGameBit(self, bit):
        entry, addr = self._calcGameBitAddr(bit)
        if addr is None: return "<no save game loaded>"
        nBits    = entry['nBits']
        firstBit = entry['offset']
        lastBit  = firstBit + nBits
        val      = 0
        idx      = 0
        for i in range(firstBit, lastBit):
            byte = self.client.read(addr + (i >> 3), 'B')
            if byte & (1 << (i & 7)): val |= 1 << idx
            idx += 1
        #printf("bits %d - %d val 0x%X\n", firstBit, lastBit, val)
        return val

    def setGameBit(self, bit, val):
        entry, addr = self._calcGameBitAddr(bit)
        if addr is None:
            printf("Can't set bit 0x%X, no save game loaded\n", bit)
            return

        nBits    = entry['nBits']
        firstBit = entry['offset']
        lastBit  = firstBit + nBits
        idx      = 0

        maxVal = (1 << nBits) - 1
        if val < 0: val += maxVal
        if val < 0 or val > maxVal:
            printf("Value out of range (0 to %d, got %d)\n", maxVal, val)
            return

        for i in range(firstBit, lastBit):
            byte = self.client.read(addr + (i >> 3), 'B')
            oldB = byte
            mask = (1 << (i & 7))
            b    = val & (1 << idx)
            if b: byte |= mask
            else: byte &= ~mask
            #printf("write 0x%02X -> 0x%02X at 0x%08X, idx %d val %d\n",
            #    oldB, byte, addr + (i >> 3), idx, 1 if b else 0)
            self.client.write(addr + (i >> 3), bytes([byte]))
            idx += 1
            if idx == 8: idx = 0

    def dumpGameBits(self):
        self._readBitTable()
        pSaveGame = self.client.read(0x803dcae0, '>I')
        tables = (
            0x803a4198, # temp bits
            pSaveGame+0x564, pSaveGame+0x24, pSaveGame+0x5D8,
        )
        sizes = (0x80, 0x74, 0x144, 0xAC)

        tableData = []
        for i, addr in enumerate(tables):
            if addr >= 0x80000000:
                tableData.append(self.client.read(addr, sizes[i]))
            else:
                tableData.append(b'\0' * sizes[i])

        printf("\x1B[1mBit#│T│Unk │MaxValue  │Value\x1B[0m\n")
        for iBit in range(0x1000):
            entry = self._bitTable[iBit]
            tblIdx = entry['tblIdx']
            tData  = tableData[tblIdx]

            nBits    = entry['nBits']
            firstBit = entry['offset']
            lastBit  = firstBit + nBits
            maxVal   = (1 << nBits) - 1
            val      = 0
            idx      = 0
            for i in range(firstBit, lastBit):
                n = i >> 3
                b = tData[n] if n < len(tData) else 0
                if b & (1 << (i & 7)): val |= 1 << idx
                idx += 1
            printf("\x1B[48;5;%dm%04X│%d│%02X %d│%10d│%s\x1B[0m\n",
                ROW_COLOR[iBit & 1], iBit, tblIdx,
                entry['unk03'], entry['unk13'],
                maxVal, val)
