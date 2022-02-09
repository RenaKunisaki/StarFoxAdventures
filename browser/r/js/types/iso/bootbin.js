import Struct from '../../lib/Struct.js';

//Disc header
export const BootBin = Struct(
    ['4s',    'gameCode'],
    ['2s',    'company'],
    ['B',     'discNo'],
    ['B',     'version'],
    ['B',     'audioStreaming'],
    ['B',     'streamBufSize'],
    ['I',     'magic', 1, 0x1C],
    ['992s',  'gameName'],
    ['I',     'debugMonitorOffs', 1, 0x400],
    ['I',     'debugMonitorAddr'],
    ['I',     'mainDolOffs', 1, 0x420],
    ['I',     'fstOffs'],
    ['I',     'fstSize'],
    ['I',     'maxFstSize'],
    ['I',     'fstAddr'],
    ['I',     'fileOffset'],
    ['I',     'fileLength'],
);
