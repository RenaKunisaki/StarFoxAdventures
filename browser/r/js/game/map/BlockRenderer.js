import BitStreamReader from '../BitStreamReader.js';
import { Reg as CPReg } from '../../app/ui/gl/gx/CP.js';
import DlistParser from '../../app/ui/gl/gx/DlistParser.js';
import RenderBatch from '../../app/ui/gl/gx/RenderBatch.js';
import Texture from '../../app/ui/gl/Texture.js'

const LogRenderOps = false;

//from noclip
/* export enum ShaderAttrFlags {
    NRM = 0x1,
    CLR = 0x2,
}
export const enum NormalFlags {
    HasVertexColor = 0x2,
    NBT = 0x8,
    HasVertexAlpha = 0x10,
}

export const enum LightFlags {
    OverrideLighting = 0x2,
}
export enum ShaderFlags {
    DevGeometry = 0x2,
    Fog = 0x4,
    CullBackface = 0x8,
    ReflectSkyscape = 0x20, // ???
    Caustic = 0x40,
    Lava = 0x80,
    Reflective = 0x100, // Occurs on Krazoa Palace reflective floors
    AlphaCompare = 0x400,
    ShortFur = 0x4000, // 4 layers
    MediumFur = 0x8000, // 8 layers
    LongFur = 0x10000, // 16 layers
    StreamingVideo = 0x20000, // Occurs on video panels in Great Fox. Used to display preview video.
    IndoorOutdoorBlend = 0x40000, // Occurs near cave entrances and windows. Requires special handling for lighting.
    Water = 0x80000000,
} */

const vatDefaults = [
    //these are set in videoInit() and almost never change
    { //VAT 0
        POSCNT:  1, POSFMT:  3, POSSHFT:   0, //s16
        COL0CNT: 1, COL0FMT: 5, COL0SHFT:  0, //rgba8888
        TEX0CNT: 1, TEX0FMT: 3, TEX0SHFT:  7, //s16
    },
    { //VAT 1
        POSCNT:  1, POSFMT:  3, POSSHFT:   2, //s16
        COL0CNT: 1, COL0FMT: 5, COL0SHFT:  0, //rgba8888
        TEX0CNT: 1, TEX0FMT: 4, TEX0SHFT:  0, //float
    },
    { //VAT 2
        POSCNT:  1, POSFMT:  4, POSSHFT:   0, //float
        NRMCNT:  0, NRMFMT:  4, NRMSHFT:   0, //float
        COL0CNT: 1, COL0FMT: 5, COL0SHFT:  0, //rgba8888
        TEX0CNT: 1, TEX0FMT: 4, TEX0SHFT:  0, //float
        TEX1CNT: 1, TEX1FMT: 4, TEX1SHFT:  0, //float
    },
    { //VAT 3
        POSCNT:  1, POSFMT:  3, POSSHFT:   8, // s16
        NRM3CNT: 1, NRM3FMT: 1, NRM3SHFT:  0, // s8
        COL0CNT: 1, COL0FMT: 3, COL0SHFT:  0, // rgba4444
        TEX0CNT: 1, TEX0FMT: 3, TEX0SHFT: 10, // s16
        TEX1CNT: 1, TEX1FMT: 3, TEX1SHFT: 10, // s16
        TEX2CNT: 1, TEX2FMT: 3, TEX2SHFT: 10, // s16
        TEX3CNT: 1, TEX3FMT: 3, TEX3SHFT: 10, // s16
    },
    { //VAT 4
        POSCNT:  1, POSFMT:  4, POSSHFT:   0, //float
        COL0CNT: 1, COL0FMT: 5, COL0SHFT:  0, //rgba8888
        TEX0CNT: 1, TEX0FMT: 3, TEX0SHFT:  7, //s16
        NRMCNT:  0, NRMFMT:  4, NRMSHFT:   0, //float
    },
    { //VAT 5
        POSCNT:  1, POSFMT:  3, POSSHFT:   3, //s16
        NRMCNT:  0, NRMFMT:  1, NRMSHFT:   0, //s8
        COL0CNT: 1, COL0FMT: 3, COL0SHFT:  0, //rgba4444
        TEX0CNT: 1, TEX0FMT: 3, TEX0SHFT:  8, //s16
        TEX1CNT: 1, TEX1FMT: 3, TEX1SHFT:  8, //s16
        TEX2CNT: 1, TEX2FMT: 3, TEX2SHFT:  8, //s16
        TEX3CNT: 1, TEX3FMT: 3, TEX3SHFT:  8, //s16
    },
    { //VAT 6
        POSCNT:  1, POSFMT:  3, POSSHFT:   8, //s16
        NRMCNT:  0, NRMFMT:  1, NRMSHFT:   0, //s8
        COL0CNT: 1, COL0FMT: 3, COL0SHFT:  0, //rgba4444
        TEX0CNT: 1, TEX0FMT: 3, TEX0SHFT: 10, //s16
        TEX1CNT: 1, TEX1FMT: 3, TEX1SHFT: 10, //s16
        TEX2CNT: 1, TEX2FMT: 3, TEX2SHFT: 10, //s16
        TEX3CNT: 1, TEX3FMT: 3, TEX3SHFT: 10, //s16
    },
    { //VAT 7
        POSCNT:  1, POSFMT:  3, POSSHFT:   0, //s16
        NRMCNT:  0, NRMFMT:  1, NRMSHFT:   0, //s8
        COL0CNT: 1, COL0FMT: 3, COL0SHFT:  0, //rgba4444
        TEX0CNT: 1, TEX0FMT: 3, TEX0SHFT: 10, //s16
        TEX1CNT: 1, TEX1FMT: 3, TEX1SHFT: 10, //s16
        TEX2CNT: 1, TEX2FMT: 3, TEX2SHFT: 10, //s16
        TEX3CNT: 1, TEX3FMT: 3, TEX3SHFT: 10, //s16
    },
];

