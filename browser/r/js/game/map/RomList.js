import GameFile from "../GameFile.js";
import { assertType } from "../../Util.js";
import Game from "../Game.js";

//struct types
let Vec3f, RomListEntryStruct;

class RomListEntry {
    constructor(game, data, offset, idx) {
        /** Construct RomListEntry.
         *  @param {Game} game Game instance this belongs to.
         *  @param {DataView} data Data to construct from.
         *  @param {integer} offset Offset to read from.
         *  @param {integer} idx Which index in the list this is.
         *    Only used to display in the UI.
         */
        this.game       = assertType(game, Game);
        this.app        = game.app;
        this.idx        = idx;

        Vec3f = this.app.types.getType('vec3f');
        RomListEntryStruct = this.app.types.getType('sfa.maps.RomListEntry');

        const view      = new DataView(data.buffer);
        const base      = RomListEntryStruct.fromBytes(view, offset);
        console.assert(base.length >= 6);
        this.byteLength = base.length * 4;
        this.objDef     = base.objDef;
        this.acts       = [false]; //act 0 loads no objects
        this.loadFlags  = base.loadFlags;
        this.bound      = base.bound;
        this.cullDist   = base.cullDist;
        this.position   = base.position;
        this.id         = base.id;
        this.paramData  = new DataView(data.buffer,
            offset+data.byteOffset,
            this.byteLength);
        this.params     = null;

        for(let i=1; i<16; i++) {
            let disp = 0;
            if(i >= 9) disp = base.acts1 & (1 << (7-(i-9)));
            else disp = base.acts0 & (1 << (i-1));
            this.acts.push(disp == 0); //bit set = do NOT show in this act
        }

        if(this.game.objects) {
            //get the object
            let defNo = this.objDef;
            defNo = (defNo < 0) ? -defNo : this.game.objIndex[defNo];
            this.object = this.game.objects[defNo];
            if(!this.object) this.object = this.game.objects[0];

            //parse the object-specific params
            const dlls = this.game.dlls;
            const dll = dlls ? dlls[this.object.dll_id] : null;
            //console.log("Object", this, "dll", dll);
            if(dll && dll.objParams) {
                this.params  = {};
                const params = dll.readObjParams(this.paramData);
                for(let [name, param] of Object.entries(params)) {
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
            let entry = new RomListEntry(this.game, view, offs,
                this.entries.length);
            this.entries.push(entry);
            offs += entry.byteLength;
        }
    }
}
