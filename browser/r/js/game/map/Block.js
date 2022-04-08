import { assertType, hex } from "../../Util.js";
import GameFile from "../GameFile.js";
import Game from "../Game.js";
import Map from "./Map.js";
import DisplayList from '../DisplayList.js';
import SfaTexture from '../SfaTexture.js';

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

    load() {
        /** Load the block model from disc.
         *  @returns {boolean} Whether the block loaded successfully.
         *  @note It's normal for some blocks to fail to load, since some
         *   maps have blocks (eg #34) out of bounds, that don't actually
         *   exist anywhere on the disc.
         */
        if(this._triedLoad != undefined) return this._triedLoad;
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
        const fBlock = new GameFile(this.game.iso.getFile(`${modPath}.zlb.bin`));

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
        let   fTab0  = this.game.iso.getFile(`${dir}/TEX0.tab`);
        let   fBin0  = this.game.iso.getFile(`${dir}/TEX0.bin`);
        let   fTab1  = this.game.iso.getFile(`${dir}/TEX1.tab`);
        let   fBin1  = this.game.iso.getFile(`${dir}/TEX1.bin`);
        if(!fTab0) {
            console.error("Texture files not found in", dir);
            return false;
        }
        fBin0 = new GameFile(fBin0);
        fBin1 = new GameFile(fBin1);
        fTab0 = new GameFile(fTab0);
        fTab1 = new GameFile(fTab1);

        const texTable = new GameFile(this.game.iso.getFile('/TEXTABLE.bin'));
        const fTab2 = new GameFile(this.game.iso.getFile('/TEXPRE.tab'));
        const fBin2 = new GameFile(this.game.iso.getFile('/TEXPRE.bin'));

        const bins = [fBin0, fBin1, fBin2];
        const tabs = [fTab0, fTab1, fTab2];

        //load the textures
        this.textures = [];
        console.log("Block has %d textures", this.header.nTextures);
        for(let i=0; i<this.header.nTextures; i++) {
            let texId = view.getInt32(this.header.textures + (i*4));
            texId = -(texId|0x8000); //game's odd way to use TEX1 by default
            if(texId < 0) texId = -texId;
            else {
                texTable.seek(texId*2);
                const newId = texTable.readU16();
                console.log(`texId 0x${hex(texId,4)} => 0x${hex(newId,4)}`)
                texId = newId + (((texId < 3000) || (newId == 0)) ? 0 : 1);
            }
            let tblIdx = 0;
            if     (texId & 0x8000) { tblIdx = 1; texId &= 0x7FFF; }
            else if(texId >=  3000) { tblIdx = 2; }
            const fBin=bins[tblIdx], fTab=tabs[tblIdx];

            fTab.seek(texId*4);
            let texOffs = fTab.readU32();
            //console.log(`Load texture ${texId} from offs 0x${hex(texOffs,8)} tbl ${tblIdx}`);

            //XXX investigate high byte of tab, something to do with mipmaps
            //and/or animation frames
            let unkCount = 0; //((texOffs >> 24) & 0x3F) + 1;
            texOffs = (texOffs & 0xFFFFFF) * 2;
            const data = fBin.decompress(texOffs + (unkCount*4));
            console.log(`texId=0x${hex(texId)} tbl=${tblIdx} texOffs=0x${hex(texOffs,6)} unkCount=${unkCount}`, data);
            const texView = new DataView(data, 0x10); //no idea
            const tex = SfaTexture.fromData(this.game, texView);
            this.textures.push(tex);
        }
        console.log("Block textures", this.textures);
    }
}