export default class BlockRenderer {
    /** Renders map blocks. */
    constructor(gx) {
        this.gx = gx;
        this.gl = gx.gl;
        this.dlistParser = new DlistParser(gx);
        this.textures = [];
    }

    parse(block, whichStream, params={}) {
        /** Parse the display lists.
         *  @param {Block} block The block to render.
         *  @param {string} whichStream One of 'main', 'water', 'reflective'
         *   specifying which bitstream to use.
         *  @param {object} params Render parameters.
         */
        if(!block.load()) return;
        const key = `${whichStream},${params.isGrass}`;
        if(block.batchOps[key]) return block.batchOps[key];

        //convert the textures
        for(let gTex of block.textures) {
            const tex = new Texture(this.gx.context);
            tex.loadGameTexture(gTex);
            //tex.loadFromImage('/r/f-texture.png');
            this.textures.push(tex);
        }

        this.curBatch = new RenderBatch(this.gx);
        block.batchOps[key] = this.curBatch;

        const ops = new BitStreamReader(block.renderInstrs[whichStream]);
        this.curOps = ops;

        //set initial GX params
        this._setDefaultVtxFmt();

        const isGrass = params.isGrass;
        let done = false;
        while(!done && !ops.isEof) {
            //this is similar but not identical to the render instructions
            //used for character models.
            const op = ops.read(4);
            switch(op) {
                case 1: this._renderOpTexture(isGrass);   break;
                case 2: this._renderOpCallList(isGrass);  break;
                case 3: this._renderOpSetVtxFmt(isGrass); break;
                case 0: //unused, but should be same as 4
                case 4: this._renderOpMatrix(isGrass);    break;

                case null: //reached end of stream
                    console.error("Premature end of stream at bit 0x%s",
                        ops.offset.toString(16));
                case 5: //end
                    //console.log("Done rendering", whichStream);
                    done = true;
                    break;

                default:
                    console.error("Unknown render op %d at bit 0x%s", op,
                        (ops.offset-4).toString(16));
            }
        }
        return block.batchOps[key];
    }

    render(block, whichStream, params={}) {
        /** Render the block.
         *  @param {Block} block The block to render.
         *  @param {string} whichStream One of 'main', 'water', 'reflective'
         *   specifying which bitstream to use.
         *  @param {object} params Render parameters.
         *  @returns {RenderBatch} The render batch.
         */
        this.curBlock = block;
        this.params   = params;
        const batch = this.parse(block, whichStream, params);
        if(!batch) return;
        this.gx.executeBatch(batch);
        this.gx.gl.flush();
        return batch;
    }

    _renderOpTexture(isGrass) {
        /** Select a texture and shader.
         *  This can affect how later commands are interpreted.
         */
        const gx  = this.gx;
        const gl  = this.gx.gl;
        const ops = this.curOps;
        const idx = ops.read(6);
        if(isGrass) return;

        this.curShader = this.curBlock.shaders[idx];
        //this.curTexture = this.curBlock.textures[this.curShader.layer[0].texture];
        this.curShaderIdx = idx;
        if(LogRenderOps) {
            console.log("Select texture %d", idx, this.curShader);
            //console.log("Select texture %d: shader flags=%s", idx,
            //    this.curShader.attrFlags);
        }

        if(this.curShader) {
            if(this.curShader.flags & 8) {
                this.curBatch.addFunction(() => gl.enable(gl.CULL_FACE));
            }
            else this.curBatch.addFunction(() => gl.disable(gl.CULL_FACE));
        }

        const nLayers = this.curShader ? this.curShader.nLayers : 0;
        for(let i=0; i<gx.MAX_TEXTURES; i++) {
            let tex = gx.blankTexture;
            if(i < nLayers) {
                const idx = this.curShader.layer[i].texture;
                //console.log("select texture", idx, this.textures[idx]);
                if(idx >= 0 && this.textures[idx]) {
                    tex = this.textures[idx];
                }
            }
            this.curBatch.addFunction(this._makeSetTextureCmd(i, tex));
        }
    }

