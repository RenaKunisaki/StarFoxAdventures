import Model from "./Model.js";
import RenderBatch from "../gx/RenderBatch.js";
import { BugCheck } from "../../../errors.js";
const PI_OVER_180 = Math.PI / 180;

const VTX_POSITIONS = [
    [0.00,  1.00,  0.00],
    [0.05,  0.05,  0.00],
    [1.00,  0.00,  0.00],
    [0.05,  0.05,  0.05],
    [0.05,  0.00,  0.00],
    [0.00,  0.00,  1.00],
    [0.00,  0.05,  0.05],
    [0.05,  0.00,  0.05],
    [0.00,  0.00,  0.00],
    [0.00,  0.05,  0.00],
    [0.00,  0.00,  0.05],
];
const VTX_COLORS = [
    [0, 1, 0],
    [1, 1, 1],
    [1, 0, 0],
    [1, 1, 1],
    [1, 1, 1],
    [0, 0, 1],
    [1, 1, 1],
    [1, 1, 1],
    [1, 1, 1],
    [1, 1, 1],
    [1, 1, 1],
];
const VTX_IDXS = [
     9,  0,  6,
     6,  0,  3,
     3,  0,  1,
     1,  0,  9,
     6,  1,  9,
     1,  2,  3,
     3,  2,  7,
     7,  2,  4,
     4,  2,  1,
     7,  1,  3,
     3,  5,  6,
     6,  5, 10,
    10,  5,  7,
     7,  5,  3,
    10,  3,  6,
     8,  6, 10,
     1,  8,  4,
     7,  8, 10,
     6,  3,  1,
     7,  4,  1,
    10,  7,  3,
     8,  9,  6,
     1,  9,  8,
     7,  4,  8,
];

export default class Axes extends Model {
    /** A set of red, green, and blue axis arrows. */
    constructor(gx, pos=[0,0,0], rot=[0,0,0], scale=[1,1,1]) {
        /** Construct axes centered at given point.
         *  @param {GX} gx GX instance to use.
         *  @param {Array} pos Model position in 3D environment.
         *  @param {Array} rot Model rotation on X, Y, Z axes, in radians.
         *  @param {Array} scale Model scale on X, Y, Z axes.
         */
        super(gx, pos, rot, scale);
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
            const [r,g,b] = VTX_COLORS[idx];
            const color = [r*255, g*255, b*255, 0xA0];
            vtxs.push({
                POS:  vtxPositions[idx],
                COL0: color,
                COL1: color,
                id:   this.id,
            });
        }

        this._batch = new RenderBatch(this.gx);
        this._batch.addVertices(...vtxs);
        this._needsUpdate = false;
        return this;
    }
}
