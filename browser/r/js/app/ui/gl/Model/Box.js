import Model from "./Model.js";
import RenderBatch from "../gx/RenderBatch.js";
import { BugCheck } from "../../../errors.js";
const PI_OVER_180 = Math.PI / 180;

const DEFAULT_COLORS = [
    [0xFF, 0xFF, 0xFF, 0xCF],
    [0x00, 0xFF, 0xFF, 0xCF],
    [0xFF, 0xFF, 0x00, 0xCF],
    [0x00, 0xFF, 0x00, 0xCF],
    [0xFF, 0x00, 0xFF, 0xCF],
    [0x00, 0x00, 0xFF, 0xCF],
    [0x00, 0x00, 0x00, 0xCF],
    [0xFF, 0x00, 0x00, 0xCF],
];

export default class Box extends Model {
    /** A box, or rectangular prism.
     *  Expands from one corner point to another.
     *  By default, the box is aligned to global axes, then the
     *  rotation (if any) is applied.
     *  The colors are unspecified; currently, it will use an RGB cube,
     *  but if you need specific colors, set them yourself.
     */
    constructor(gx, p1=[-0.5,-0.5,-0.5], p2=[0.5,0.5,0.5]) {
        /** Construct Box between two corner points.
         *  @param {GX} gx GX instance to use.
         *  @param {Array} p1 Coords of first corner.
         *  @param {Array} p2 Coords of second corner.
         */
        super(gx, vec3.fromValues(p1[0]+0.5, p1[1]+0.5, p1[2]+0.5));
        this.p1 = vec3.fromValues(...p1);
        this.p2 = vec3.fromValues(...p2);
        this.setColors(DEFAULT_COLORS);
    }

    static fromLine(gx, p1, p2, size=[1,1,1]) {
        /** Construct a box which is rotated to contain the given line.
         *  @param {GX} gx GX instance to use.
         *  @param {Array} p1 Coords of first point.
         *  @param {Array} p2 Coords of second point.
         *  @param {Array} size Dimensions of box.
         *  @returns {Box} A box that covers the line.
         *  @note This creates a long, thin box (or thick if a different size
         *   is used) with the line at its centre. Compare to the default
         *   constructor which creates a large, axis-aligned box with the line
         *   spanning between two of its corners. The X size component determines
         *   how much extra length the box has beyond the ends of the line; the
         *   Y and Z components determine how far the box's sides are from the
         *   line.
         */
        //https://gamedev.stackexchange.com/a/149044
        //you would not belive how long it took to find this formula.
        //keywords: rotate arrow point target
        //XXX this is mostly duplicated by pointTo()
        let v1 = vec3.fromValues(...p1);
        let v2 = vec3.fromValues(...p2);
        const length = vec3.distance(v1, v2);

        //this method doesn't work if the line is exactly
        //along the Z axis, so handle that differently.
        if(Math.abs(p1[0]-p2[0]) + Math.abs(p1[1]-p2[1]) < 0.01) {
            let M = mat4.fromValues(
                1,     0,     0,     0,
                0,     1,     0,     0,
                0,     0,     1,     0,
                v1[0], v1[1], v1[2], 1);
            const s = (p1[2] > p2[2]) ? -1 : 1;
            const self = new Box(gx,
                [-size[2], -size[1], -size[0],       ],
                [ size[2],  size[1],  size[0]+(length*s)]);
            self.setMatrix(M);
            return self;
        }

        let vX = vec3.fromValues(v2[0]-v1[0], v2[1]-v1[1], v2[2]-v1[2]);
        vec3.normalize(vX, vX); //x now points at target, unit length
        let vY = vec3.fromValues(0,0,1);
        vec3.cross(vY, vY, vX);
        vec3.normalize(vY, vY); //y is now perpendicular to x, unit length
        let vZ = vec3.create();
        vec3.cross(vZ, vX, vY); //z is now perpendicular to both x and y, and unit length
        let M = mat4.fromValues(
            vX[0], vX[1], vX[2], 0,
            vY[0], vY[1], vY[2], 0,
            vZ[0], vZ[1], vZ[2], 0,
            v1[0], v1[1], v1[2], 1);
        const self = new Box(gx,
            [-size[0],        -size[1], -size[2]],
            [ size[0]+length,  size[1],  size[2]]);
        self.setMatrix(M);
        return self;
    }

