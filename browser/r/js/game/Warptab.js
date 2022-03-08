import Struct from '../lib/Struct.js';
import { assertType } from '../Util.js';
import Game from './Game.js';

const WarpTabEntry = Struct(
    ['f', 'x'],
    ['f', 'y'],
    ['f', 'z'],
    ['h', 'layer'],
    ['h', 'xRot'],
);

export default function parseWarpTab(game) {
    assertType(game, Game);
    const warpTab = game.iso.getFile('/WARPTAB.bin').getData();
    const result  = [];
    for(let offs=0; offs < warpTab.byteLength; offs += WarpTabEntry._size) {
        let entry = new WarpTabEntry(warpTab, offs);
        //if(entry.x == 0 && entry.y == 0 && entry.z == 0) entry = null;
        result.push(entry);
    }
    console.assert(result.length == 0x80);
    return result;
}
