import {int, hex, getDescriptionAndNotes} from './Util.js';
import {ObjDef} from './Structs.js';

function loadFlagsToStr(flags) {
    return [
        (flags & 0x01) ? 'L' : '-',
        (flags & 0x02) ? 'M' : '-',
        (flags & 0x04) ? 'P' : '-',
        (flags & 0x08) ? 'C' : '-',
        (flags & 0x10) ? 'O' : '-',
        (flags & 0x20) ? 'B' : '-',
        (flags & 0x40) ? '4' : '-',
        (flags & 0x80) ? '8' : '-',
    ].join('')
}

function actListToStr(objDef) {
    let acts = [];
    for(let i=0; i<8; i++) {
        let b = (objDef.acts1 >> (7-i)) & 1;
        acts.push(b ? '-' : hex(i+1, 1));
    }
    for(let i=8; i<16; i++) {
        let b = (objDef.acts2 >> (i-8)) & 1;
        let a = hex(i+1, 1);
        if(a.length > 1) a = '0';
        acts.push(b ? '-' : a);
    }
    return acts.join('');
}

export default class RomList {
    /** A map's romlist file.
     */
    constructor(game, data) {
        /** Construct a RomList from the given data.
         *  @param game the Game instance.
         *  @param data the decompressed romlist file.
         */
        this.game      = game;
        this.items     = [];
        const listView = new DataView(data);
        this.byteLength = data.byteLength;

        let offset = 0, idx = 0;
        while(offset < data.byteLength) {
            if(offset + 0x18 > data.byteLength) {
                console.error(`Offset 0x${hex(offset)} is out of bounds (romlist length 0x${data.byteLength})`);
                break;
            }

            let objDef;
            //try {
                objDef = new ObjDef(data, offset);
                objDef.params = [];
                for(let i=0; i<objDef.size-6; i++) {
                    let val = listView.getUint32((offset+0x18)+(i*4));
                    objDef.params.push(hex(val, 8));
                }
            //}
            //catch(ex) { break }

            //get real obj type
            let realId = objDef.type;
            realId = (realId < 0) ? -realId : this.game.objIndex[realId];

            //get obj name
            const obj = this.game.objects[realId];
            const name = obj ? obj.name : `&lt;obj 0x${hex(realId,4)}&gt;`;

            this.items.push({
                idx:       idx,
                offset:    offset,
                objDef:    objDef,
                realId:    realId,
                name:      name,
                acts:      actListToStr(objDef),
                loadFlags: loadFlagsToStr(objDef.loadFlags),
            });

            offset += objDef.size * 4;
            idx++;
        }
    }
}
