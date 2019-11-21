import struct
from util import printf
from structs import ObjectFileStruct, GameObject, HeapStruct, HeapEntry, ModelFileHeader, Model

ROW_COLOR = (19, 17)
fileNames = (
    "AUDIO.tab",
    "AUDIO.bin",
    "SFX.tab",
    "SFX.bin",
    "AMBIENT.tab",
    "AMBIENT.bin",
    "MUSIC.tab",
    "MUSIC.bin",
    "MPEG.tab",
    "MPEG.bin",
    "MUSICACT.bin",
    "CAMACTIO.bin",
    "LACTIONS.bin",
    "ANIMCURV.bin",
    "ANIMCURV.tab",
    "OBJSEQ2C.tab",
    "FONTS.bin",
    "CACHEFON.bin",
    "CACHEFON.bin_12",
    "GAMETEXT.bin",
    "GAMETEXT.tab",
    "globalma.bin",
    "TABLES.bin",
    "TABLES.tab",
    "SCREENS.bin",
    "SCREENS.tab",
    "VOXMAP.tab",
    "VOXMAP.bin",
    "WARPTAB.bin",
    "MAPS.bin",
    "MAPS.tab",
    "MAPINFO.bin",
    "TEX1.bin",
    "TEX1.tab",
    "TEXTABLE.bin",
    "TEX0.bin",
    "TEX0.tab",
    "BLOCKS.bin",
    "BLOCKS.tab",
    "TRKBLK.tab",
    "HITS.bin",
    "HITS.tab",
    "MODELS.tab",
    "MODELS.bin",
    "MODELIND.bin",
    "MODANIM.TAB",
    "MODANIM.BIN",
    "ANIM.TAB",
    "ANIM.BIN",
    "AMAP.TAB",
    "AMAP.BIN",
    "BITTABLE.bin",
    "WEAPONDA.bin",
    "VOXOBJ.tab",
    "VOXOBJ.bin",
    "MODLINES.bin",
    "MODLINES.tab",
    "SAVEGAME.bin",
    "SAVEGAME.tab",
    "OBJSEQ.bin",
    "OBJSEQ.tab",
    "OBJECTS.tab",
    "OBJECTS.bin",
    "OBJINDEX.bin",
    "OBJEVENT.bin",
    "OBJHITS.bin",
    "DLLS.bin",
    "DLLS.tab",
    "DLLSIMPO.bin",
    "MODELS.tab_45",
    "MODELS.bin_46",
    "BLOCKS.bin_47",
    "BLOCKS.tab_48",
    "ANIM.TAB_49",
    "ANIM.BIN_4A",
    "TEX1.bin_4B",
    "TEX1.tab_4C",
    "TEX0.bin_4D",
    "TEX0.tab_4E",
    "TEXPRE.bin",
    "TEXPRE.tab",
    "PREANIM.bin",
    "PREANIM.tab",
    "VOXMAP.tab_53",
    "VOXMAP.bin_54",
    "ANIMCURV.bin_55",
    "ANIMCURV.tab_56",
    "ENVFXACT.bin",
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
        printf("\x1B[1m#│Data    │Size    │Used    │Free    │Slots   │UsedSlot│FreeSlot│FreeB│FreeS\x1B[0m\n")
        heaps = []
        for i in range(5):
            hs = HeapStruct(self.client, 0x803406A0 + (i*HeapStruct.SIZE))
            heaps.append(hs)
        for i, hs in enumerate(heaps):
            printf("\x1B[48;5;%dm%d│%08X│%08X│%08X│%08X│%08X│%08X│%08X│%4d%%│%4d%%\x1B[0m\n",
                ROW_COLOR[i&1], i,
                hs.data, hs.size, hs.usedSize, hs.size-hs.usedSize,
                hs.avail, hs.used, hs.avail-hs.used,
                (1-(hs.usedSize/hs.size))*100 if hs.size != 0 else 0,
                (1-(hs.used/hs.avail))*100 if hs.avail != 0 else 0)


    def listFiles(self):
        printf("\x1B[1mID│FileName        │RamAddr\x1B[0m\n")
        for i in range(0x58):
            filePtr = self.client.read(0x8035f3e8 + (i*4), ">I")
            printf("\x1B[48;5;%dm%02X│%-16s│%08X\x1B[0m\n", ROW_COLOR[i&1],
                i, fileNames[i], filePtr)
