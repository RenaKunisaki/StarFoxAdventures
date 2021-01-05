import BaseRenderer from '../../BaseRenderer.js';
import VertexBuffer from '../../gx/VertexBuffer.js';
import BlockRenderer from './BlockRenderer.js';

export default class MapRenderer extends BaseRenderer {
    /** Renders SFA map.
     */
    constructor(view) {
        super(view.ctx);
        this.view = view;
        this.blockRenderer = new BlockRenderer(this);
    }

    render() {
        const gl = this.ctx.gl;
        const gx = this.ctx.gx;
        this.gl = gl;
        this.gx = gx;
        this.reset();
        //gx._setShaderMtxs();

        this.vtxBuf = new VertexBuffer(gx);
        gx.program.use();
        gx._setShaderMtxs();
        gl.enable(gl.BLEND);
        //draw cube at origin
        //this.drawCube({
        //    x:0, y:0, z:0, size:10, fill:true,
        //})

        this._setupToRender();
        this._drawBlocks();
        this._drawGrid();

        //this._drawObjects();
        //this.vtxBuf.bind();
        //gx.program.use();
        //gx._setShaderMtxs();
        //gl.drawArrays(gl.LINE_LOOP, 0, 16*this.view.romlist.items.length);
        this._didRender = true;
    }

    _setupToRender() {
        /** Set up our GX instance to the state it should be in when it's
         *  about to render the model.
         *  XXX this is mostly just copied from ModelRenderer and probably wrong.
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
        cp.setReg(0xB1, 3); //Stride NRM
        cp.setReg(0xB2, 2); //Stride COL0
        //XXX B3? (COL1)
        cp.setReg(0xB4, 4); //Stride TEX0
        cp.setReg(0xB5, 4); //Stride TEX1
    }



    _drawGrid() { //draw planes for map grid
        console.log("Draw map grid...");
        const gl = this.ctx.gl;
        const gx = this.ctx.gx;
        let count = 0;
        let first = this.vtxBuf.vtxCount;
        const map = this.view.map;
        for(let z=0; z<map.height; z++) {
            for(let x=0; x<map.width; x++) {
                let block = map.getBlockByPos(x, z);
                if(block) {
                    let mod  = block.mod;
                    let sub  = block.sub;
                    let unk1 = block.unk1;
                    let unk2 = block.unk2;
                    let col  = [
                        (block.mod & 0x1F) << 5,
                        (block.sub & 0x3F) << 2,
                        block.unk1,
                        0x7F];
                    let bx   = x * 640, bz = z * 640;
                    let v    = [ //XXX this is very wasteful. use indexed rendering.
                        {POS:[bx+ 10, 0, bz+ 10], COL0:col},
                        {POS:[bx+620, 0, bz+ 10], COL0:col},
                        {POS:[bx+620, 0, bz+620], COL0:col},
                        {POS:[bx+ 10, 0, bz+620], COL0:col},
                    ];
                    this.vtxBuf.addVtxs(v[0], v[1], v[2], v[0], v[2], v[3]);
                    count += 6;
                }
            }
        }
        this.vtxBuf.bind();
        gl.drawArrays(gl.TRIANGLES, first, count);
    }

    _drawBlocks() { //draw actual map geometry
        console.log("Draw blocks...");
        const gl    = this.ctx.gl;
        const map   = this.view.map;

        /* const color = [0, 0, 0, 255];
        let first   = this.vtxBuf.vtxCount;
        let count   = 0;
        for(let y=0; y<map.height; y++) {
            for(let x=0; x<map.width; x++) {
                const block = this.view.blocks[(y*map.width)+x];
                if(!block) continue;

                for(let vtx of block.vtxs) {
                    this.vtxBuf.addVtx({POS:vtx, COL0:color});
                    count++;
                }
            }
        }
        this.vtxBuf.bind();
        gl.drawArrays(gl.POINTS, first, count); */

        let count = 0;
        for(let y=0; y<map.height; y++) {
            for(let x=0; x<map.width; x++) {
                const block = this.view.blocks[(y*map.width)+x];
                if(block) {
                    this.blockRenderer.renderBlock(block, 0);
                    count++;
                    if(count >= 2) break;
                }
            }
        }
    }

    _drawObjects() { //draw cubes at objects
        console.log("Draw objects...");
        for(let item of this.view.romlist.items) {
            this.drawCube({
                x: item.objDef.x,
                y: item.objDef.y,
                z: item.objDef.z,
                color: [
                    ( item.objDef.type       & 0x0F) << 4,
                    ((item.objDef.type >> 4) & 0x0F) << 4,
                    ((item.objDef.type >> 8) & 0x0F) << 4,
                    255,
                ],
                size:10, fill:true,
            })
        }
    }
}
