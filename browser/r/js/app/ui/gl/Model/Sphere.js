import Model from "./Model.js";
import RenderBatch from "../gx/RenderBatch.js";
import { BugCheck } from "../../../errors.js";

//increase for more precise spheres at the expense of performance
const STACKS = 32;
const SLICES = 32;
const two_pi = Math.PI * 2;

export default class Sphere extends Model {
    /** A sphere, centered at the given point, with radius 1.
     *  Use the `scale` property to change radius or make ellipsoids.
     */
    constructor(gx, pos) {
        /** Construct Sphere at given position.
         *  @param {GX} gx GX instance to use.
         *  @param {Array} pos Coords of center.
         */
        super(gx, vec3.fromValues(...pos));
        this.color = [255, 255, 255, 255];
    }

    setColor(color) {
        /** Set the vertex colors.
         *  @param {Array} color Color of the sphere [r, g, b, a].
         *  @returns {Sphere} this.
         */
        this.color = color;
        this._needsUpdate = true;
        return this;
    }

    _update() {
        /** Recalculate buffers. Called after geometry changes. */
        const gl = this.gl;

        const vtxPositions = [];
        for(let stack = 0; stack < STACKS; ++stack) {
            for(let slice = 0; slice < SLICES; ++slice) {
                let y = 2.0 * stack / STACKS - 1.0; //faster
                //let y = -Math.cosf(Math.PI * stack / STACKS); //more precise
                let r = Math.sqrt(1 - (y * y));
                let x = r * Math.sin(two_pi * slice / SLICES);
                let z = r * Math.cos(two_pi * slice / SLICES);
                let v = vec3.fromValues(x, y, z);
                vtxPositions.push(v);
            }
        }
        vtxPositions.push([0, -1, 0]); //top
        vtxPositions.push([0,  1, 0]); //bottom

        //do this now since it's faster than doing it on every render.
        this.mtx = mat4.create();
        mat4.fromTranslation(this.mtx, this.pos);
        mat4.scale(this.mtx, this.mtx, this.scale);
        for(let v of vtxPositions) {
            vec3.transformMat4(v, v, this.mtx);
        }

        let vtxs = [gl.TRIANGLE_STRIP];

        //draw strips around the perimeter.
        for(let iStack = 0; iStack < STACKS - 1; ++iStack) {
            let s1 = iStack, s2 = iStack + 1;
            if(s2 >= STACKS) s2 = 0;

            //first two vtxs
            vtxs.push({ //top left
                POS:  vtxPositions[s1 * SLICES],
                COL0: this.color, COL1: this.color, id: this.id,
            });
            vtxs.push({ //bottom left
                POS:  vtxPositions[s2 * SLICES],
                COL0: this.color, COL1: this.color, id: this.id,
            });

            //then chain the rest on for the strip
            for(let iSlice = 1; iSlice < SLICES; ++iSlice) {
                let c1 = iSlice, c2 = iSlice + 1;
                if(c2 >= SLICES) c2 = 0;
                vtxs.push({ //top right
                    POS:  vtxPositions[(s1 * SLICES+c2)],
                    COL0: this.color, COL1: this.color, id: this.id,
                });
                vtxs.push({ //bottom right
                    POS:  vtxPositions[(s2 * SLICES+c2)],
                    COL0: this.color, COL1: this.color, id: this.id,
                });
            }
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
                POS:  vtxPositions[i + ((STACKS - 1) * SLICES)],
                COL0: this.color, COL1: this.color, id: this.id,
            });
        }
        this._batch.addVertices(...vtxs);

        this._needsUpdate = false;
        return this;
    }
}
