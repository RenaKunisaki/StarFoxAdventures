import { assertType, hex } from "../../Util.js";
import GameFile from "../GameFile.js";
import Game from "../Game.js";
import Map from "./Map.js";
import DisplayList from '../DisplayList.js';

//struct types
let MapBlock = null;
let GCPolygon, PolygonGroup, DisplayListPtr, Shader;

export default class Block {
    /** One block of a map.
     */
    constructor(game, map, x, z, val) {
        this.game   =  assertType(game, Game)
        this.app    =  this.game.app;
        this.map    =  assertType(map, Map);
        this.x      =  x; //grid coords, relative to map's origin
        this.z      =  z;
        this.unk1   =  val >> 31;
        this.mod    = (val >> 23) & 0x00FF;
        this.sub    = (val >> 17) & 0x003F;
        this.unk2   =  val        & 0x01FF;
        this.header = null;
        this._triedLoad = false;

        //the game does this for some reason
        if(this.mod >= 5 && this.mod < 255) this.mod++;

        if(!MapBlock) {
            MapBlock = this.app.types.getType('sfa.maps.MapBlock');
            GCPolygon = this.app.types.getType('sfa.maps.GCPolygon');
            PolygonGroup = this.app.types.getType('sfa.maps.PolygonGroup');
            DisplayListPtr = this.app.types.getType('sfa.maps.DisplayListPtr');
            Shader = this.app.types.getType('sfa.maps.Shader');
        }
    }

    load() {
        /** Load the block model from disc. */
        if(this._triedLoad) return;
        this._triedLoad = true;
        //even if loading fails, don't try again

        const path = `/${this.map.dirName}/mod${this.mod}`;
        //console.log(`Loading map block ${this.mod}.${this.sub} from`, path, this);
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
        //console.log("Map Block Header", this.header);

        this._loadVtxData(view);
        this._loadPolygons(view);
        this._loadDlists(view);
        this._loadRenderInstrs(view);
        this._loadShaders(view);

        //XXX read textures
        this.textures = [];
    }

    _loadVtxData(view) {
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

        //debug
        let vtxs=[], colors=[], texCoords=[];
        for(let i=0; i<this.header.nVtxs; i++) {
            vtxs.push([
                view.getInt16(offs + this.header.vertexPositions + (i*6)),
                view.getInt16(offs + this.header.vertexPositions + (i*6)+2),
                view.getInt16(offs + this.header.vertexPositions + (i*6)+4),
            ]);
        }
        for(let i=0; i<this.header.nColors; i++) {
            colors.push(view.getInt16(offs + this.header.vertexColors + (i*2)));
        }
        for(let i=0; i<this.header.nTexCoords; i++) {
            texCoords.push([
                view.getInt16(offs + this.header.vertexTexCoords + (i*4)),
                view.getInt16(offs + this.header.vertexTexCoords + (i*4)+2),
            ]);
        }
        //console.log("vertexPositions", vtxs);
        //console.log("vertexColors", colors);
        //console.log("texCoords", texCoords);
    }

    _loadPolygons(view) {
        //read polygon data
        const offs = view.byteOffset;
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
    }

    _loadDlists(view) {
        //read display lists
        const offs = view.byteOffset;
        this.dlists = [];
        for(let i=0; i<this.header.nDlists; i++) {
            let list = new DisplayList(this.game, view,
                this.header.displayLists + (i * DisplayListPtr.size));
            this.dlists.push(list);

            const bytes = new DataView(list.data);
            let butt = [];
            for(let n=0; n<DisplayListPtr.size; n++) {
                butt.push(hex(bytes.getUint8(n), 2));
            }
            //console.log("Block Dlist", i, list, butt.join(' '));
        }
    }

    _loadRenderInstrs(view) {
        //read render instructions (bit-packed stream)
        const offs = view.byteOffset;
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
    }

    _loadShaders(view) {
        //read shader data
        const offs = view.byteOffset;
        this.shaders = [];
        for(let i=0; i<this.header.nShaders; i++) {
            this.shaders.push(Shader.fromBytes(view,
                offs + this.header.shaders + (i * Shader.size)));
        }
    }
}
