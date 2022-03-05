import GameFile from "../game/GameFile.js";
import Struct from "../lib/Struct.js";
import { Vec3f } from "./common.js";

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
    constructor(app, data, offset) {
        this.app        = app;
        const base      = new RomListEntryStruct(data, offset);
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

        if(this.app.game.objects) {
            //get the object
            let defNo = this.objDef;
            if(defNo < 0) defNo = -defNo;
            else defNo = this.app.game.objIndex[defNo];
            this.object = this.app.game.objects[defNo];
            if(!this.object) this.object = this.app.game.objects[0];

            //parse the object-specific params
            const dll = this.app.game.dlls[this.object.dll_id];
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
    constructor(app, view) {
        /** Construct RomList.
         *  @param {App} app the app instance.
         *  @param {DataView} view the data to read.
         */
        this.app     = app;
        this.entries = [];
        if(view instanceof GameFile) view = view.decompress();
        for(let offs=0; offs<view.byteLength;) {
            let entry = new RomListEntry(this.app, view, offs);
            this.entries.push(entry);
            offs += entry.byteLength;
        }
    }
}
