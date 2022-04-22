import RenderBatch from "../gl/gx/RenderBatch.js";
import GX from "../gl/gx/GX.js";
import { makeCube } from "../gl/GlUtil.js";

export default class ObjectRenderer {
    /** Handles object rendering for map viewer. */
    constructor(mapViewer) {
        this.mapViewer = mapViewer;
        this.app = this.mapViewer.app;
        this.gx = this.mapViewer.gx;
        this.reset();
    }

    reset() {
        /** Reset state for new map. */
        //keep track of picker IDs assigned to objects
        //so we don't keep making up new ones
        this.pickerIds = {}; //entry idx => picker ID

        //cache created batches
        this.batches = {}; //key => batch
    }

    async loadObjects() {
        /** Load object models and prepare batches. */
        const map = this.mapViewer.map;
        if(!map.romList) return; //nothing to render

        for(let i=0; i<map.romList.entries.length; i++) {
            if(!(i & 0xF)) {
                await this.app.progress.update({
                    taskText:  "Loading Map",
                    subText:   "Decoding object models...",
                    stepsDone: i,
                    numSteps:  map.romList.entries.length,
                });
            }

            //pre-render the object
            const entry = map.romList.entries[i];
            const batch = this.drawObject(entry);
            this.batches[`obj${entry.idx}`] = batch;
        }
    }

    async drawObjects(act, isPicker) {
        /** Draw all enabled objects.
         *  @param {integer} act Which act number to draw.
         *  @param {bool} isPicker Whether we're rendering for picker buffer.
         *  @returns {RenderBatch} Batch that renders the objects.
         */
        const gx = this.gx;
        const gl = this.gx.gl;
        this._isDrawingForPicker = isPicker;

        const cacheKey = [isPicker, act].join(':');

        //if we already generated a batch, use it.
        if(this.batches[cacheKey]) return this.batches[cacheKey];

        console.log("Creating object batch", cacheKey);
        const batch = new RenderBatch(this.gx);
        this.batches[cacheKey] = batch;

        //act 0 = no objects (same as in game)
        if(act == 0) return batch;
        const map = this.mapViewer.map;
        if(!map.romList) return batch; //nothing to render

        const objs = [];
        for(let entry of map.romList.entries) {
            //act -1 = all objects (same as in game, even though
            //it's not possible to actually set the act to -1
            //in the game)
            if(act == -1 || entry.acts[act]) {
                objs.push(this.batches[`obj${entry.idx}`])
            }
        }

        this._setupRenderParams(batch);
        batch.addBatches(...objs);

        return batch;
    }

    drawObject(entry) {
        /** Draw an object.
         *  @param {RomListEntry} entry Object to draw.
         */
        const gl = this.gx.gl;
        const batch = new RenderBatch(this.gx);
        const x = entry.position.x;
        const y = entry.position.y;
        const z = entry.position.z;
        const s = Math.max(entry.object.scale, 10);
        //console.log("draw obj", entry, "at", x, y, z, "scale", s)

        let id = this.pickerIds[entry.idx];
        if(id == undefined) {
            id = this.gx.addPickerObj({
                type: 'object',
                obj:  entry,
            });
            this.pickerIds[entry.idx] = id;
        }

        //just draw a cube
        batch.addVertices(...makeCube(gl, x, y, z, s, id));
        return batch;
    }

    _setupRenderParams(batch) {
        /** Set up the render params to render objects.
         *  @param {RenderBatch} batch Batch to render to.
         */
        const gx = this.gx;
        const gl = this.gx.gl;
        if(this._isDrawingForPicker) {
            batch.addFunction(() => {
                //blending off, face culling off
                gx.disableTextures(GX.BlendMode.NONE, false);
            });
        }
        else {
            batch.addFunction(() => {
                //blending on, face culling off
                gx.disableTextures(GX.BlendMode.BLEND, false);
            });
        }
    }
}
