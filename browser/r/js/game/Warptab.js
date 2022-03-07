import App from '../app/App.js';
import Struct from '../lib/Struct.js';
import { assertType } from '../Util.js';

const WarpTabEntry = Struct(
    ['f', 'x'],
    ['f', 'y'],
    ['f', 'z'],
    ['h', 'layer'],
    ['h', 'xRot'],
);

export default function parseWarpTab(app) {
    assertType(app, App);
    const warpTab = app.game.iso.getFile('/WARPTAB.bin').getData();
    const result  = [];
    for(let offs=0; offs < warpTab.byteLength; offs += WarpTabEntry._size) {
        let entry = new WarpTabEntry(warpTab, offs);
        //if(entry.x == 0 && entry.y == 0 && entry.z == 0) entry = null;
        result.push(entry);
    }
    console.assert(result.length == 0x80);
    return result;
}
