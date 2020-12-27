import VertexBuffer from '/js/gl/gx/VertexBuffer.js';
import {hexdump} from '/js/Util.js';

export default class ModelRenderer {
    /** Renders an SFA Model.
     */
    constructor(gx, model=null) {
        this.gx    = gx;
        this.gl    = gx.gl;
        this.model = model;
        this.reset();
    }

    reset() {
        /** Reset all state to default.
         */
        this.curTexture = null;
        this.curShader  = null;
        this._didRender = false;
    }

    _setupToRender() {
        /** Set up our GX instance to the state it should be in when it's
         *  about to render the model.
         */
        const cp = this.gx.cp;
        //set up GX for the model format.
        //most of this is just extracted from the game code and/or guessed.

        const vatA = [
            0x4ee16007, 0x41216027, 0x41217009, 0x54e0e687,
            0x4ee17009, 0x50e0e437, 0x54e0e487, 0x54e0e407,
        ];
        const vatB = [
            0x80000000, 0x80000000, 0x80000009, 0x829d4ea7,
            0x80000000, 0x821d0e87, 0x829d4ea7, 0x829d4ea7,
        ];

        //80049e48
        for(let i=0;i<8; i++) {
            cp.setReg(0x70+i, vatA[i]);
            cp.setReg(0x80+i, vatB[i]);
            cp.setReg(0x90+i, 0);
            //XXX is this right?
            //the game just sets a field in its GX state struct, and it's
            //not clear wihch VCDs it applies to.
            cp.setReg(0x50+i, 0x00002201);
            cp.setReg(0x60+i, 0x00000001);
        }
        //gxSetBackfaceCulling(0);
        //gxSetClearColorAndDepth(&color,0xffffff);
        //gxSetBlendMode(GX_BM_NONE,GX_BL_ONE,GX_BL_ZERO,GX_LO_NOOP);

        /* for(let i=0; i<8; i++) {
            cp.setReg(0x50+i, 0x1F03); //VCD_LO
            cp.setReg(0x60+i, 3); //VCD_HI (texcoords)
            cp.setReg(0x70+i, //VCD_A
                ( 1 <<  0) | //POSCNT:    1 (x,y,z)
                ( 3 <<  1) | //POSFMT:    3 (s16)
                ( 8 <<  4) | //POSSHFT:   8 (divide by 256)
                ( 0 <<  9) | //NRMCNT:    0 (one normal, not three)
                ( 3 << 10) | //NRMFMT:    3 (s16)
                ( 1 << 21) | //TEX0CNT:   1 (s,t)
                ( 3 << 22) | //TEX0FMT:   3 (s16)
                (10 << 25) | //TEX0SHFT: 10 (divide by 1024)
                0); //XXX more...
            cp.setReg(0x80+i, //CP_VAT_REG_B
                ( 1 <<  0) | //TEX1CNT
                ( 3 <<  1) | //TEX1FMT
                (10 <<  4) | //TEX1SHFT
                ( 1 << 31)); //VCACHE_ENHANCE (must be 1)
            cp.setReg(0x90+i, 0); //VCD_C (TEX4-7)
        } */
        cp.setReg(0xB0, 6); //Stride POS
        cp.setReg(0xB1, this.model.header.flags24 & 0x8 ? 9 : 3); //Stride NRM
        cp.setReg(0xB2, 2); //Stride COL0
        //XXX B3? (COL1)
        cp.setReg(0xB4, 4); //Stride TEX0
        cp.setReg(0xB5, 4); //Stride TEX1

        //XXX more...

        //XXX init mtxs.
        //8003f8ec - modelGetBoneMatrix() uses idx 0 if no bones.
        //mtx0 is copied to XF.
        //else, all mtxs are copied to XF (bones+vgroups)
        //this only seems to apply for models that use the alt render ops
        //though, which I suspect is non-animated models.
        //otherwise something similar is done, not sure exactly but I think
        //it's copying all the bone mtxs?

        //this doesn't seem to make any difference.
        for(let i=0; i<this.model.header.nBones+this.model.header.nVtxGroups; i++) {
            //convert xlate to actual matrix.
            let mtx = mat4.create();
            mat4.translate(mtx, // destination matrix
                mat4.create(),  // matrix to translate
                this.model.xlates[i]);  // amount to translate
            //console.log("Init mtx %d to", dstIdx, mtx);
            this.gx.xf.setMtx(i*3, mtx);
        }
    }

