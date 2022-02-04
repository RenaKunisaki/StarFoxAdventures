/** Define structs used by GCI files (GameCube memory card image).
 */
import Struct from '../lib/Struct.js';

export const Header = Struct(
    ['4s',  'gameCode'],
    ['2s',  'company'],
    ['b',   'pad06'],
    ['b',   'bannerFmt'],
    ['32s', 'filename'],
    ['I',   'lastModified'], //seconds since 2000-01-01 00:00:00
    ['I',   'iconAddr'],     //where in the file to find the icon image
    ['H',   'iconFmt'],
    ['H',   'iconSpeed'],
    ['B',   'permissions'],
    ['B',   'copyCount'],
    ['H',   'block'],        //which block this file begins at, plus 5
    ['H',   'length'],
    ['H',   'pad3A'],
    ['H',   'commentAddr'],  //where in the file to find the comment string
);
