import BitStreamReader from '../../../game/BitStreamReader.js';
import { Reg as CPReg } from '../gl/gx/CP.js';
import DlistParser from '../gl/gx/DlistParser.js';
import RenderBatch from '../gl/gx/RenderBatch.js';
import GX from '../gl/gx/GX.js';
import { makeBox } from '../gl/GlUtil.js';
import { MAP_CELL_SIZE } from '../../../game/Game.js';

//struct types
let HitsBinEntry;

const LogRenderOps = false;
const ShaderFlags = {
    Hidden:             (1<< 1), //invisible, exploded walls, etc
    Fog:                (1<< 2), //enable fog
    CullBackface:       (1<< 3),
    ReflectSkyscape:    (1<< 5),
    Caustic:            (1<< 6),
    Lava:               (1<< 7),
    Reflective:         (1<< 8), //Occurs on Krazoa Palace reflective floors
    FuzzRelated:        (1<< 9),
    AlphaCompare:       (1<<10),
    TranspRelated2000:  (1<<13),
    ShortFur:           (1<<14), //4 layers
    MediumFur:          (1<<15), //8 layers
    LongFur:            (1<<16), //16 layers
    StreamingVideo:     (1<<17), //Occurs on video panels in Great Fox. Used to display preview video.
    IndoorOutdoorBlend: (1<<18), //Occurs near cave entrances and windows. Requires special handling for lighting.
    BlendFlag29:        (1<<29),
    ForceBlend:         (1<<30),
    Water:              (1<<31),
};

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

function _setShaderParams(gl, gx, cull, blendMode, sFactor, dFactor, logicOp,
compareEnable, compareFunc, updateEnable, alphaTest) {
    if(cull) gl.enable(gl.CULL_FACE); else gl.disable(gl.CULL_FACE);
    gx.setBlendMode(blendMode, sFactor, dFactor, logicOp);
    gx.setZMode(compareEnable, compareFunc, updateEnable);
    gx.setUseAlphaTest(alphaTest);
}

export default class BlockRenderer {
    /** Renders map blocks. */
    constructor(game, gx) {
        this.game = game;
        HitsBinEntry = game.app.types.getType('sfa.maps.HitsBinEntry');
        this.gx = gx;
        this.gl = gx.gl;
        this.dlistParser = new DlistParser(gx);
        this.reset();
    }

    reset() {
        this.pickerIds = {}; //id => list
        this._batches  = {};
    }

