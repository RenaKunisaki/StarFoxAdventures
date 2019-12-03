import struct
from util import printf
from structs import ObjectFileStruct, GameObject, HeapStruct, HeapEntry, ModelFileHeader, Model

ROW_COLOR = (19, 17)
fileTable = {
    'AMAP': {
        'bin': (0x32, ),
        'tab': (0x31, ),
    },
    'AMBIENT': {
        'bin': (0x05, ),
        'tab': (0x04, ),
    },
    'ANIM': {
        'bin': (0x30, 0x4A),
        'tab': (0x2F, 0x49),
    },
    'ANIMCURV': {
        'bin': (0x0D, 0x55),
        'tab': (0x0E, 0x56),
    },
    'AUDIO': {
        'bin': (0x01, ),
        'tab': (0x00, ),
    },
    'BITTABLE': {
        'bin': (0x33, ),
        'tab': None,
    },
    'BLOCKS': {
        'bin': (0x25, 0x47),
        'tab': (0x26, 0x48),
    },
    'CACHEFON': {
        'bin': (0x11, 0x12),
        'tab': None,
    },
    'CAMACTIO': {
        'bin': (0x0B, ),
        'tab': None,
    },
    'DLLS': {
        'bin': (0x42, ),
        'tab': (0x43, ),
    },
    'DLLSIMPO': {
        'bin': (0x44, ),
        'tab': None,
    },
    'ENVFXACT': {
        'bin': (0x57, ),
        'tab': None,
    },
    'FONTS': {
        'bin': (0x10, ),
        'tab': None,
    },
    'GAMETEXT': {
        'bin': (0x13, ),
        'tab': (0x14, ),
    },
    'globalma': {
        'bin': (0x15, ),
        'tab': None,
    },
    'HITS': {
        'bin': (0x28, ),
        'tab': (0x29, ),
    },
    'LACTIONS': {
        'bin': (0x0C, ),
        'tab': None,
    },
    'MAPINFO': {
        'bin': (0x1F, ),
        'tab': None,
    },
    'MAPS': {
        'bin': (0x1D, ),
        'tab': (0x1E, ),
    },
    'MODELIND': {
        'bin': (0x2C, ),
        'tab': None,
    },
    'MODELS': {
        'bin': (0x2B, 0x46),
        'tab': (0x2A, 0x45),
    },
    'MODLINES': {
        'bin': (0x37, ),
        'tab': (0x38, ),
    },
    'MPEG': {
        'bin': (0x09, ),
        'tab': (0x08, ),
    },
    'MUSIC': {
        'bin': (0x07, ),
        'tab': (0x06, ),
    },
    'MUSICACT': {
        'bin': (0x0A, ),
        'tab': None,
    },
    'OBJECTS': {
        'bin': (0x3E, ),
        'tab': (0x3D, ),
    },
    'OBJEVENT': {
        'bin': (0x40, ),
        'tab': None,
    },
    'OBJHITS': {
        'bin': (0x41, ),
        'tab': None,
    },
    'OBJINDEX': {
        'bin': (0x3F, ),
        'tab': None,
    },
    'OBJSEQ': {
        'bin': (0x3B, ),
        'tab': (0x3C, ),
    },
    'OBJSEQ2C': {
        'bin': None,
        'tab': (0x0F, ),
    },
    'PREANIM': {
        'bin': (0x51, ),
        'tab': (0x52, ),
    },
    'SAVEGAME': {
        'bin': (0x39, ),
        'tab': (0x3A, ),
    },
    'SCREENS': {
        'bin': (0x18, ),
        'tab': (0x19, ),
    },
    'SFX': {
        'bin': (0x03, ),
        'tab': (0x02, ),
    },
    'TABLES': {
        'bin': (0x16, ),
        'tab': (0x17, ),
    },
    'TEX0': {
        'bin': (0x23, 0x4D),
        'tab': (0x24, 0x4E),
    },
    'TEX1': {
        'bin': (0x20, 0x4B),
        'tab': (0x21, 0x4C),
    },
    'TEXPRE': {
        'bin': (0x4F, ),
        'tab': (0x50, ),
    },
    'TEXTABLE': {
        'bin': (0x22, ),
        'tab': None,
    },
    'TRKBLK': {
        'bin': None,
        'tab': (0x27, ),
    },
    'VOXMAP': {
        'bin': (0x1B, 0x54),
        'tab': (0x1A, 0x53),
    },
    'VOXOBJ': {
        'bin': (0x36, ),
        'tab': (0x35, ),
    },
    'WARPTAB': {
        'bin': (0x1C, ),
        'tab': None,
    },
    'WEAPONDA': {
        'bin': (0x34, ),
        'tab': None,
    },
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
        printf("\x1B[1mObj│Address │Name       │ID  │Def │ModelPtr│XPos     │YPos     │ZPos     │Ch\x1B[0m\n")
        for i in range(cnt):
            pObj = self.client.read(ptr + (i*4), ">I")
            if pObj == 0:
                printf("NULL at object %d\n", i)
                break

            obj = GameObject(self.client, pObj)
            printf("\x1B[48;5;%dm%3d│%08X│%-11s│%04X│%04X│%08X│%+9.2f│%+9.2f│%+9.2f│%2d\x1B[0m\n",
                ROW_COLOR[i&1],
                i, pObj, obj.name,
                obj.objId, obj.defNo, obj.models, obj.pos[0], obj.pos[1], obj.pos[2],
                obj.nChildren)

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
