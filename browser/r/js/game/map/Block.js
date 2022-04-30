import { assertType, hex } from "../../Util.js";
import GameFile from "../GameFile.js";
import Game from "../Game.js";
import Map from "./Map.js";
import DisplayList from '../DisplayList.js';
import SfaTexture from '../SfaTexture.js';
import Texture from '../../app/ui/gl/Texture.js'
import BlockHit from './BlockHit.js';

//struct types
let MapBlock = null;
let GCPolygon, PolygonGroup, DisplayListPtr, Shader, HitsBinEntry, SurfaceType;

export default class Block {
    /** One block of a map.
     */
    constructor(game, map, x, z, val) {
        this.game   =  assertType(game, Game)
        this.app    =  this.game.app;
        this.map    =  assertType(map, Map);
        this.x      =  x; //grid coords, relative to map's origin
        this.z      =  z;
        this.index  = null; //index into tables (HITS, modxx.tab...)
        this.unk1   =  val >> 31;
        this.mod    = (val >> 23) & 0x00FF;
        this.sub    = (val >> 17) & 0x003F;
        this.unk2   =  val        & 0x01FF;
        this.header = null;
        this._triedLoad = undefined;

        //the game does this for some reason
        if(this.mod >= 5 && this.mod < 255) this.mod++;

        if(!MapBlock) {
            MapBlock = this.app.types.getType('sfa.maps.MapBlock');
            GCPolygon = this.app.types.getType('sfa.maps.GCPolygon');
            PolygonGroup = this.app.types.getType('sfa.maps.PolygonGroup');
            DisplayListPtr = this.app.types.getType('sfa.maps.DisplayListPtr');
            Shader = this.app.types.getType('sfa.maps.Shader');
            HitsBinEntry = this.app.types.getType('sfa.maps.HitsBinEntry');
            SurfaceType = this.app.types.getType('sfa.maps.SurfaceType');
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
        //I have no idea why this mechanic exists. maybe because of HITS.bin
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
        let view;
        try {
            this.index      = firstBlock + this.sub;
            const offsBlock = dTab.getUint32(this.index*4) & 0xFFFFFF;
            const blockData = fBlock.decompress(offsBlock);
            view            = new DataView(blockData);
            this.header     = MapBlock.fromBytes(view);
            //console.log("Map Block Header", this.header);
        }
        catch(ex) {
            console.error("Error loading block", this, ex);
            this._triedLoad = false;
            return false;
        }

        //read the block's data
        try {
            this._loadVtxData(view);
            this._loadPolygons(view);
            this._loadDlists(view);
            this._loadRenderInstrs(view);
            this._loadShaders(view);
            this._loadTextures(view);
            this._loadHits(view);
        }
        catch(ex) {
            console.error("Error loading block data", this, ex);
            this._triedLoad = false;
            return false;
        }
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

        //here we could make a GL buffer for each of these. the problem is,
        //the attribute format can change during a render stream, so we don't
        //know what the actual values being read from the buffer are until
        //we parse the display lists, and the same value could even possibly
        //be used multiple times as different formats. (unlikely, but
        //it can be done.)
        //(maybe we can make a new buffer containing all values in the new
        //format whenever the format changes?)
    }

    _loadPolygons(view) {
        //read polygon data
        const offs = view.byteOffset;
        this.polygons   = [];
        this.polyGroups = [];
        for(let i=0; i<this.header.nPolygons; i++) {
            const offset = offs + this.header.GCpolygons + (i * GCPolygon.size);
            const poly   = GCPolygon.fromBytes(view, offset);
            poly.offset  = offset;
            poly.index   = i;
            this.polygons.push(poly);
        }
        for(let i=0; i<this.header.nPolyGroups; i++) {
            const offset = offs + this.header.polygonGroups + (i * PolygonGroup.size);
            const group  = PolygonGroup.fromBytes(view, offset);
            group.offset = offset;
            group.index  = i;
            this.polyGroups.push(group);
        }
        for(let i=0; i<this.header.nPolyGroups; i++) {
            const group    = this.polyGroups[i];
            const lastPoly = this.polyGroups[i+1] ?
                this.polyGroups[i+1].firstPolygon : this.header.nPolyGons;
            for(let iPoly=group.firstPolygon; iPoly<lastPoly; iPoly++) {
                this.polygons[iPoly].groupIdx = i;
                this.polygons[iPoly].group = group;
            }
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
        //console.assert(Shader.size == 0x44);
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
                    this.textures.push(this.gx.missingTexture);
                }
            }
            catch(ex) {
                console.error("Failed loading texture", i, ex);
            }
        }
        console.log("Block textures", this.textures, gameTextures);
    }

    _loadHits(view) {
        //read HITS.bin
        const fTab      = this.game.iso.getFile('/HITS.tab');
        const fBin      = this.game.iso.getFile('/HITS.bin');
        const dTab      = fTab.getData();
        const dBin      = fBin.getData();
        const offset    = dTab.getUint32(this.index*4) & 0xFFFFFF;
        const nextoffs  = dTab.getUint32((this.index+1)*4) & 0xFFFFFF;
        const size      = nextoffs - offset;
        const count     = Math.trunc(size / HitsBinEntry.size);
        this.hitsOffset = offset;
        this.hits       = [];
        for(let iHit=0; iHit<count; iHit++) {
            const offs  = offset + (iHit * HitsBinEntry.size);
            const entry = HitsBinEntry.fromBytes(dBin, offs);
            this.hits.push(new BlockHit(this, entry, iHit, offs));
        }
    }
}
