import VertexBuffer from './VertexBuffer.js';
import CP from './CP.js';
import XF from './XF.js';
import DlistParser from './DlistParser.js';
import Program from '../Program.js';
import Texture from '../Texture.js';
import {get} from '/js/Util.js';

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
        this.blankTexture.makeSolidColor(255, 255, 255, 0);
        //if changing this we need to also add more samplers in the fragment
        //shader and update loadPrograms()
        this.MAX_TEXTURES = 2;

        //the order the fields appear in in a display list.
        //this never changes, but is here so that other classes can
        //easily access it.
        this.vatFieldOrder = [
            'PNMTXIDX', 'T0MIDX', 'T1MIDX', 'T2MIDX', 'T3MIDX', 'T4MIDX',
            'T5MIDX', 'T6MIDX', 'T7MIDX', 'POS', 'NRM', 'COL0', 'COL1',
            'TEX0', 'TEX1', 'TEX2', 'TEX3', 'TEX4', 'TEX5', 'TEX6',
            'TEX7'];
        this.cp          = new CP(this);
        this.xf          = new XF(this);
        this.vtxBuf      = new VertexBuffer(this);
        this.dlistParser = new DlistParser(this);
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
        this.vtxLog = []; //log all drawn vertices for picker
        this.stats = {
            nVtxs:     0, //total vtxs sent to GPU
            nDrawCmds: 0, //total draw commands issued
            nDlists:   0, //total display lists executed
            geomBound: { //min/max value of each vtx axis
                xMin:  99999999, yMin:  99999999, zMin:  99999999,
                xMax: -99999999, yMax: -99999999, zMax: -99999999,
            },
            PNMTXIDX: {}, //count of each idx
        }

        //reset lights to whatever the user set.
        //XXX this should really be moved to a "before render" method.
        gl.uniform3iv(this.programInfo.uniforms.ambLightColor,
            this.context.lights.ambient.color);
        gl.uniform3iv(this.programInfo.uniforms.dirLightColor,
            this.context.lights.directional.color);
        gl.uniform3fv(this.programInfo.uniforms.dirLightVector,
            this.context.lights.directional.vector);
    }

    printStats() {
        /** Print stats to console for debug.
         */
        const S = this.stats;
        console.log("nVtxs=%d nDrawCmds=%d nDlists=%d", S.nVtxs, S.nDrawCmds, S.nDlists);
        console.log("X bound: %d - %d", S.geomBound.xMin, S.geomBound.xMax);
        console.log("Y bound: %d - %d", S.geomBound.yMin, S.geomBound.yMax);
        console.log("Z bound: %d - %d", S.geomBound.zMin, S.geomBound.zMax);
        console.log("XF:", this.xf);
        console.log("CP:", this.cp);
        console.log("PNMTXIDX counts:", S.PNMTXIDX);
    }

    drawBoundCube() {
        /** Draw a cube around the boundaries of the geometry that's been
         *  rendered so far by the display lists.
         *  This is an old debug function that doesn't account for matrix
         *  transforms and will need some fixing.
         */
        //XXX needs to set color buffer, etc.
        const gl = this.gl;
        this.vtxBuf.clear();
        this.vtxBuf.setPosMtx(null);

        /*4________5
          /       /.
         /       / |
        .-------.  |
        |0     1|  |6
        |   7   | /
        |3     2|/
        '-------' */
        const vtxs = [
            [-1, -1, -1], [ 1, -1, -1], [ 1,  1, -1], [-1,  1, -1],
            [-1, -1,  1], [ 1, -1,  1], [ 1,  1,  1], [-1,  1,  1],
        ];
        const idxs = [
            0, 1, 2,   0, 2, 3,   4, 5, 6,   4, 6, 7, //-Z, +Z faces
            0, 4, 7,   0, 7, 3,   1, 5, 6,   1, 6, 2, //-X, +X faces
            0, 4, 5,   0, 5, 1,   3, 7, 6,   3, 6, 2, //-Y, +Y faces
        ];
        const B = this.stats.geomBound;
        for(const i of idxs) {
            let x = vtxs[i][0], y = vtxs[i][1], z = vtxs[i][2];
            x *= (x < 0 ? B.xMin : B.xMax);
            y *= (y < 0 ? B.yMin : B.yMax);
            z *= (z < 0 ? B.zMin : B.zMax);
            this.vtxBuf.addVtx({POS:[x, y, z]});
        }
        const buffers = this.vtxBuf.build();

        { //bind the buffers to our program's attributes
            const numComponents = 3;  // pull out 3 values per iteration
            const type = gl.FLOAT;    // the data in the buffer is 32bit floats
            const normalize = false;  // don't normalize
            const stride = 0;         // how many bytes to get from one set of values to the next
                          // 0 = use type and numComponents above
            const offset = 0;         // how many bytes inside the buffer to start from

            gl.bindBuffer(gl.ARRAY_BUFFER, buffers.position);
            gl.vertexAttribPointer(this.programInfo.attribs.vtxPos,
                numComponents, type, normalize, stride, offset);
            gl.enableVertexAttribArray(this.programInfo.attribs.vtxPos);
        }
        this.program.use();
        gl.uniformMatrix4fv(this.programInfo.uniforms.matProjection,
            false, this.context.matProjection);
        gl.uniformMatrix4fv(this.programInfo.uniforms.matModelView,
            false, this.context.matModelView);
        //gl.drawArrays(gl.TRIANGLES, 0, 36);
        gl.drawArrays(gl.LINE_LOOP, 0, 36);
    }

    async loadPrograms() {
        /** Download and set up the shader programs.
         */
        const gl = this.gl;

        //get shader code and create program
        this.program = new Program(this.context, {
            [gl.VERTEX_SHADER]:   (await get('/js/gl/gx/vertex.glsl'))  .responseText,
            [gl.FRAGMENT_SHADER]: (await get('/js/gl/gx/fragment.glsl')).responseText,
        });

        //get program info
        this.programInfo = {
            program: this.program,
            attribs: {
                vtxPos:      this.program.getAttribLocation('vtxPos'),
                vtxColor:    this.program.getAttribLocation('vtxColor'),
                vtxNormal:   this.program.getAttribLocation('vtxNormal'),
                vtxId:       this.program.getAttribLocation('vtxId'),
                vtxTexCoord: this.program.getAttribLocation('vtxTexCoord'),
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
    }

    executeDisplayList(list, data) {
        /** Execute display list.
         *  list: List (ArrayBuffer) to execute.
         *  data: Dict of data sources for attribute array indices.
         */
        this.dlistParser.execute(list, data);
        //dlistParser will refer back to our fields and call our methods
        //to perform the commands from the display list.
        this.stats.nDlists++;
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
         *  mode: GX rendering mode.
         *  Returns: {
         *    glMode: GL rendering mode.
         *    glVtxCount: Number of vertices added to buffer.
         *    glOffset: Offset of first vertex added. (Currently always 0.)
         *  }
         *  Since this method triangulates quads, the mode and vertex count
         *  can differ from the input.
         */
        const gl = this.gl;
        const res = {glMode:null, glVtxCount:vtxs.length, glOffset:0};
        this.vtxBuf.clear();
        switch(mode) {
            case 0: case 1: //quads, also quads
                //there's no quads mode in modern GL so we need to convert
                //to triangles.
                res.glVtxCount = 0;
                for(let i=0; i<vtxs.length; i += 4) {
                    //XXX find a more efficient way to do this.
                    //we can probably use index buffers instead of repeating
                    //vertices, use one big buffer instead of refilling a small
                    //one many times, not use gl.STATIC_DRAW, etc.
                    const v0=vtxs[i], v1=vtxs[i+1], v2=vtxs[i+2], v3=vtxs[i+3];
                    const mi0=v0.PNMTXIDX, mi1=v1.PNMTXIDX;
                    const mi2=v2.PNMTXIDX, mi3=v3.PNMTXIDX;
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
                res.glMode = gl.TRIANGLES;
                break;

            case 2: res.glMode = gl.TRIANGLES;      break;
            case 3: res.glMode = gl.TRIANGLE_STRIP; break;
            case 4: res.glMode = gl.TRIANGLE_FAN;   break;
            case 5: res.glMode = gl.LINES;          break;
            case 6: res.glMode = gl.LINE_STRIP;     break;
            case 7: res.glMode = gl.POINTS;         break;
            default: throw new Error("Invalid draw op mode: "+String(mode));
        }
        if(mode != 0 && mode != 1) { //add vtxs to buffer if we didn't already
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
    }

    _doDrawOp(mode, vtxs) {
        /** Execute a draw operation.
         *  Called from DlistParser.
         *  mode: Draw mode.
         *  vtxs: Array of vertices.
         */
        const gl = this.gl;
        this._recordVtxStats(vtxs);
        const drawParams = this._storeVtxsToBuffer(vtxs, mode);

        this.vtxBuf.bind();
        //this.program.use();
        this._setShaderMtxs();

        //drawParams.glMode = gl.LINE_LOOP; //DEBUG - wireframe (glitchy)
        gl.drawArrays(drawParams.glMode, drawParams.glOffset, drawParams.glVtxCount);
        this.stats.nDrawCmds++;
    }
}
