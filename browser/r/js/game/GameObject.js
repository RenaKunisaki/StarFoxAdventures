import { assertType, hex } from "../Util.js";
import Struct from '../lib/Struct.js';
import Game from "./Game.js";

const ObjectData = Struct(
    ['f',   'unk00'],
    ['f',   'scale'],
    ['I',   'modelList'],
    ['I',   'textures'],
    ['I',   'unk10'], //unknown*
    ['I',   'unk14'], //unknown*
    ['I',   'seqCmds'], //ObjSeqCmds*
    ['I',   'seq'], //u16*
    ['I',   'event'], //unknown*
    ['I',   'hits'], //unknown*
    ['I',   'weaponData'], //unknown*
    ['I',   'attachPoints'], //AttachPoint*
    ['I',   'modLines'], //s16*
    ['I',   'intersectPoints'], //unknown*
    ['I',   'nextIntersectPoint'], //unknown*
    ['I',   'nextIntersectLine'], //unknown*
    ['I',   'aButtonInteraction'], //AButtonInteraction*
    ['I',   'flags'], //ObjFileStructFlags44
    ['H',   'shadowType'], //ObjFileStruct_ShadowType
    ['h',   'shadowTexture'],
    ['h',   'unk4C'],
    ['H',   'hitbox_flags60'], //HitboxFlags60
    ['h',   'dll_id'], //DLL_ID
    ['H',   'catId'], //ObjCatId
    ['B',   'unk54'],
    ['B',   'nModels'],
    ['B',   'nPlayerObjs'],
    ['B',   'unk57'],
    ['B',   'nAttachPoints'],
    ['B',   'nTextures'],
    ['B',   'nVecs'],
    ['B',   'unk5B'],
    ['B',   'modLinesSize'],
    ['b',   'modLinesIdx'],
    ['B',   'nSeqs'],
    ['B',   'flags5F'], //ObjFileStructFlags5F
    ['B',   'hitbox_fieldB0'],
    ['B',   'hasHitbox'],
    ['B',   'hitboxSizeXY'],
    ['B',   'hitbox_field6A'],
    ['B',   'hitbox_field6B'],
    ['B',   'hitbox_flags62'], //HitboxFlags62
    ['B',   'unk66'],
    ['B',   'hitbox_fieldB5'],
    ['h',   'hitboxSizeX1'],
    ['h',   'hitboxSizeY1'],
    ['h',   'hitboxSizeZ1'],
    ['h',   'hitboxSizeZ2'],
    ['B',   'hitbox_fieldB4'],
    ['B',   'flags71'],
    ['B',   'nFocusPoints'],
    ['B',   'cullDistance'],
    ['B',   'unk74'],
    ['B',   'unk75'],
    ['B',   'flags76'], //ObjectFileStructFlags76
    ['B',   'hitboxSizeZ'],
    ['h',   'map'], //MapDirIdx16
    ['h',   'unk7A'],
    ['4h',  'helpTexts'], //GameTextId (one per model)
    ['h',   'unk84'],
    ['h',   'unk86'],
    ['f',   'lagVar88'],
    ['B',   'nLights'],
    ['B',   'lightIdx'],
    ['B',   'colorIdx'],
    ['B',   'unk8F'],
    ['B',   'hitbox_flagsB6'], //HitboxFlags62 again
    ['11s', 'name'],
    //following is fields pointed to by the above.
    //only way to determine size is by OBJECTS.TAB
);

export default class GameObject {
    /** An object definition in the game.
     */
    constructor(game, idx) {
        /** Construct object with given index into OBJECTS.tab.
         */
        this.game = assertType(game, Game);
        this.app  = game.app;

        const objsTab = this.game.iso.getFile('/OBJECTS.tab').getData();
        const objsBin = this.game.iso.getFile('/OBJECTS.bin');
        const objsBinData = objsBin.getData();
        const offset  = objsTab.getUint32(idx*4);
        const size    = objsTab.getUint32((idx+1)*4) - offset;
        if(offset >= objsBinData.byteLength) {
            throw new Error(`Invalid object ID 0x${hex(idx)}`);
        }
        this.offset   = offset;
        this.dataSize = size;
        this.id       = idx;
        this.data     = objsBin.getData(offset, size);
        const header  = new ObjectData(this.data);
        this.header   = header;
        this.index    = null; //from OBJINDEX.bin

        return new Proxy(this, {
            get: function(instance, key) {
                if(key in instance) return instance[key];
                return header[key];
            }
        });
    }
}
