import VertexBuffer from './VertexBuffer.js';
import CP from './CP.js';
import XF from './XF.js';
import DlistParser from './DlistParser.js';
import Program from '../Program.js';
import Texture from '../Texture.js';
import {get} from '/r/js/Util.js';
import RenderBatch from './RenderBatch.js';

//the order the fields appear in in a display list. this never changes.
export const VAT_FIELD_ORDER = [
    'PNMTXIDX', 'T0MIDX', 'T1MIDX', 'T2MIDX', 'T3MIDX', 'T4MIDX',
    'T5MIDX', 'T6MIDX', 'T7MIDX', 'POS', 'NRM', 'COL0', 'COL1',
    'TEX0', 'TEX1', 'TEX2', 'TEX3', 'TEX4', 'TEX5', 'TEX6',
    'TEX7'];

export default class GX {
    /** GameCube GPU simulator.
     *  While nowhere near precise enough to be considered an emulator, this
     *  class functions roughly the same as the real GX chip - give it arrays
     *  containing display list and vertex attribute data, set up the registers
     *  telling how the data is formatted, and it renders an image.
     */
    constructor(context) {
        this.context = context;
        this.gl      = context.gl;
        this.blankTexture = new Texture(context);
        this.blankTexture.makeSolidColor(255, 0, 255, 0);
        //if changing this we need to also add more samplers in the fragment
        //shader and update loadPrograms()
        this.MAX_TEXTURES = 2;
        this.cp           = new CP(this);
        this.xf           = new XF(this);
        this.vtxBuf       = new VertexBuffer(this);
        this.dlistParser  = new DlistParser(this);
    }

    reset() {
        /** Reset all state to default.
         */
        const gl = this.gl;
        this.cp.reset();
        this.xf.reset();
        this.vtxBuf.clear();
        this.program.use();
        this.gl.uniform1i(this.programInfo.uniforms.useId, 0);
        this.gl.uniform1i(this.programInfo.uniforms.useLights, 0);
        this.gl.uniform1i(this.programInfo.uniforms.useTexture, 0);
        this.vtxLog = []; //log all drawn vertices for picker
        this.stats = {
            nVtxs:     0, //total vtxs sent to GPU
            nPolys:    0, //total polygons drawn
            nDrawCmds: 0, //total draw commands issued
            //nDlists:   0, //total display lists executed
            //geomBound: { //min/max value of each vtx axis
            //    xMin:  99999999, yMin:  99999999, zMin:  99999999,
            //    xMax: -99999999, yMax: -99999999, zMax: -99999999,
            //},
            //PNMTXIDX: {}, //count of each idx
            //drawModeCount: {}, //count of each draw mode (tris, quads...)
        }
    }

    printStats() {
        /** Print stats to console for debug. */
        const S = this.stats;
        console.log("nVtxs=%d nDrawCmds=%d nDlists=%d", S.nVtxs, S.nDrawCmds, S.nDlists);
        //console.log("X bound: %d - %d", S.geomBound.xMin, S.geomBound.xMax);
        //console.log("Y bound: %d - %d", S.geomBound.yMin, S.geomBound.yMax);
        //console.log("Z bound: %d - %d", S.geomBound.zMin, S.geomBound.zMax);
        console.log("XF:", this.xf);
        console.log("CP:", this.cp);
        //console.log("PNMTXIDX counts:", S.PNMTXIDX);
        //console.log("Draw mode counts:", S.drawModeCount);
    }

    async loadPrograms() {
        /** Download and set up the shader programs. */
        const gl = this.gl;

        //get shader code and create program
        const path = '/r/js/app/ui/gl/gx';
        this.program = new Program(this.context, {
            [gl.VERTEX_SHADER]:   (await get(`${path}/vertex.glsl`))  .responseText,
            [gl.FRAGMENT_SHADER]: (await get(`${path}/fragment.glsl`)).responseText,
        });

        //get program info, used to set variables
        this.programInfo = {
            program: this.program,
            attribs: {
                POS:  this.program.getAttribLocation('vtxPos'),
                COL0: this.program.getAttribLocation('vtxColor'),
                NRM:  this.program.getAttribLocation('vtxNormal'),
                //ID:   this.program.getAttribLocation('vtxId'),
                TEX0: this.program.getAttribLocation('vtxTexCoord'),
            },
            uniforms: {
                matProjection: this.program.getUniformLocation('matProjection'),
                matModelView:  this.program.getUniformLocation('matModelView'),
                matNormal:     this.program.getUniformLocation('matNormal'),
                useId:         this.program.getUniformLocation('useId'),
                ambLightColor: this.program.getUniformLocation('ambLightColor'),
                dirLightColor: this.program.getUniformLocation('dirLightColor'),
                dirLightVector:this.program.getUniformLocation('dirLightVector'),
                uSampler:      [
                    this.program.getUniformLocation('uSampler0'),
                    this.program.getUniformLocation('uSampler1'),
                ],
            },
        };
        console.log("GX loadPrograms OK");
    }

