import GameFile from "../GameFile.js";
import Struct from "../../lib/Struct.js";
import { Vec3f } from "../../types/common.js";
import { assertType } from "../../Util.js";
import Game from "../Game.js";

export const RomListEntryStruct = Struct(
    ['h',   'objDef'],
    ['B',   'length'], //in 4-byte words
    ['B',   'acts0'],
    ['B',   'loadFlags'],
    ['B',   'acts1'],
    ['B',   'bound'],
    ['B',   'cullDist'],
    [Vec3f, 'position'],
    ['i',   'id'],
    //following is object-specific params
);

class RomListEntry {
    constructor(game, data, offset) {
        this.game       = assertType(game, Game);
        this.app        = game.app;
        const base      = new RomListEntryStruct(data, offset);
        console.assert(base.length >= 6);
        this.byteLength = base.length * 4;
        this.objDef     = base.objDef;
        this.acts       = [base.acts0, base.acts1]; //XXX parse
        this.loadFlags  = base.loadFlags;
        this.bound      = base.bound;
        this.cullDist   = base.cullDist;
        this.position   = base.position;
        this.id         = base.id;
        this.paramData  = new DataView(data.buffer,
            offset+data.byteOffset+0x18,
            this.byteLength-0x18);
        this.params     = null;

        if(this.game.objects) {
            //get the object
            let defNo = this.objDef;
            defNo = (defNo < 0) ? -defNo : this.game.objIndex[defNo];
            this.object = this.game.objects[defNo];
            if(!this.object) this.object = this.game.objects[0];

            //parse the object-specific params
            const dlls = this.game.dlls;
            const dll = dlls ? dlls[this.object.dll_id] : null;
            if(dll && dll.objParams) {
                this.params  = {};
                const params = dll.readObjParams(this.paramData);
                for(let [name, param] of Object.entries(dll.objParams)) {
                    this.params[name] = {
                        //XXX this is gross, we end up with param.param
                        //and value.value
                        param: param,
                        value: params[name],
                    };
                }
            }
        }
    }
}

export default class RomList {
    /** romlist file, defines objects on a map. */
    constructor(game, view) {
        /** Construct RomList.
         *  @param {Game} game the game this is from.
         *  @param {DataView} view the data to read.
         */
        this.game    = assertType(game, Game);
        this.entries = [];
        if(view instanceof GameFile) view = new DataView(view.decompress());
        if(view instanceof ArrayBuffer) view = new DataView(view);
        for(let offs=0; offs<view.byteLength;) {
            let entry = new RomListEntry(this.game, view, offs);
            this.entries.push(entry);
            offs += entry.byteLength;
        }
    }
}
