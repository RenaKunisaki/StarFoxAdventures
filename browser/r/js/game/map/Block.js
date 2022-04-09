import { assertType, hex } from "../../Util.js";
import GameFile from "../GameFile.js";
import Game from "../Game.js";
import Map from "./Map.js";
import DisplayList from '../DisplayList.js';
import SfaTexture from '../SfaTexture.js';
import Texture from '../../app/ui/gl/Texture.js'

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
        this._triedLoad = undefined;
        this.batchOps = {}; //filled in by BlockRenderer

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

    load(gx) {
        /** Load the block model from disc.
         *  @param {GX} gx The GX instance to use.
         *  @returns {boolean} Whether the block loaded successfully.
         *  @note It's normal for some blocks to fail to load, since some
         *   maps have blocks (eg #34) out of bounds, that don't actually
         *   exist anywhere on the disc.
         */
        if(this._triedLoad != undefined) return this._triedLoad;
        this.gx = gx;
        this._triedLoad = true;
        this._loadModel();
        return true;
    }

    _loadModel() {
        //TRKBLK.tab gives an offset per map dir ID, to add to the block
        //sub idx to get the actual offset within the modN.tab file.
        //I have no idea why this mechanic exists.
        const trkBlk     = this.game.iso.getFile(`/TRKBLK.tab`).getData();
        const firstBlock = trkBlk.getUint16(this.map.dirId*2);
        const modPath    = `/${this.map.dirName}/mod${this.mod}`;
        const fTab       = this.game.iso.getFile(`${modPath}.tab`);
        if(!fTab) {
            console.error("File not found", `${modPath}.tab`);
            this._triedLoad = false;
            return false;
        }
        const dTab   = fTab.getData();
        let   fBlock = this.game.iso.getFile(`${modPath}.zlb.bin`);
        if(!fBlock) {
            console.error("File not found", `${modPath}.zlb.bin`);
            this._triedLoad = false;
            return false;
        }
        fBlock = new GameFile(fBlock);

        //get the data for this block
        const offsBlock = dTab.getUint32((this.sub+firstBlock)*4) & 0xFFFFFF;
        const blockData = fBlock.decompress(offsBlock);
        const view      = new DataView(blockData);
        this.header     = MapBlock.fromBytes(view);
        //console.log("Map Block Header", this.header);

        //read the block's data
        this._loadVtxData(view);
        this._loadPolygons(view);
        this._loadDlists(view);
        this._loadRenderInstrs(view);
        this._loadShaders(view);
        this._loadTextures(view);
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
        /* let vtxs=[], colors=[], texCoords=[];
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
        console.log("vertexPositions", vtxs);
        console.log("vertexColors", colors);
        console.log("texCoords", texCoords); */
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
        console.assert(Shader.size == 0x44);
        for(let i=0; i<this.header.nShaders; i++) {
            this.shaders.push(Shader.fromBytes(view,
                offs + this.header.shaders + (i * Shader.size)));
        }
    }

    _loadTextures(view) {
        //load the texture files for this map
        const dir = `/${this.map.dirName}`;
        this.textures = [];
        const gameTextures = []; //debug
        console.log("Block has %d textures", this.header.nTextures);
        for(let i=0; i<this.header.nTextures; i++) {
            try {
                let texId = view.getInt32(this.header.textures + (i*4));
                texId = -(texId|0x8000); //game's odd way to use TEX1 by default
                const gTex = this.game.loadTexture(texId, dir);
                gameTextures.push(gTex);
                if(gTex) {
                    const tex = new Texture(this.gx.context);
                    tex.loadGameTexture(gTex);
                    //tex.loadFromImage('/r/f-texture.png');
                    this.textures.push(tex);
                }
                else {
                    console.warn(`Failed loading texture 0x${hex(texId)}`);
                    this.textures.push(gx.missingTexture);
                }
            }
            catch(ex) {
                console.error("Failed loading texture", i, ex);
            }
        }
        console.log("Block textures", this.textures, gameTextures);
    }
}