    beginRender() {
        /** Reset render state for new frame. */
        const gl = this.gl;
        this.program.use();
        //reset lights to whatever the user set.
        gl.uniform3iv(this.programInfo.uniforms.ambLightColor,
            this.context.lights.ambient.color);
        gl.uniform3iv(this.programInfo.uniforms.dirLightColor,
            this.context.lights.directional.color);
        gl.uniform3fv(this.programInfo.uniforms.dirLightVector,
            this.context.lights.directional.vector);
    }

    executeBatch(batch) {
        /** Execute render batch.
         *  @param {RenderBatch} batch Render batch to execute.
         */
        const ctx = this.context;
        const stats = batch.execute(this.programInfo, {
            projection: ctx.matProjection,
            modelView:  ctx.matModelView,
            normal:     ctx.matNormal,
        });
        this.stats.nVtxs     += stats.nVtxs;
        this.stats.nDrawCmds += stats.nOps;
        this.stats.nPolys    += stats.nPolys;
    }

    _recordVtxStats(vtxs) {
        /** Update vertex stats for debug.
         */
        const geomBound = this.stats.geomBound;
        for(let i=0; i<vtxs.length; i++) {
            const vtx = vtxs[i];

            //log for picker
            vtx.id = this.vtxLog.length;
            vtx.dlist = this.stats.nDlists;
            this.vtxLog.push(vtx);

            if(vtx.POS) {
                geomBound.xMin = Math.min(geomBound.xMin, vtx.POS[0]);
                geomBound.yMin = Math.min(geomBound.yMin, vtx.POS[1]);
                geomBound.zMin = Math.min(geomBound.zMin, vtx.POS[2]);
                geomBound.xMax = Math.max(geomBound.xMax, vtx.POS[0]);
                geomBound.yMax = Math.max(geomBound.yMax, vtx.POS[1]);
                geomBound.zMax = Math.max(geomBound.zMax, vtx.POS[2]);
            }
        }
        this.stats.nVtxs += vtxs.length;
    }

