import Struct from "../../lib/Struct.js";

export const MapsBinEntry0 = Struct(
    ['h',  'sizeX'],
    ['h',  'sizeZ'],
    ['h',  'originX'],
    ['h',  'originZ'],
    ['I',  'unk08'],
    ['4I', 'unk0C'], //rect related? 4 of these, 4 of those...
    ['h',  'nBlocks'],
    ['H',  'unk1E'], //flags?
);
export const MapsTabEntry = Struct(
    ['I',  'info'], // -> MapsBinEntry0
    ['I',  'blocks'], // -> u32
    ['4I', 'rects'],
    ['I',  'romList'], //-> FACEFEED
);
//in old versions, romList pointed to the actual list; in final it points
//to a FACEFEED header with no data following, which is only used to tell the
//size needed to allocate for the list, which is in a separate file.
//(it will still use the data here if there is no separate file.)
//XXX check for any maps that have old data here.

export const MapInfoEntry = Struct(
    ['28s', 'name'],
    ['B',   'type'],
    ['B',   'unk1D'],   //always 6
    ['h',   'objType'], //ObjDefEnum for player (not used in final)
);
export const MapGridItem = Struct(
    ['h',  'x'],     //global X coord
    ['h',  'z'],     //global Z coord
    ['h',  'layer'], //layer (-2 to 2)
    ['h',  'mapId'], //map ID (if <0, marks end of list)
    ['2h', 'link'],  //linked map IDs
);