    _makeSetTextureCmd(slot, tex) {
        const gl = this.gl;
        return () => {
            //console.log("using texture", slot, tex);
            gl.activeTexture(gl.TEXTURE0 + slot);
            tex.bind();
            gl.uniform1i(this.gx.programInfo.uniforms.uSampler[slot], slot);
        };
    }

    _renderOpCallList(isGrass) {
        /** Call one of the block's display lists.
         */
        const ops = this.curOps;
        const idx = ops.read(8);
        if(this.curBlock.dlists[idx] == undefined) {
            throw new Error(`Calling list ${idx} but max is ${this.curBlock.dlists.length}`);
        }

        if(isGrass) {
            if(this.curShader && (this.curShader.flags & 0x2000)) return;
        }
        if(!this.params.showHidden) {
            //don't render hidden polys
            if(this.curShader && (this.curShader.flags & 2)) return;
        }

        const dlistData = {
            POS:  this.curBlock.vtxPositions,
            //NRM:  this.curBlock.normals, //map blocks don't have normals
            COL0: this.curBlock.vtxColors,
            TEX0: this.curBlock.texCoords,
            TEX1: this.curBlock.texCoords,
            TEX2: this.curBlock.texCoords,
            TEX3: this.curBlock.texCoords,
            TEX4: this.curBlock.texCoords,
            TEX5: this.curBlock.texCoords,
            TEX6: this.curBlock.texCoords,
            TEX7: this.curBlock.texCoords,
            /* _debug: {
                shader:    this.curShader,
                shaderIdx: this.curShaderIdx,
                textureIdx:[
                    this.curShader ? this.curShader.layer[0].texture : null,
                    this.curShader ? this.curShader.layer[1].texture : null,
                ],
            }, */
        };
        if(LogRenderOps) console.log("Execute list", idx);
        //this.gx.executeDisplayList(this.curBlock.dlists[idx].data, dlistData);
        this.curBatch.addFunction(
            this.dlistParser.parse(this.curBlock.dlists[idx].data, dlistData));
        if(LogRenderOps) {
            console.log("executed list", this.curBlock.dlists[idx].data);
        }
    }

    _setDefaultVtxFmt() {
        this.gx.cp.setReg(CPReg.ARRAY_STRIDE_VTXS,  6); //sizeof(vec3s)
        this.gx.cp.setReg(CPReg.ARRAY_STRIDE_COLOR, 2); //sizeof(u16)
        for(let i=0; i<8; i++) {
            this.gx.cp.setReg(CPReg.ARRAY_STRIDE_TEXCOORD+i, 4); //sizeof(vec2s)
            this.gx.cp.setVatFormat(i, vatDefaults[i]);
        }
    }

    _renderOpSetVtxFmt(isGrass) {
        /** Change the vertex data format.
         */
        const INDEX8 = 2, INDEX16 = 3;
        const ops    = this.curOps;
        let posSize  = ops.read(1) ? INDEX16 : INDEX8;
        let colSize=0, nrmSize=0;
        //only for character models; maps don't have normals
        //if((!this.curShader) || this.curShader.attrFlags & 1) {
        //    nrmSize = ops.read(1) ? INDEX16 : INDEX8;
        //}
        if((!this.curShader) || (this.curShader.attrFlags & 2)) {
            colSize = ops.read(1) ? INDEX16 : INDEX8;
        }
        let texSize  = ops.read(1) ? INDEX16 : INDEX8;
        if(isGrass) return;

        let TEX = [0, 0, 0, 0, 0, 0, 0, 0];
        if(this.curShader && !(this.curShader.flags & 0x80000000)) {
            for(let i=0; i<this.curShader.nLayers; i++) {
                TEX[i] = texSize;
            }
        }
        else TEX[0] = texSize;

        if(LogRenderOps) {
            console.log("Set vfmt: pos=%d col=%d tex=%d", posSize, colSize,
                texSize);
        }

        let PNMTXIDX = 0;
        let POS      = posSize;
        let NRM      = nrmSize;
        let COL      = [colSize, 0];

        this.gx.cp.setReg(0x55, //VCD FMT LO (VAT 5)
            (NRM << 11) | (POS <<  9) | (COL[0] << 13) | (COL[1] << 15));
        this.gx.cp.setReg(0x65, //VCD FMT HI (VAT 5)
            TEX[0] | (TEX[1] <<  2) | (TEX[2] <<  4) | (TEX[3] <<  6) |
            (TEX[4] <<  8) | (TEX[5] << 10) | (TEX[6] << 12) | (TEX[7] << 14));
    }

    _renderOpMatrix(isGrass) {
        /** Load one of the block's matrices into GX XF registers.
         */
        const ops   = this.curOps;
        const count = ops.read(4);
        const mtxs  = [];
        for(let i=0; i<count; i++) {
            //can't read more than 24 bits at once
            mtxs.push(ops.read(8)); //idxs into mtxs (XXX where are they?)
        }
        //XXX which XF reg do we write to?
        if(LogRenderOps) console.log("init %d mtxs", count, mtxs);
    }
}