    _storeVtxsToBuffer(vtxs, mode) {
        /** Store vertices in vertex buffer.
         *  @param mode GX rendering mode.
         *  @returns {object} {
         *    glMode: GL rendering mode.
         *    glVtxCount: Number of vertices added to buffer.
         *    glOffset: Offset of first vertex added. (Currently always 0.)
         *  }
         *  @note Since this method triangulates quads, the mode and vertex
         *   count can differ from the input.
         */
        const gl = this.gl;
        const res = {glMode:null, glVtxCount:vtxs.length, glOffset:0};
        this.vtxBuf.clear();
        if(this.stats.drawModeCount[mode] == undefined) {
            this.stats.drawModeCount[mode] = 0;
        }
        this.stats.drawModeCount[mode]++;
        switch(mode) {
            case 0: case 1: //quads, also quads
                //there's no quads mode in modern GL so we need to convert
                //to triangles.
                res.glVtxCount = 0;
                res.glMode = gl.TRIANGLES;
                for(let i=0; i<vtxs.length; i += 4) {
                    //XXX find a more efficient way to do this.
                    //we can probably use index buffers instead of repeating
                    //vertices, use one big buffer instead of refilling a small
                    //one many times, not use gl.STATIC_DRAW, etc.
                    let v0=vtxs[i], v1=vtxs[i+1], v2=vtxs[i+2], v3=vtxs[i+3];
                    if(!(v0 && v1 && v2 && v3)) {
                        const n = (v0 ? 1 : 0) + (v1 ? 1 : 0) +
                            (v2 ? 1 : 0) + (v3 ? 1 : 0);
                        console.warn("Quad only has %d vtxs", n);
                        if(!v1) v1 = v0;
                        if(!v2) v2 = v1;
                        if(!v3) v3 = v2;
                    }
                    if(v0) {
                        const mi0=v0 ? v0.PNMTXIDX : undefined;
                        const mi1=v1 ? v1.PNMTXIDX : undefined;
                        const mi2=v2 ? v2.PNMTXIDX : undefined;
                        const mi3=v3 ? v3.PNMTXIDX : undefined;
                        const pm0=(mi0 == undefined) ? null : this.xf.getPosMtx(mi0);
                        const pm1=(mi1 == undefined) ? null : this.xf.getPosMtx(mi1);
                        const pm2=(mi2 == undefined) ? null : this.xf.getPosMtx(mi2);
                        const pm3=(mi3 == undefined) ? null : this.xf.getPosMtx(mi3);
                        const nm0=null; //this.xf.getNrmMtx(v0.PNMTXIDX);
                        const nm1=null; //this.xf.getNrmMtx(v1.PNMTXIDX);
                        const nm2=null; //this.xf.getNrmMtx(v2.PNMTXIDX);
                        const nm3=null; //this.xf.getNrmMtx(v3.PNMTXIDX);
                        const m0 = {POS:pm0, NRM:nm0}, m1 = {POS:pm1, NRM:nm1};
                        const m2 = {POS:pm2, NRM:nm2}, m3 = {POS:pm3, NRM:nm3};
                        this.vtxBuf.setMtxs(m0); this.vtxBuf.addVtx(v0);
                        this.vtxBuf.setMtxs(m1); this.vtxBuf.addVtx(v1);
                        this.vtxBuf.setMtxs(m2); this.vtxBuf.addVtx(v2);
                        this.vtxBuf.setMtxs(m0); this.vtxBuf.addVtx(v0);
                        this.vtxBuf.setMtxs(m2); this.vtxBuf.addVtx(v2);
                        this.vtxBuf.setMtxs(m3); this.vtxBuf.addVtx(v3);
                        res.glVtxCount += 6;
                    }
                }
                break;

            case 2: res.glMode = gl.TRIANGLES;      break;
            case 3: res.glMode = gl.TRIANGLE_STRIP; break;
            case 4: res.glMode = gl.TRIANGLE_FAN;   break;
            case 5: res.glMode = gl.LINES;          break;
            case 6: res.glMode = gl.LINE_STRIP;     break;
            case 7: res.glMode = gl.POINTS;         break;
            default: throw new Error("Invalid draw op mode: "+String(mode));
        }
        if(mode > 1) { //add vtxs to buffer if we didn't already
            for(let i=0; i<vtxs.length; i++) {
                const mi = vtxs[i].PNMTXIDX;
                const pm = (mi==undefined) ? null : this.xf.getPosMtx(mi);
                const nm = null; //this.xf.getNrmMtx(vtxs[i].PNMTXIDX);
                this.vtxBuf.setMtxs({POS:pm, NRM:nm});
                this.vtxBuf.addVtx(vtxs[i]);
            }
        }
        return res;
    }

    _setShaderMtxs() {
        /** Send the current projection, modelview, and normal matrices
         *  to the shaders.
         */
        const gl = this.gl;
        gl.uniformMatrix4fv(this.programInfo.uniforms.matProjection,
            false, this.context.matProjection);
        gl.uniformMatrix4fv(this.programInfo.uniforms.matModelView,
            false, this.context.matModelView);
        gl.uniformMatrix4fv(this.programInfo.uniforms.matNormal,
            false, this.context.matNormal);
        //console.log("mtxs: proj", this.context.matProjection,
        //    "modelview", this.context.matNormal,
        //    "normal", this.context.matNormal);
    }

    _doDrawOp(mode, vtxs) {
        /** Execute a draw operation.
         *  @param mode Draw mode.
         *  @param vtxs Array of vertices.
         *  @note Called from DlistParser.
         */
        const gl = this.gl;
        this._recordVtxStats(vtxs);
        const drawParams = this._storeVtxsToBuffer(vtxs, mode);

        this.vtxBuf.bind();
        //this.program.use();
        this._setShaderMtxs();

        //XXX this doesn't work very well.
        if(this.context.useWireframe) drawParams.glMode = gl.LINE_LOOP;
        //drawParams.glMode = gl.POINTS;
        //console.log("drawArrays", drawParams);
        gl.drawArrays(drawParams.glMode, drawParams.glOffset, drawParams.glVtxCount);
        this.stats.nDrawCmds++;
    }
}
