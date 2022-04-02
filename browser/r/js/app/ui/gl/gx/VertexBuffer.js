import {validNumber, validVector, validMatrix} from '../GlUtil.js';

export default class VertexBuffer {
    /** Vertex data buffer used by GX.
     */
    constructor(gx) {
        this.gx          = gx;
        this.gl          = gx.gl;
        this.bufPos      = this.gl.createBuffer(); //float x, y, z
        this.bufNrm      = this.gl.createBuffer(); //float x, y, z
        this.bufTexCoord = this.gl.createBuffer(); //float s, t
        this.bufColor    = this.gl.createBuffer(); //u8 r, g, b, a
        this.bufId       = this.gl.createBuffer(); //u32 id
        this.clear();
    }

    clear() {
        /** Clear all buffers.
         */
        this.datPos      = []; //arrays of vtx data
        this.datNrm      = [];
        this.datTexCoord = [];
        this.datColor    = [];
        this.datId       = [];
        this.vtxCount    = 0;
        this.mtxPos      = null;
        this.mtxNrm      = null;
        this.mtxTexCoord = null;
        this.mtxColor    = null;
    }

    setPosMtx     (mtx) { this.mtxPos      = validMatrix(mtx); }
    setNrmMtx     (mtx) { this.mtxNrm      = validMatrix(mtx); }
    setTexCoordMtx(mtx) { this.mtxTexCoord = validMatrix(mtx); }
    setColorMtx   (mtx) { this.mtxColor    = validMatrix(mtx); }
    setMtxs(mtxs) {
        if(mtxs.POS) this.setPosMtx     (mtxs.POS);
        if(mtxs.NRM) this.setNrmMtx     (mtxs.NRM);
        if(mtxs.TEX) this.setTexCoordMtx(mtxs.TEX);
        if(mtxs.COL) this.setColorMtx   (mtxs.COL);
    }

    _addPos(pos) {
        let v = vec3.fromValues(pos[0], pos[1], pos[2] || 0);
        if(this.mtxPos) vec3.transformMat4(v, v, this.mtxPos);
        this.datPos.push(v[0]); this.datPos.push(v[1]); this.datPos.push(v[2]);
    }

    _addColor(col) {
        if(col == undefined || col == null) col = [255,255,255,255];
        this.datColor.push(col[0]); this.datColor.push(col[1]);
        this.datColor.push(col[2]); this.datColor.push(col[3]);
    }

    _addNormal(nrm) {
        if(nrm == undefined || nrm == null) nrm = [0,0,0];
        nrm = validVector(nrm);
        let v = vec3.fromValues(nrm[0], nrm[1], nrm[2]);
        if(this.mtxNrm) vec3.transformMat3(v, v, this.mtxNrm);
        this.datNrm.push(v[0]); this.datNrm.push(v[1]); this.datNrm.push(v[2]);
    }

    _addTexCoord(coord) {
        if(coord == undefined || coord == null) coord = [0,0];
        const s = validNumber(coord[0]);
        const t = (coord[1] == undefined) ? 0 : validNumber(coord[1]);
        const v = vec2.fromValues(s, t);
        if(this.mtxTexCoord) vec2.transformMat4(v, v, this.mtxTexCoord);
        this.datTexCoord.push(v[0]); this.datTexCoord.push(v[1]);
    }

    addVtx(vtx) {
        /** Add a vertex to the buffer.
         *  @param {object} vtx Vertex to add.
         */
        this._addPos(vtx.POS);
        this._addNormal(vtx.NRM);
        if(vtx.debugColor) this._addColor(vtx.debugColor);
        else this._addColor(vtx.COL0);
        this._addTexCoord(vtx.TEX0);
        //if(vtx.COL1) { for(const v of vtx.COL1) this.datColor.push(v); }
        //if(vtx.TEX1) { for(const v of vtx.TEX1) this.datTexCoord.push(v); }
        //if(vtx.TEX2) { for(const v of vtx.TEX2) this.datTexCoord.push(v); }
        //if(vtx.TEX3) { for(const v of vtx.TEX3) this.datTexCoord.push(v); }
        //if(vtx.TEX4) { for(const v of vtx.TEX4) this.datTexCoord.push(v); }
        //if(vtx.TEX5) { for(const v of vtx.TEX5) this.datTexCoord.push(v); }
        //if(vtx.TEX6) { for(const v of vtx.TEX6) this.datTexCoord.push(v); }
        //if(vtx.TEX7) { for(const v of vtx.TEX7) this.datTexCoord.push(v); }
        const id = vtx.id || 0; //undefined => 0
        this.datId.push((id >> 24) & 0xFF);
        this.datId.push((id >> 16) & 0xFF);
        this.datId.push((id >>  8) & 0xFF);
        this.datId.push( id        & 0xFF);
        this.vtxCount++;
    }

