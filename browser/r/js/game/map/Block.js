import { assertType } from "../../Util.js";
import GameFile from "../GameFile.js";
import Game from "../Game.js";
import Map from "./Map.js";
import DisplayList from '../DisplayList.js';

//struct types
let MapBlock = null;
let GCPolygon, PolygonGroup, DisplayListPtr;

export default class Block {
    /** One block of a map.
     */
    constructor(game, map, x, z, val) {
        this.game   =  assertType(game, Game)
        this.app    =  this.game.app;
        this.map    =  assertType(map, Map);
        this.x      =  x;
        this.z      =  z;
        this.unk1   =  val >> 31;
        this.mod    = (val >> 23) & 0x00FF;
        this.sub    = (val >> 17) & 0x003F;
        this.unk2   =  val        & 0x01FF;
        this.header = null;
        this._triedLoad = false;

        //the game does this for some reason
        if(this.mod >= 5) this.mod++;

        if(!MapBlock) {
            MapBlock = this.app.types.getType('sfa.maps.MapBlock');
            GCPolygon = this.app.types.getType('sfa.maps.GCPolygon');
            PolygonGroup = this.app.types.getType('sfa.maps.PolygonGroup');
            DisplayListPtr = this.app.types.getType('sfa.maps.DisplayListPtr');
        }
    }

    load() {
        /** Load the block model from disc. */
        if(this._triedLoad) return;
        this._triedLoad = true;
        //even if loading fails, don't try again

        const path = `/${this.map.dirName}/mod${this.mod}`;
        console.log(`Loading map block ${this.mod}.${this.sub} from`, path, this);
        const fTab   = this.game.iso.getFile(`${path}.tab`);
        if(!fTab) {
            console.error("File not found", `${path}.tab`);
            return;
        }
        const dTab   = fTab.getData();
        const fBlock = new GameFile(this.game.iso.getFile(
            `${path}.zlb.bin`));
        const offsBlock = dTab.getUint32(this.sub*4) & 0xFFFFFF;
        const blockData = fBlock.getItem(this.sub); //XXX use tab somehow
        const view      = new DataView(blockData);
        this.header     = MapBlock.fromBytes(view);
        console.log("Map Block Data", this.header);

        //read vertex data
        const offs = view.byteOffset;
        this.vtxPositions = view.buffer.slice( //vec3s[]
            offs + this.header.vertexPositions,
            offs + this.header.vertexPositions + (this.header.nVtxs * 6),
        );
        this.vtxColors = view.buffer.slice( //u16[]
            offs + this.header.vertexColors,
            offs + this.header.vertexColors + (this.header.nColors * 2),
        );
        this.texCoords = view.buffer.slice( //vec2s[]
            offs + this.header.vertexTexCoords,
            offs + this.header.vertexTexCoords + (this.header.nTexCoords * 4),
        );

        //read polygon data
        this.polygons   = [];
        this.polyGroups = [];
        for(let i=0; i<this.header.nPolygons; i++) {
            this.polygons.push(GCPolygon.fromBytes(view,
                offs + this.header.GCpolygons + (i * GCPolygon.size)));
        }
        for(let i=0; i<this.header.nPolyGroups; i++) {
            this.polyGroups.push(PolygonGroup.fromBytes(view,
                offs + this.header.polygonGroups + (i * PolygonGroup.size)));
        }

        //read display lists
        this.dlists = [];
        for(let i=0; i<this.header.nDlists; i++) {
            this.dlists.push(new DisplayList(this.game, view,
                this.header.displayLists + (i * DisplayListPtr.size)));
        }

        //read render instructions (bit-packed stream)
        this.renderInstrs = {
            main:       view.buffer.slice(
                offs + this.header.renderInstrsMain,
                offs + this.header.renderInstrsMain + this.header.nRenderInstrsMain,
            ),
            reflective: view.buffer.slice(
                offs + this.header.renderInstrsReflective,
                offs + this.header.renderInstrsReflective + this.header.nRenderInstrsReflective,
            ),
            water:      view.buffer.slice(
                offs + this.header.renderInstrsWater,
                offs + this.header.renderInstrsWater + this.header.nRenderInstrsWater,
            ),
        };

        //XXX read shaders, textures
        this.shaders  = [];
        this.textures = [];
    }
}
