export default class BlockRenderer {
    /** Renders one map block.
     */
    constructor(mapRenderer) {
        this.mapRenderer = mapRenderer;
    }

    renderBlock(block, opsIdx) {
        /** Render a MapBlock.
         *  @param block the block.
         *  @param opsIdx which set of render ops to use (0, 1, 2).
         */
        this.gl = this.mapRenderer.gl;
        this.gx = this.mapRenderer.gx;
        this.curBlock = block;
        const ops = block.renderOps[opsIdx];
        this.curOps = ops;
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
                    //console.log("Done rendering");
                    done = true;
                    break;

                default:
                    console.error("Unknown render op %d at bit 0x%s", op,
                        (ops.offset-4).toString(16));
            }
        }
    }

    _renderOpTexture() {
        /** Select a texture and shader.
         *  This can affect how later commands are interpreted.
         */
        const gl  = this.gl;
        const ops = this.curOps;
        const idx = ops.read(6);
        this.curShader = this.curBlock.shaders[idx];
        //this.curTexture = this.curBlock.textures[this.curShader.layer[0].texture];
        this.curShaderIdx = idx;
        //console.log("Select texture %d", idx, this.curShader);
        //console.log("Select texture %d: shader flags=%s", idx,
        //    this.curShader.attrFlags);

        if(this.curShader) {
            if(this.curShader.flags & 8) gl.enable(gl.CULL_FACE);
            else gl.disable(gl.CULL_FACE);
        }

        const nLayers = this.curShader ? this.curShader.nLayers : 0;
        for(let i=0; i<this.gx.MAX_TEXTURES; i++) {
            let tex = this.gx.blankTexture;
            if(i < nLayers) {
                const idx = this.curShader.layer[i].texture;
                if(idx >= 0 && this.curBlock.textures[idx]) {
                    tex = this.curBlock.textures[idx];
                }
            }
            gl.activeTexture(gl.TEXTURE0 + i);
            tex.bind();
            gl.uniform1i(this.gx.programInfo.uniforms.uSampler[i], i);
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
            POS:  this.curBlock.vtxs,
            //NRM:  this.curBlock.normals,
            TEX0: this.curBlock.texCoords,
            TEX1: this.curBlock.texCoords,
            TEX2: this.curBlock.texCoords,
            TEX3: this.curBlock.texCoords,
            TEX4: this.curBlock.texCoords,
            TEX5: this.curBlock.texCoords,
            TEX6: this.curBlock.texCoords,
            TEX7: this.curBlock.texCoords,
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
        this.gx.executeDisplayList(this.curBlock.dlists[idx], dlistData);
    }

    _renderOpSetVtxFmt() {
        /** Change the vertex data format.
         */
        const ops   = this.curOps;
        let posSize = ops.read(1) ? 3 : 2;
        //map blocks don't have normals
        let colSize = ops.read(1) ? 3 : 2; //has colors?
        let texSize = ops.read(1) ? 3 : 2; //texCoord size

        let TEX = [0, 0, 0, 0, 0, 0, 0, 0];
        if(this.curShader && !(this.curShader.flags & 0x80000000)) {
            for(let i=0; i<this.curShader.nLayers; i++) {
                TEX[i] = texSize;
            }
        }
        else TEX[0] = texSize;

        //console.log("Set vfmt: pos=%d col=%d tex=", posSize, colSize, texSize);

        let PNMTXIDX = 0;
        let POS      = posSize;
        let COL      = [colSize, 0];

        //XXX this can't be right. how does the game decide which VAT to set?
        for(let i=0; i<8; i++) {
            this.gx.cp.setReg(0x50+i, //VCD FMT LO
                (POS <<  9) | (COL[0] << 13) | (COL[1] << 15));
            this.gx.cp.setReg(0x60+i, //VCD FMT HI
                TEX[0] | (TEX[1] <<  2) | (TEX[2] <<  4) | (TEX[3] <<  6) |
                (TEX[4] <<  8) | (TEX[5] << 10) | (TEX[6] << 12) | (TEX[7] << 14));
            //let VATC = this.gx.cp.getReg(0x70+i & 0x7FFFFDFF);
            //if(this.model.header.flags24 & 0x08) { //use 9 normals
            //    VATC |= 0x80000200;
            //}
            //this.gx.cp.setReg(0x70+i, VATC);
        }
    }

    _renderOpMatrix() {
        /** Load one of the block's matrices into GX XF registers.
         */
        const ops   = this.curOps;
        const count = ops.read(4);
        const idxs  = []; //debug
        //console.log("init %d mtxs", count);

        let iVar3 = 0;
        if(count > 8) {
            let n = (count - 1) >> 3;
            ops.read(0x40 * n);
            let iVar3 = n * 8;
        }
        let iVar2 = count - iVar3;
        if(iVar3 < count) {
            ops.read(8 * iVar2);
        }
    }
}