    setColors(colors) {
        /** Set the vertex colors.
         *  @param {Array} colors One or more colors: [r, g, b, [a=255]].
         *  @note If fewer than 8 colors are supplied, the colors
         *   are repeated for the remaining vertices. Colors beyond
         *   the first 8 are ignored.
         *  @returns {Box} this.
         */
        if(!Array.isArray(colors[0])) colors = [colors]; //array of [r,g,b,a]
        if(colors.length < 1) throw new BugCheck("Empty color array");
        while(colors.length < 8) { //we have 8 vertices
            colors = colors.concat(colors);
        }
        this.colors = [];
        for(let i=0; i<8; i++) {
            let [r, g, b, a] = colors[i];
            if(a == undefined) a = 255;
            this.colors.push([r,g,b,a*this.opacity]);
        }
        this._needsUpdate = true;
        return this;
    }

    _update() {
        /** Recalculate buffers. Called after geometry changes. */
        const gl = this.gl;
        let v1 = vec3.create();
        let v2 = vec3.create();
        vec3.multiply(v1, this.p1, this.scale);
        vec3.multiply(v2, this.p2, this.scale);
        //vec3.add(v1, v1, this.pos);
        //vec3.add(v2, v2, this.pos);
        const [x1, y1, z1] = v1;
        const [x2, y2, z2] = v2;
        const vtxPositions = [ //x, y, z
            vec3.fromValues(x2, y2, z2),
            vec3.fromValues(x1, y2, z2),
            vec3.fromValues(x2, y2, z1),
            vec3.fromValues(x1, y2, z1),
            vec3.fromValues(x2, y1, z2),
            vec3.fromValues(x1, y1, z2),
            vec3.fromValues(x1, y1, z1),
            vec3.fromValues(x2, y1, z1),
        ];
        //do this now since it's faster than doing it on every render.
        let mtx = this.mtx ? mat4.clone(this.mtx) : mat4.create();
        mat4.translate(mtx, mtx, this.pos);
        //XXX will this work? doesn't look like it.
        //mat4.rotateX(mtx, mtx, this.rot[0]);
        //mat4.rotateY(mtx, mtx, this.rot[1]);
        //mat4.rotateZ(mtx, mtx, this.rot[2]);
        let q = quat.create();
        quat.fromEuler(q,
            //why the shit is this in degrees!?
            (this.rot[0] / PI_OVER_180),
            (this.rot[1] / PI_OVER_180),
            (this.rot[2] / PI_OVER_180),
        );
        let m2 = mat4.create();
        mat4.fromQuat(m2, q);
        mat4.multiply(mtx, mtx, m2);
        //this.pointTo(this.rot[0], this.rot[1], this.rot[2]);
        for(let v of vtxPositions) {
            vec3.transformMat4(v, v, mtx);
        }

        const vtxIdxs = [3,2,6,7,4,2,0,3,1,6,5,4,1,0];
        //const vtxIdxs = [0,1,4,5,6,1,3,0,2,4,7,6,2,3];
        const vtxs = [gl.TRIANGLE_STRIP];
        for(const idx of vtxIdxs) {
            vtxs.push({
                POS:  vtxPositions[idx],
                COL0: this.colors[idx],
                COL1: this.colors[idx],
                id:   this.id,
            });
        }

        this._batch = new RenderBatch(this.gx);
        this._batch.addVertices(...vtxs);
        this._needsUpdate = false;
        return this;
    }
}
