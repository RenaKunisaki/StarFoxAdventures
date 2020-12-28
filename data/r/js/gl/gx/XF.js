import {validNumber, validVector, validMatrix} from '/r/js/Util.js';

export default class XF {
    /** Transform Processor subsystem for GX.
     */
    constructor(gx) {
        this.gx = gx;
        this.reset();
    }

    reset() {
        /** Reset all state to default.
         */
        this._reg = [];
        this._mtx = {
            'POS': {}, //0x0000 - 0x00FF
            'NRM': {}, //0x0400 - 0x045F
            'DUALTEX': {}, //0x0500 - 0x05FF
        };
        for(let i=0; i<0x1058; i++) this._reg.push(null);
        this._updateMtxs();
    }

    setReg(reg, val) {
        /** Set a register.
         *  reg: Register ID.
         *  val: Value, which should be an integer or float, depending
         *    on the target register.
         */
        //while it is allowed to store Inf/NaN on the real GX, there's no reason
        //we should be doing so in this program.
        this._reg[reg] = validNumber(val);
        this._updateMtxs();
    }

    getReg(reg) {
        /** Read a register.
         *  reg: Register ID.
         *  Returns the value, or null if the register hasn't been set, or
         *    undefined if the register doesn't exist. (This is a bit backward,
         *    but JS uses undefined for nonexistent entries, so it's
         *    consistent at least.)
         */
        return this._reg[reg];
    }

    getPosMtx(idx) {
        /** Get a position matrix.
         *  idx: Matrix index.
         *  Returns a mat4.
         *  XF Position Matrix Memory is an array of floats, and a matrix can
         *  begin at any index that's a multiple of 4. So the index is just the
         *  register ID divided by 4. That means that if matrix index 0 is
         *  [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11] then matrix index 1 is
         *  [4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15].
         *  Note also that these are only 12 entries. GX position matrices are
         *  only 4x3, but since GLmatrix.js doesn't deal with those, we convert
         *  to 4x4 when populating tihs array.
         */
        if(this._mtx.POS[idx] == undefined) {
            console.error("Position matrix "+String(idx)+" is undefined");
            //debugger;
            return mat4.create();
            //throw new Error("Position matrix "+String(idx)+" is undefined");
        }
        return this._mtx.POS[idx];
    }

    getNrmMtx(idx) {
        /** Get a normal matrix.
         *  idx: Matrix index.
         *  Returns a mat3.
         *  This is similar to position matrices, but these are 3x3.
         */
        if(this._mtx.NRM[idx] == undefined) {
            console.error("Normal matrix "+String(idx)+" is undefined");
            return mat3.create();
            //debugger;
            throw new Error("Normal matrix "+String(idx)+" is undefined");
        }
        return this._mtx.NRM[idx];
    }

    setMtx(idx, mtx) {
        /** Set a matrix.
         *  idx: Matrix index. (Register ID divided by 4)
         *  mtx: Matrix (mat4) to set.
         */
        //XXX this only works for 4x4 matrices. what about normals?
        /* mat4 gives us a matrix like:
         * [ 1  0  0  0]
         * [ 0  1  0  0]
         * [ 0  0  1  0]
         * [tx ty tz  1]
         * but XF transform memory stores 4x3 mtxs, which means we need to
         * transpose this matrix and then discard the last row.
         */
        validMatrix(mtx);
        const m = mat4.create();
        mat4.transpose(m, mtx);
        for(let i=0; i<12; i++) this._reg[(idx*4)+i] = m[i];
        this._updateMtxs();
    }

    _makeMat3(reg) {
        if(reg < 0 || reg >= 0x1058 - 9) {
            throw new Error("Reading matrix from reg", reg);
        }
        const R = this._reg;
        return mat3.fromValues( //XXX this is probably transposed too.
            R[reg  ], R[reg+1], R[reg+2],
            R[reg+3], R[reg+4], R[reg+5],
            R[reg+6], R[reg+7], R[reg+8]);
    }
    _makeMat4(reg) {
        if(reg < 0 || reg >= 0x1058 - 16) {
            throw new Error("Reading matrix from reg", reg);
        }
        const R = this._reg;
        for(let i=0; i<12; i++) {
            if(R[reg+i] == null || R[reg+i] == undefined) return null;
        }
        //transpose here, but then transpose again because for some
        //insane reason this method takes values in column-major order
        //even though they can only be written in row-major order.
        //the end result is we don't transpose after all.
        return mat4.fromValues(
            R[reg   ], R[reg+ 4], R[reg+ 8], 0,
            R[reg+ 1], R[reg+ 5], R[reg+ 9], 0,
            R[reg+ 2], R[reg+ 6], R[reg+10], 0,
            R[reg+ 3], R[reg+ 7], R[reg+11], 1);
    }

    _updateMtxs() {
        /** Recompute the matrices.
         *  Called when some matrix memory has changed.
         */
        //XXX we could save a lot of time by only computing each matrix when
        //it's requested, caching the result, and invalidating that cache when
        //the registers behind it change.

        //position matrix memory is rows of 4 floats.
        //a matrix can begin at any row, and has 3 rows.
        for(let i=0; i<64; i++) {
            this._mtx.POS[i] = this._makeMat4(i*4);
            //this might be wrong. I don't think it's even used here.
            this._mtx.DUALTEX[i] = this._makeMat4((i*4)+0x0500);
        }
        //normal matrix memory is rows of 3 floats, and 3x3 mtxs.
        for(let i=0; i<32; i++) {
            this._mtx.NRM[i] = this._makeMat3((i*3)+0x0400);
        }
    }
}
