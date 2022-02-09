import Struct from '../../lib/Struct.js';

//App loader
export const Appldr = Struct(
    ['10s', 'buildDate'],
    ['16b', 'pad1'],
    ['I',   'entryPoint'],
    ['I',   'dataSize'],
    ['I',   'trailerSize'],
    //total size = dataSize + trailerSize + 0x20
);
