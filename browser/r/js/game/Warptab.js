import { assertType } from '../Util.js';
import Game from './Game.js';

//struct types
let WarpTabEntry;

export default function parseWarpTab(game) {
    assertType(game, Game);
    WarpTabEntry  = game.app.types.getType('sfa.maps.WarpTabEntry');
    const warpTab = game.iso.getFile('/WARPTAB.bin').getData();
    const result  = [];
    for(let offs=0; offs < warpTab.byteLength; offs += WarpTabEntry._size) {
        let entry = WarpTabEntry.fromBytes(warpTab, offs);
        //if(entry.x == 0 && entry.y == 0 && entry.z == 0) entry = null;
        result.push(entry);
    }
    console.assert(result.length == 0x80);
    return result;
}