    addVtxs(...vtx) {
        for(let v of vtx) this.addVtx(v);
    }

    build() {
        /** Push the data into the GL buffers.
         *  Returns a dict of buffer name => GL buffer object.
         *  Mainly only used by bind().
         */
        const gl = this.gl;

        //build the vertex buffer objects GL-side
        gl.bindBuffer(gl.ARRAY_BUFFER, this.bufPos);
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(this.datPos), gl.STATIC_DRAW);

        gl.bindBuffer(gl.ARRAY_BUFFER, this.bufNrm);
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(this.datNrm), gl.STATIC_DRAW);

        gl.bindBuffer(gl.ARRAY_BUFFER, this.bufTexCoord);
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(this.datTexCoord), gl.STATIC_DRAW);

        gl.bindBuffer(gl.ARRAY_BUFFER, this.bufColor);
        gl.bufferData(gl.ARRAY_BUFFER, new Uint8Array(this.datColor), gl.STATIC_DRAW);

        gl.bindBuffer(gl.ARRAY_BUFFER, this.bufId);
        gl.bufferData(gl.ARRAY_BUFFER, new Uint8Array(this.datId), gl.STATIC_DRAW);

        //console.log("VtxBuf build size pos=%d nrm=%d tex=%d col=%d",
        //    this.datPos.length,
        //    this.datNrm.length,
        //    this.datTexCoord.length,
        //    this.datColor.length);

        return {
            position: this.bufPos,
            normal:   this.bufNrm,
            texCoord: this.bufTexCoord,
            color:    this.bufColor,
            id:       this.bufId,
        };
    }

    bind() {
        /** Build the buffers and bind them to the vertex attributes
         *  in the GX program.
         */
        const gl = this.gl;
        const programInfo = this.gx.programInfo;

        //XXX make this more efficient.
        const buffers = this.build();

        //Position buffer
        gl.bindBuffer(gl.ARRAY_BUFFER, buffers.position);
        gl.vertexAttribPointer(programInfo.attribs.vtxPos,
            3,        //number of values per vertex
            gl.FLOAT, //data type in buffer
            false,    //don't normalize
            0,        //stride (0=use type and numComponents)
            0);       //offset in bytes to start from in buffer
        gl.enableVertexAttribArray(programInfo.attribs.vtxPos);

        //Color buffer
        gl.bindBuffer(gl.ARRAY_BUFFER, buffers.color);
        gl.vertexAttribPointer(programInfo.attribs.vtxColor,
            4, gl.UNSIGNED_BYTE, false, 0, 0);
        gl.enableVertexAttribArray(programInfo.attribs.vtxColor);

        //Normal buffer
        gl.bindBuffer(gl.ARRAY_BUFFER, buffers.normal);
        gl.vertexAttribPointer(programInfo.attribs.vtxNormal,
            3, gl.FLOAT, false, 0, 0);
        gl.enableVertexAttribArray(programInfo.attribs.vtxNormal);

        //Texcoord buffer
        gl.bindBuffer(gl.ARRAY_BUFFER, buffers.texCoord);
        gl.vertexAttribPointer(programInfo.attribs.vtxTexCoord,
            2, gl.FLOAT, false, 0, 0);
        gl.enableVertexAttribArray(programInfo.attribs.vtxTexCoord);

        //ID buffer for picking
        gl.bindBuffer(gl.ARRAY_BUFFER, buffers.id);
        gl.vertexAttribPointer(programInfo.attribs.vtxId,
            4, gl.UNSIGNED_BYTE, false, 0, 0);
        gl.enableVertexAttribArray(programInfo.attribs.vtxId);
    }
}
