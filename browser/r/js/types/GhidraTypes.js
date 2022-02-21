import { hex } from "../Util.js";

export const GhidraTypes = {
    /** Maps type names exported from Ghidra to Struct type names
     *  and adds useful info.
     */
    bool:         {type:'b'},
    byte:         {type:'b'},
    char:         {type:'b'}, //in our case it's basically never really char
    double:       {type:'d'},
    float:        {type:'f'},
    GameBit:      {type:'H'},
    GameBit16:    {type:'H'},
    GameBit32:    {type:'I'},
    GameTextId16: {type:'H'},
    GameTextId32: {type:'I'},
    int:          {type:'i'},
    MapDirIdx8:   {type:'B'},
    ObjDefEnum:   {type:'h'},
    ObjectId:     {type:'i', disp: v => `0x${hex(v,8)}`},
    pad8:         {type:'b', disp: v => `0x${hex(v,2)}`},
    pad16:        {type:'h', disp: v => `0x${hex(v,4)}`},
    pad32:        {type:'i', disp: v => `0x${hex(v,8)}`},
    pointer:      {type:'I', disp: v => `0x${hex(v,8)}`},
    RomListObjLoadFlags: {type:'B', disp: v => `0x${hex(v,2)}`},
    s16:          {type:'h'},
    s32:          {type:'i'},
    s64:          {type:'q'},
    s8:           {type:'b'},
    short:        {type:'h'},
    u16:          {type:'H', disp: v => `0x${hex(v,4)}`},
    u32:          {type:'I', disp: v => `0x${hex(v,8)}`},
    u64:          {type:'Q', disp: v => `0x${hex(v,16)}`},
    u8:           {type:'B', disp: v => `0x${hex(v,2)}`},
    ubyte:        {type:'B', disp: v => `0x${hex(v,2)}`},
    uchar:        {type:'B', disp: v => `0x${hex(v,2)}`},
    uint:         {type:'I', disp: v => `0x${hex(v,8)}`},
    undefined:    {type:'B', disp: v => `0x${hex(v,2)}`},
    undefined1:   {type:'B', disp: v => `0x${hex(v,2)}`},
    undefined2:   {type:'H', disp: v => `0x${hex(v,4)}`},
    undefined4:   {type:'I', disp: v => `0x${hex(v,8)}`},
    unk8:         {type:'B', disp: v => `0x${hex(v,2)}`},
    unk16:        {type:'H', disp: v => `0x${hex(v,4)}`},
    unk32:        {type:'I', disp: v => `0x${hex(v,8)}`},
    ushort:       {type:'H', disp: v => `0x${hex(v,4)}`},
    vec3s:        {type:'3h'},
    WarpEnum8:    {type:'B'},
}

export function readGhidraType(app, type, file) {
    let val = file.readType(GhidraTypes[type]);
    //XXX is this useful?
    /* switch(type) {
        case 'bool': val = (val != 0); break;
        case 'GameBit': case 'GameBit16': case 'GameBit32': {
            if(app.game.bits) {
                //XXX val & 0x8000 = invert
                val = app.game.bits[val];
            }
            break;
        }
    } */
    return val;
}