    renderBones() {
        /** Render the model's bones. Mainly for debug.
         *  Renders head in red, tail in white (or maybe pale?).
         */
        const gl     = this.gl;
        const vtxBuf = new VertexBuffer(this.gx);
        for(let i=0; i<this.model.header.nBones; i++) {
            const bone = this.model.bones[i];
            const [head, tail] = this.model.calcBonePos(bone, true);
            vtxBuf.addVtx({POS:head, COL0:[255,   0,   0, 255]});
            vtxBuf.addVtx({POS:tail, COL0:[255, 255, 255, 255]});
        }
        vtxBuf.bind();
        this.gx.program.use();
        this.gx._setShaderMtxs();
        gl.drawArrays(gl.LINES, 0, this.model.header.nBones*2);
    }

    render() {
        /** Render the model.
         */
        //if(this._didRender) return;
        const gx=this.gx, gl=this.gl, model=this.model;
        this.gx.reset();
        this._setupToRender();

        if(model.header.flags & 0x8000) {
            alert("Model uses alt render ops");
            return;
            /* these appear to be just:
            - 4 bits unused
            - 1 bit POS size
            - 1 bit TEX0 size
            - 4 bits unused
            - 4 bits count
            - 8*count bits bone idx
              - copy the given bone idx to XF mtxs
            - 4 bits unused
            - 8 bits dlist idx (not 8*count)
              - call this list (just one)
            it's possible some of the unused fields don't exist. I didn't read
            carefully enough. 8003f7f4
            */
        }

        //parse render ops
        const ops = model.renderOps;
        ops.seek(0);
        let done = false;
        while(!done) {
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
                    console.log("Done rendering");
                    done = true;
                    break;

                default:
                    console.error("Unknown render op %d at bit 0x%s", op,
                        (ops.offset-4).toString(16));
            }
        }
        this._didRender = true;
    }

    _renderOpTexture() {
        /** Select a texture and shader.
         *  This can affect how later commands are interpreted.
         */
        const gl  = this.gl;
        const ops = this.model.renderOps;
        const idx = ops.read(6);
        this.curShader = this.model.shaders[idx];
        //this.curTexture = this.model.textures[this.curShader.layer[0].texture];
        this.curShaderIdx = idx;
        //console.log("Select texture %d: shader flags=%s", idx,
        //    this.curShader.attrFlags);

        if(this.curShader) {
            if(this.curShader.flags & 8) gl.enable(gl.CULL_FACE);
            else gl.disable(gl.CULL_FACE);
            //XXX other flags: 8003f5ac
        }

        const nLayers = this.curShader ? this.curShader.nLayers : 0;
        for(let i=0; i<this.gx.MAX_TEXTURES; i++) {
            let tex = this.gx.blankTexture;
            if(i < nLayers) {
                const idx = this.curShader.layer[i].texture;
                if(idx >= 0 && this.model.textures[idx]) {
                    tex = this.model.textures[idx];
                }
            }
            gl.activeTexture(gl.TEXTURE0 + i);
            tex.bind();
            gl.uniform1i(this.gx.programInfo.uniforms.uSampler[i], i);
        }
    }

    _renderOpCallList() {
        /** Call one of the model's display lists.
         */
        const ops = this.model.renderOps;
        const idx = ops.read(8);
        const dlistData = {
            POS:  this.model.rawVtxs,
            NRM:  this.model.rawNormals,
            TEX0: this.model.rawTexCoords,
            TEX1: this.model.rawTexCoords,
            TEX2: this.model.rawTexCoords,
            TEX3: this.model.rawTexCoords,
            TEX4: this.model.rawTexCoords,
            TEX5: this.model.rawTexCoords,
            TEX6: this.model.rawTexCoords,
            TEX7: this.model.rawTexCoords,
            _debug: {
                shader:    this.curShader,
                shaderIdx: this.curShaderIdx,
                textureIdx:[
                    this.curShader ? this.curShader.layer[0].texture : null,
                    this.curShader ? this.curShader.layer[1].texture : null,
                ],
            },
        };
        //console.log("Execute list", idx);
        this.gx.executeDisplayList(this.model.dlists[idx], dlistData);
    }

    _renderOpSetVtxFmt() {
        /** Change the vertex data format.
         */
        const ops   = this.model.renderOps;
        const flags = this.curShader ? this.curShader.attrFlags : 0;
        let posSize = ops.read(1) ? 3 : 2;
        let nrmSize = (flags & 1) ? (ops.read(1) ? 3 : 2) : 0; //has normals?
        let colSize = (flags & 2) ? (ops.read(1) ? 3 : 2) : 0; //has colors?
        let texSize = ops.read(1) ? 3 : 2;
        //console.log("Set vfmt: pos=%d nrm=%d col=%d tex=%d flags=0x%s",
        //    posSize, nrmSize, colSize, texSize, flags.toString(16));

        let PNMTXIDX = 0;
        let POS      = posSize;
        let NRM      = nrmSize;
        let COL      = [colSize, 0];
        let TMIDX    = [0, 0, 0, 0, 0, 0, 0, 0];
        let TEX      = [texSize, 0, 0, 0, 0, 0, 0, 0];
        if(this.model.header.nBones >= 2) { //8003e638
            PNMTXIDX = 1;
            if(this.curShader && (this.curShader.unk00 || this.curShader.unk04)) {
                //XXX if(shader has texture or lighting)
                //console.log("Shader auxTex2=", this.curShader.auxTex2);
                TMIDX[0] = 1;
                if(this.curShader.auxTex2 > 0) {
                    TMIDX[1] = 1;
                    TMIDX[2] = 1;
                }
                //this is slightly different from what the game code does but the
                //result is the same; setting idx 0 or 2 to 1 depending on auxTex2
            }
            for(let i=0; i<this.model.header.nTexMtxs; i++) {
                //XXX not entirely sure how this works
                //this is probably right since anything else fails to parse
                TMIDX[7-i] = 1;
            }
            if(this.curShader) {
                for(let i=0; i<this.curShader.nLayers; i++) {
                    TEX[i] = texSize;
                    //don't set TMIDX here. this is directly copied from game code.
                }
            }
        }

        //XXX this can't be right. how does the game decide which VAT to set?
        for(let i=0; i<8; i++) {
            this.gx.cp.setReg(0x50+i, //VCD FMT LO
                PNMTXIDX |
                (TMIDX[0] <<  1) | (TMIDX[1] <<  2) | (TMIDX[2] <<  3) |
                (TMIDX[3] <<  4) | (TMIDX[4] <<  5) | (TMIDX[5] <<  6) |
                (TMIDX[6] <<  7) | (TMIDX[7] <<  8) | (POS <<  9) | (NRM << 11) |
                (COL[0] << 13) | (COL[1] << 15));
            this.gx.cp.setReg(0x60+i, //VCD FMT HI
                TEX[0] | (TEX[1] <<  2) | (TEX[2] <<  4) | (TEX[3] <<  6) |
                (TEX[4] <<  8) | (TEX[5] << 10) | (TEX[6] << 12) | (TEX[7] << 14));

            let VATC = this.gx.cp.getReg(0x70+i & 0x7FFFFDFF);
            if(this.model.header.flags24 & 0x08) { //use 9 normals
                VATC |= 0x80000200;
            }
            this.gx.cp.setReg(0x70+i, VATC);
        }
    }

    _renderOpMatrix() {
        /** Load one of the model's matrices into GX XF registers.
         *  Matrices come from the bones and vertex groups.
         */
        //XXX I think this works differently if the model has no bones.
        const lut = [ //no idea why the game does this.
             0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 0,
            10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 0, 0];
        const ops   = this.model.renderOps;
        const count = ops.read(4);
        const idxs  = []; //debug
        for(let i=0; i<count; i++) {
            const srcIdx = ops.read(8);
            const dstIdx = lut[i];
            idxs.push(srcIdx);
            if(this.model.xlates[srcIdx] == undefined) {
                console.error("Using matrix %d, max is %d (dest %d idx %d)",
                    srcIdx, this.model.header.nVtxGroups, dstIdx, i);
            }
            else {
                //convert xlate to actual matrix.
                let mtx = mat4.create();
                mat4.translate(mtx, // destination matrix
                    mat4.create(),  // matrix to translate
                    this.model.xlates[srcIdx]);  // amount to translate
                //console.log("Init mtx %d to", dstIdx, mtx);
                this.gx.xf.setMtx(dstIdx*3, mtx);
                //XXX this can set texture mtx and normals too, see 8003e3a0
                /* dstIdx = eg 0, 1, 2, 3...
                 * mtx = eg [1 0 0 0]
                 *          [0 1 0 0]
                 *          [0 0 1 0]
                 *          [x y z 1]
                 */
            }
        }
        //console.log("Init mtxs:", idxs);
    }
}
