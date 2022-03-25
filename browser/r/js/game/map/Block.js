import { assertType } from "../../Util.js";
import GameFile from "../GameFile.js";
import Game from "../Game.js";
import Map from "./Map.js";

//struct types
let MapBlock = null;

export default class Block {
    /** One block of a map.
     */
    constructor(game, map, x, z, val) {
        this.game =  assertType(game, Game)
        this.app  =  this.game.app;
        this.map  =  assertType(map, Map);
        this.x    =  x;
        this.z    =  z;
        this.unk1 =  val >> 31;
        this.mod  = (val >> 23) & 0x00FF;
        this.sub  = (val >> 17) & 0x003F;
        this.unk2 =  val        & 0x01FF;
        this._triedLoad = false;

        //the game does this for some reason
        if(this.mod >= 5) this.mod++;

        if(!MapBlock) {
            MapBlock = this.app.types.getType('sfa.maps.MapBlock');
        }
    }

    load() {
        /** Load the block model from disc. */
        if(this._triedLoad) return;
        this._triedLoad = true;
        //even if loading fails, don't try again

        const path = `/${this.map.dirName}/mod${this.mod}`;
        console.log(`Loading map block ${this.mod}.${this.sub} from`, path);
        const fTab   = this.game.iso.getFile(`${path}.tab`).getData();
        const fBlock = new GameFile(this.game.iso.getFile(
            `${path}.zlb.bin`));
        const offsBlock = fTab.getUint32(this.sub*4) & 0xFFFFFF;
        const blockData = fBlock.getItem(this.sub); //XXX use tab somehow
        const view      = new DataView(blockData);
        const header    = MapBlock.fromBytes(view);
        console.log("Map Block Data", header);
    }
}
