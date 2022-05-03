import Model from "./Model.js";
import RenderBatch from "../gx/RenderBatch.js";

//increase for more precise cylinders at the expense of performance
const SLICES = 32;
const two_pi = Math.PI * 2;

export default class Cylinder extends Model {
    /** A cylinder, centered at the given point, with radius 1 and height 1.
     *  Use the `scale` property to change radius and height.
     *  @note The X and Z scale set the diameter (NOT radius) on the axes;
     *   the Y scale sets the height in BOTH directions from centre.
     */
    constructor(gx, pos) {
        /** Construct Cylinder at given position.
         *  @param {GX} gx GX instance to use.
         *  @param {Array} pos Coords of center.
         */
        super(gx, vec3.fromValues(...pos));
        this.color = [255, 255, 255, 255];
    }

    setColor(color) {
        /** Set the vertex colors.
         *  @param {Array} color Color of the cylinder [r, g, b, a].
         *  @returns {Cylinder} this.
         */
        this.color = color;
        this._needsUpdate = true;
        return this;
    }

    _update() {
        /** Recalculate buffers. Called after geometry changes. */
        const gl = this.gl;

        const vTop = [], vBot = [];
        for(let slice = 0; slice < SLICES; ++slice) {
            let x = Math.sin(two_pi * slice / SLICES);
            let z = Math.cos(two_pi * slice / SLICES);
            vTop.push(vec3.fromValues(x,  1, z));
            vBot.push(vec3.fromValues(x, -1, z));
        }
        const offsBot = vTop.length;
        const vtxPositions = vTop.concat(vBot);
        vtxPositions.push([0, -1, 0]); //top
        vtxPositions.push([0,  1, 0]); //bottom

        //do this now since it's faster than doing it on every render.
        this.mtx = mat4.create();
        mat4.fromTranslation(this.mtx, this.pos);
        mat4.scale(this.mtx, this.mtx, this.scale);
        for(let v of vtxPositions) {
            vec3.transformMat4(v, v, this.mtx);
        }

        //push vertices alternating top/bottom
        let vtxs = [gl.TRIANGLE_STRIP];
        for(let iSlice = 0; iSlice < SLICES+1; ++iSlice) {
            const n = iSlice % SLICES;
            vtxs.push({
                POS:  vtxPositions[n],
                COL0: this.color, COL1: this.color, id: this.id,
            });
            vtxs.push({
                POS:  vtxPositions[n+offsBot],
                COL0: this.color, COL1: this.color, id: this.id,
            });
        }

        this._batch = new RenderBatch(this.gx);
        this._batch.addVertices(...vtxs);

        //manually draw top and bottom
        vtxs = [gl.TRIANGLE_FAN, {
            POS:  vtxPositions[vtxPositions.length - 2],
            COL0: this.color, COL1: this.color, id: this.id,
        }];
        for(let i=0; i<SLICES; i++) {
            vtxs.push({
                POS:  vtxPositions[i],
                COL0: this.color, COL1: this.color, id: this.id,
            });
        }
        this._batch.addVertices(...vtxs);

        //bottom
        vtxs = [gl.TRIANGLE_FAN, {
            POS:  vtxPositions[vtxPositions.length - 1],
            COL0: this.color, COL1: this.color, id: this.id,
        }];
        for(let i=0; i<SLICES; i++) {
            vtxs.push({
                POS:  vtxPositions[i+offsBot],
                COL0: this.color, COL1: this.color, id: this.id,
            });
        }
        this._batch.addVertices(...vtxs);

        this._needsUpdate = false;
        return this;
    }
}
