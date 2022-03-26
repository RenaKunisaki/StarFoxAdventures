import BitStreamReader from '../BitStreamReader.js';

export default class BlockRenderer {
    /** Renders map blocks. */
    constructor(gx) {
        this.gx = gx;
        this.gl = gx.gl;
    }

    render(block, whichStream) {
        /** Render the block.
         *  @param {Block} block The block to render.
         *  @param {string} whichStream One of 'main', 'reflective', 'water',
         *   telling which render instruction stream to use.
         */
        this.curBlock = block;
        const ops = new BitStreamReader(this.curBlock.renderInstrs[whichStream]);
        this.curOps = ops;
        let done = false;
        while(!done && !ops.isEof) {
            //this is similar but not identical to the render instructions
            //used for character models.
            const op = ops.read(4);
            switch(op) {
                case 1: this._renderOpTexture();   break;
                case 2: this._renderOpCallList();  break;
                case 3: this._renderOpSetVtxFmt(whichStream); break;
                case 0: //unused, but should be same as 4
                case 4: this._renderOpMatrix();    break;

                case null: //reached end of stream
                    console.error("Premature end of stream at bit 0x%s",
                        ops.offset.toString(16));
                case 5: //end
                    //console.log("Done rendering");
                    done = true;
                    break;

                default:
                    console.error("Unknown render op %d at bit 0x%s", ops,
                        (ops.offset-4).toString(16));
            }
        }
    }

    _renderOpTexture() {
        /** Select a texture and shader.
         *  This can affect how later commands are interpreted.
         */
        const gx  = this.gx;
        const gl  = this.gx.gl;
        const ops = this.curOps;
        const idx = ops.read(6);
        this.curShader = this.curBlock.shaders[idx];
        //this.curTexture = this.curBlock.textures[this.curShader.layer[0].texture];
        this.curShaderIdx = idx;
        console.log("Select texture %d", idx, this.curShader);
        //console.log("Select texture %d: shader flags=%s", idx,
        //    this.curShader.attrFlags);

        if(this.curShader) {
            if(this.curShader.flags & 8) gl.enable(gl.CULL_FACE);
            else gl.disable(gl.CULL_FACE);
        }

        const nLayers = this.curShader ? this.curShader.nLayers : 0;
        for(let i=0; i<gx.MAX_TEXTURES; i++) {
            let tex = gx.blankTexture;
            if(i < nLayers) {
                const idx = this.curShader.layer[i].texture;
                if(idx >= 0 && this.curBlock.textures[idx]) {
                    tex = this.curBlock.textures[idx];
                }
            }
            gl.activeTexture(gl.TEXTURE0 + i);
            tex.bind();
            gl.uniform1i(gx.programInfo.uniforms.uSampler[i], i);
        }
    }

    _renderOpCallList() {
        /** Call one of the block's display lists.
         */
        const ops = this.curOps;
        const idx = ops.read(8);
        if(this.curBlock.dlists[idx] == undefined) {
            throw new Error(`Calling list ${idx} but max is ${this.curBlock.dlists.length}`);
        }
        const dlistData = {
            POS:  this.curBlock.vtxPositions,
            //NRM:  this.curBlock.normals,
            COL0: this.curBlock.vtxColors,
            TEX0: this.curBlock.texCoords,
            TEX1: this.curBlock.texCoords,
            //TEX2: this.curBlock.texCoords,
            //TEX3: this.curBlock.texCoords,
            //TEX4: this.curBlock.texCoords,
            //TEX5: this.curBlock.texCoords,
            //TEX6: this.curBlock.texCoords,
            //TEX7: this.curBlock.texCoords,
            _debug: {
                shader:    this.curShader,
                shaderIdx: this.curShaderIdx,
                textureIdx:[
                    this.curShader ? this.curShader.layer[0].texture : null,
                    this.curShader ? this.curShader.layer[1].texture : null,
                ],
            },
        };
        console.log("Execute list", idx);
        this.gx.executeDisplayList(this.curBlock.dlists[idx].data, dlistData);
    }

    _renderOpSetVtxFmt(whichStream) {
        /** Change the vertex data format.
         */
        const INDEX8 = 2, INDEX16 = 3;
        const ops   = this.curOps;
        let posSize = ops.read(1) ? INDEX16 : INDEX8;
        //for character models, 1 bit for normals here.
        //map blocks don't have normals.
        let colSize = ops.read(1) ? INDEX16 : INDEX8;
        let texSize = ops.read(1) ? INDEX16 : INDEX8;

        let TEX = [0, 0, 0, 0, 0, 0, 0, 0];
        if(this.curShader && !(this.curShader.flags & 0x80000000)) {
            for(let i=0; i<this.curShader.nLayers; i++) {
                TEX[i] = texSize;
            }
        }
        else TEX[0] = texSize;

        console.log("Set vfmt: pos=%d col=%d tex=%d", posSize, colSize, texSize,
            whichStream);

        let PNMTXIDX = 0;
        let POS      = posSize;
        let COL      = [colSize, 0];

        //XXX this can't be right. how does the game decide which VAT to set?
        //is it just always 5?
        for(let i=0; i<8; i++) {
            if(whichStream != 'main') { //dunno why the game does this
                this.gx.cp.setReg(0x50+i, //VCD FMT LO
                    (POS <<  9) | (COL[0] << 13) | (COL[1] << 15));
                this.gx.cp.setReg(0x60+i, //VCD FMT HI
                    TEX[0] | (TEX[1] <<  2) | (TEX[2] <<  4) | (TEX[3] <<  6) |
                    (TEX[4] <<  8) | (TEX[5] << 10) | (TEX[6] << 12) | (TEX[7] << 14));
            }
            else {
                //XXX move this and find correct values
                this.gx.cp.setReg(0x50+i, //VCD FMT LO
                    (2 <<  9) | (2 << 13) | (0 << 15));
                this.gx.cp.setReg(0x60+i, //VCD FMT HI
                    2);
            }
            this.gx.cp.setReg(0x70+i, //VAT_A
                (1 << 30) | //BYTEDEQUANT
                //(0 << 14) | //COL0FMT (RGB565)
                (3 << 14) | //COL0FMT (RGBA4444)
                (3 << 1) | //POSFMT (s16)
                1); //POSCNT (3 values)
            //shouldn't need to touch B or C
        }
    }

    _renderOpMatrix() {
        /** Load one of the block's matrices into GX XF registers.
         */
        const ops   = this.curOps;
        const count = ops.read(4);
        const idxs  = []; //debug
        console.log("init %d mtxs", count);

        let iVar3 = 0;
        if(count > 8) {
            let n = (count - 1) >> 3;
            let mtxData = ops.read(0x40 * n);
            iVar3 = n * 8;
        }
        let iVar2 = count - iVar3;
        if(iVar3 < count) {
            ops.read(8 * iVar2); //XXX what is this?
        }
    }
}
