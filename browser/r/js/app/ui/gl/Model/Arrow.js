import Model from "./Model.js";
import RenderBatch from "../gx/RenderBatch.js";
import { BugCheck } from "../../../errors.js";
const PI_OVER_180 = Math.PI / 180;

const VTX_POSITIONS = [
    [ 0.3,  0.3,  0.0],
    [ 0.3, -0.3,  0.0],
    [ 0.3,  0.3,  1.0],
    [ 0.3, -0.3,  1.0],
    [-0.3,  0.3,  0.0],
    [-0.3, -0.3,  0.0],
    [-0.3,  0.3,  1.0],
    [-0.3, -0.3,  1.0],
    [ 0.0,  0.0, -1.0],
    [ 0.6,  0.6,  0.0],
    [ 0.6, -0.6,  0.0],
    [-0.6,  0.6,  0.0],
    [-0.6, -0.6,  0.0],
];

const VTX_IDXS = [
     4,  2,  0,
     2,  7,  3,
     6,  5,  7,
     1,  7,  5,
     0,  3,  1,
     4,  1,  5,
     8, 11,  9,
    12, 11,  8,
     8, 10, 12,
     8,  9, 10,
    12,  9, 11,
     4,  6,  2,
     2,  6,  7,
     6,  4,  5,
     1,  3,  7,
     0,  2,  3,
     4,  0,  1,
    12, 10,  9,
];

export default class Arrow extends Model {
    /** An arrow. */
    constructor(gx, pos=[0,0,0], rot=[0,0,0], scale=[1,1,1]) {
        /** Construct arrow centered at given point.
         *  @param {GX} gx GX instance to use.
         *  @param {Array} pos Model position in 3D environment.
         *  @param {Array} rot Model rotation on X, Y, Z axes, in radians.
         *  @param {Array} scale Model scale on X, Y, Z axes.
         */
        super(gx, pos, rot, scale);
        this.setColor(0xFF, 0xFF, 0xFF, 0xFF);
    }

    setColor(r, g, b, a=255) {
        /** Set the vertex colors.
         *  @returns {Arrow} this.
         */
        this.color = [r,g,b,a];
        this._needsUpdate = true;
        return this;
    }

    _update() {
        /** Recalculate buffers. Called after geometry changes. */
        const gl = this.gl;
        //do this now since it's faster than doing it on every render.
        let mtx = this.mtx ? mat4.clone(this.mtx) : mat4.create();
        mat4.translate(mtx, mtx, this.pos);
        let q = quat.create();
        quat.fromEuler(q,
            (this.rot[0] / PI_OVER_180), //why the shit is this in degrees!?
            (this.rot[1] / PI_OVER_180),
            (this.rot[2] / PI_OVER_180),
        );
        let m2 = mat4.create();
        mat4.fromQuat(m2, q);
        mat4.multiply(mtx, mtx, m2);
        const scale = this.scale;
        const vtxPositions = [];
        for(let v of VTX_POSITIONS) {
            let v1 = vec3.fromValues(v[0]*scale[0], v[1]*scale[1], v[2]*scale[2]);
            let v2 = vec3.create();
            vec3.transformMat4(v2, v1, mtx);
            vtxPositions.push(v2);
        }

        const vtxs = [gl.TRIANGLES];
        for(const idx of VTX_IDXS) {
            vtxs.push({
                POS:  vtxPositions[idx],
                COL0: this.color,
                COL1: this.color,
                id:   this.id,
            });
        }
        console.log("arrow vtxs", vtxs);

        this._batch = new RenderBatch(this.gx);
        this._batch.addVertices(...vtxs);
        this._needsUpdate = false;
        return this;
    }
}