    parse(block, whichStream, params={}) {
        /** Parse the display lists.
         *  @param {Block} block The block to render.
         *  @param {string} whichStream One of 'main', 'water', 'reflective'
         *   specifying which bitstream to use.
         *  @param {object} params Render parameters.
         *  @returns {RenderBatch} Parsed render batch.
         */
        if(!block.load(this.gx)) return null;

        //check if we already parsed this
        const key = ([
            whichStream, block.x, block.z,
            params.isGrass ? 1 : 0,
            params.showHidden ? 1 : 0,
            params.isPicker ? 1 : 0,
            params.dlist,
        ]).join(',');
        if(this._batches[key]) return this._batches[key];

        //console.log("parsing", block, whichStream);
        this.curBatch = new RenderBatch(this.gx);
        this._batches[key] = this.curBatch;
        this._isDrawingForPicker = params.isPicker;

        const ops = new BitStreamReader(block.renderInstrs[whichStream]);
        this.curOps = ops;

        let done = false;
        this._setInitialGxParams();
        while(!done && !ops.isEof) {
            //this is similar but not identical to the render instructions
            //used for character models.
            const op = ops.read(4);
            switch(op) {
                case 1: this._renderOpTexture();   break;
                case 2: this._renderOpCallList();  break;
                case 3: this._renderOpSetVtxFmt(); break;
                case 0: //unused, but should be same as 4
                case 4: this._renderOpMatrix();    break;

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
        return this._batches[key];
    }

    render(block, whichStream, params={}) {
        /** Render the block.
         *  @param {Block} block The block to render.
         *  @param {string} whichStr{eam One of 'main', 'water', 'reflective'
         *   specifying which bitstream to use.
         *  @param {object} params Render parameters.
         *  @returns {RenderBatch} The render batch.
         */
        this.curShaderIdx = null;
        this.curBlock     = block;
        this.curStream    = whichStream;
        this.params       = params;
        const batch       = this.parse(block, whichStream, params);
        if(batch) {
            this.gx.executeBatch(batch);
            //this.gx.gl.flush();
        }
        return batch;
    }

    renderHits(block, params) {
        /** Render the block's hitboxes. */
        if(!block.hits) return null;

        const key = ([
            'hits', block.x, block.z, params.isPicker ? 1 : 0]).join(',');
        let batch = this._batches[key];
        if(batch) {
            this.gx.executeBatch(batch);
            return batch;
        }
        batch = new RenderBatch(this.gx);
        this._batches[key] = batch;

        const gx = this.gx;
        const gl = this.gx.gl;
        const offsX = block.x*MAP_CELL_SIZE;
        const offsY = 0; //block.header.yOffset;
        const offsZ = block.z*MAP_CELL_SIZE;

        if(params.isPicker) batch.addFunction(() => {
            //blend off, face culling off
            gx.disableTextures(GX.BlendMode.NONE, false);
            let mv = mat4.clone(gx.context.matModelView);
            mat4.translate(mv, mv, vec3.fromValues(offsX, offsY, offsZ));
            gx.setModelViewMtx(mv);
        });
        else batch.addFunction(() => {
            //blend on, face culling off
            gx.disableTextures(GX.BlendMode.BLEND, false);
            let mv = mat4.clone(gx.context.matModelView);
            mat4.translate(mv, mv, vec3.fromValues(offsX, offsY, offsZ));
            gx.setModelViewMtx(mv);
            //gl.enable(gl.POLYGON_OFFSET_FILL);
            //gl.polygonOffset(-10, 10);
            //XXX this still doesn't work perfectly
        });

        for(let i=0; i<block.hits.length; i++) {
            const hit = block.hits[i];
            let id = 0;
            if(this._isDrawingForPicker) {
                id = gx.addPickerObj({
                    type:   'blockHit',
                    idx:    i,
                    offset: block.hitsOffset + (i * HitsBinEntry.size),
                    block:  block,
                    hit:    hit,
                });
            }
            const d  = 0.1;
            const v1 = [hit.x1,hit.y1,hit.z1];
            const v2 = [hit.x2,hit.y2,hit.z2];
            //offset Y slightly so height isn't zero and so it
            //won't be hidden by the geometry
            if(v1[1] > v2[1]) { v1[1] += d; v2[1] -= d; }
            else { v1[1] -= d; v2[1] += d; }
            batch.addVertices(...makeBox(gl, v1, v2, id, [
                    (hit._0C & 0xF) << 4,
                    (hit._0D & 0xF) << 4,
                    hit.flags0E,
                    0xC0 ]));
        }

        batch.addFunction(() => {
            gl.disable(gl.POLYGON_OFFSET_FILL);
        })

        this.gx.executeBatch(batch);
        return batch;
    }

    _setInitialGxParams() {
        //set default vtx formats
        this.gx.cp.setReg(CPReg.ARRAY_STRIDE_VTXS,  6); //sizeof(vec3s)
        this.gx.cp.setReg(CPReg.ARRAY_STRIDE_COLOR, 2); //sizeof(u16)
        for(let i=0; i<8; i++) {
            this.gx.cp.setReg(CPReg.ARRAY_STRIDE_TEXCOORD+i, 4); //sizeof(vec2s)
            this.gx.cp.setVatFormat(i, vatDefaults[i]);
        }

        //set initial render modes (XXX verify)
        if(this._isDrawingForPicker) {
            this.curBatch.addFunction(() => {_setShaderParams(
                this.gx.gl, this.gx, true, //culling enabled
                GX.BlendMode.NONE, GX.BlendFactor.SRCALPHA,
                GX.BlendFactor.INVSRCALPHA, GX.LogicOp.NOOP,
                true, GX.CompareMode.LEQUAL, true, //depth test+update enabled
                true); //alpha test enabled
            });
        }
        else this.curBatch.addFunction(() => {_setShaderParams(
            this.gx.gl, this.gx, true, //culling enabled
            GX.BlendMode.BLEND, GX.BlendFactor.SRCALPHA,
            GX.BlendFactor.INVSRCALPHA, GX.LogicOp.NOOP,
            true, GX.CompareMode.LEQUAL, true, //depth test+update enabled
            true); //alpha test enabled
        });
    }

    _handleShaderFlags() {
        const gx    = this.gx;
        const gl    = this.gx.gl;
        const flags = this.curShader.flags;
        const cull  = flags & ShaderFlags.CullBackface;
        let blendMode, sFactor, dFactor, logicOp,
            compareEnable, compareFunc, updateEnable;

        let alphaTest = (flags & ShaderFlags.AlphaCompare) != 0;
        if(((flags & ShaderFlags.ForceBlend) == 0)
        && ((flags & ShaderFlags.BlendFlag29) == 0)) {
            if (((flags & ShaderFlags.AlphaCompare) == 0)
            || ((flags & ShaderFlags.Lava) != 0)) {
                blendMode     = GX.BlendMode.NONE;
                sFactor       = GX.BlendFactor.ONE;
                dFactor       = GX.BlendFactor.ZERO;
                logicOp       = GX.LogicOp.NOOP;
                compareEnable = true;
                compareFunc   = GX.CompareMode.LEQUAL;
                updateEnable  = true;
                //XXX
                //gx.setPeControl_ZCompLoc_(1);
                //gx.setTevAlphaIn(7,0,0,7,0); //stage, a, b, c, d
            }
            else {
                blendMode     = GX.BlendMode.NONE;
                sFactor       = GX.BlendFactor.ONE;
                dFactor       = GX.BlendFactor.ZERO;
                logicOp       = GX.LogicOp.NOOP;
                compareEnable = true;
                compareFunc   = GX.CompareMode.LEQUAL;
                updateEnable  = true;
                //gx.setPeControl_ZCompLoc_(0);
                //gx.setTevAlphaIn(4,0,0,4,0);
            }
        }
        else {
            blendMode     = GX.BlendMode.BLEND;
            sFactor       = GX.BlendFactor.SRCALPHA;
            dFactor       = GX.BlendFactor.INVSRCALPHA;
            logicOp       = GX.LogicOp.NOOP;
            compareEnable = true;
            compareFunc   = GX.CompareMode.LEQUAL;
            updateEnable  = false;
            //gx.setPeControl_ZCompLoc_(1);
            //gx.setTevAlphaIn(7,0,0,7,0);
        }

        //condense these into one function for hopefully better speed
        if(!this._isDrawingForPicker) {
            this.curBatch.addFunction(() => {
                _setShaderParams(gl, gx, cull, blendMode, sFactor, dFactor,
                    logicOp, compareEnable, compareFunc, updateEnable,
                    alphaTest);
            });
        }
    }

    _makeSetTextureCmd(params) {
        const gl = this.gl;
        return () => {
            for(let [slot, tex] of params) {
                //console.log("using texture", slot, tex);
                gl.activeTexture(gl.TEXTURE0 + slot);
                tex.bind();
                gl.uniform1i(this.gx.programInfo.uniforms.uSampler[slot], slot);
            }
        };
    }

    _renderOpTexture() {
        /** Select a texture and shader.
         *  This can affect how later commands are interpreted.
         */
        const gx  = this.gx;
        const gl  = this.gx.gl;
        const ops = this.curOps;
        const idx = ops.read(6);
        if(this.params.isGrass) return;
        if(this.curShaderIdx == idx) return;

        this.curShader = this.curBlock.shaders[idx];
        this.curShaderIdx = idx;
        if(LogRenderOps) {
            console.log("Select texture %d", idx, this.curShader);
            //console.log("Select texture %d: shader flags=%s", idx,
            //    this.curShader.attrFlags);
        }

        /*if(this._isDrawingForPicker) {
            this.curBatch.addFunction(() => {_setShaderParams(gl, gx,
                true, //cull backfaces
                GX.BlendMode.NONE, //blend mode
                GX.BlendFactor.ONE, //sFactor
                GX.BlendFactor.ZERO, //dFactor
                GX.LogicOp.NOOP, //logicOp
                true, //compareEnable
                GX.CompareMode.LEQUAL, //compareFunc
                true, //updateEnable
                true, //alphaTest
            )});
        }
        else*/ if(this.curShader) {
            this._handleShaderFlags();
        }
        else if(this.curStream == 'water'
        || this.curStream == 'reflective') { //XXX verify these
            this.curBatch.addFunction(() => {_setShaderParams(gl, gx,
                true, //cull backfaces
                GX.BlendMode.BLEND, //blend mode
                GX.BlendFactor.SRCALPHA, //sFactor
                GX.BlendFactor.INVSRCALPHA, //dFactor
                GX.LogicOp.NOOP, //logicOp
                true, //compareEnable
                GX.CompareMode.LEQUAL, //compareFunc
                false, //updateEnable
                true, //alphaTest
            )});
        }
        else {
            this.curBatch.addFunction(() => {_setShaderParams(gl, gx,
                true, //cull backfaces
                GX.BlendMode.NONE, //blend mode
                GX.BlendFactor.ONE, //sFactor
                GX.BlendFactor.ZERO, //dFactor
                GX.LogicOp.NOOP, //logicOp
                true, //compareEnable
                GX.CompareMode.LEQUAL, //compareFunc
                true, //updateEnable
                true, //alphaTest
            )});
        }

        //if(!this._isDrawingForPicker) { //select the textures
            const nLayers = this.curShader ? this.curShader.nLayers : 0;
            const params  = []; //batch these ops
            for(let i=0; i<gx.MAX_TEXTURES; i++) {
                let tex = gx.blankTexture;
                if(i < nLayers) {
                    const idx = this.curShader.layer[i].texture;
                    //console.log("select texture", idx, this.textures[idx]);
                    if(idx >= 0 && this.curBlock.textures[idx]) {
                        tex = this.curBlock.textures[idx];
                    }
                }
                params.push([i, tex]);
            }
            if(params.length > 0) {
                this.curBatch.addFunction(this._makeSetTextureCmd(params));
            }
        //}
    }

    _renderOpCallList() {
        /** Call one of the block's display lists.
         */
        const ops = this.curOps;
        const idx = ops.read(8);
        if(this.curBlock.dlists[idx] == undefined) {
            throw new Error(`Calling list ${idx} but max is ${this.curBlock.dlists.length}`);
        }

        if(this.params.isGrass) {
            if(this.curShader
            && (this.curShader.flags & ShaderFlags.TranspRelated2000)) return;
        }
        if(!this.params.showHidden) {
            //don't render hidden polys
            if(this.curShader
            && (this.curShader.flags & ShaderFlags.Hidden)) return;
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
        };
        if(LogRenderOps) console.log("Execute list", idx);

        let id = -1;
        if(this._isDrawingForPicker) {
            id = this.gx.addPickerObj({
                type:   'mapBlockDlist',
                block:  this.curBlock,
                list:   idx,
                stream: this.curStream,
                shader: this.curShader,
                params: this.params,
            });
        }
        const list = this.dlistParser.parse(
            this.curBlock.dlists[idx].data, dlistData, id);
        if(this.params.dlist < 0 || this.params.dlist == idx) {
            this.curBatch.addFunction(list);
        }
        if(LogRenderOps) {
            console.log("executed list", this.curBlock.dlists[idx].data);
        }
    }

    _renderOpSetVtxFmt() {
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
        if(this.params.isGrass) return;

        let TEX = [0, 0, 0, 0, 0, 0, 0, 0];
        if(this.curShader && !(this.curShader.flags & ShaderFlags.Water)) {
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

    _renderOpMatrix() {
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
