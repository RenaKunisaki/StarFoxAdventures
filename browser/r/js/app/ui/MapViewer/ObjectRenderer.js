import { MAP_CELL_SIZE } from "../../../game/Game.js";
import RenderBatch from "../gl/gx/RenderBatch.js";
import GX from "../gl/gx/GX.js";

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

        //generating a batch can take a while
        if(!isPicker) {
            this.app.progress.show({
                taskText:  "Loading Map",
                subText:   "Rendering objects...",
                stepsDone: 0,
                numSteps:  objs.length * 2,
            });
        }

        //render the objects (XXX why does this take so long?)
        this._setupRenderParams(batch);
        const offs = isPicker ? objs.length : 0;
        for(let i=0; i<objs.length; i++) {
            batch.addFunction(objs[i]);
            if(!(i & 0xF)) {
                await this.app.progress.update({stepsDone: i + offs});
            }
        }
        if(isPicker) this.app.progress.hide();

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
        batch.addVertices(...this._makeCube(x, y, z, s, id));
        return batch;
    }

    _setupRenderParams(batch) {
        /** Set up the render params to render objects.
         *  @param {RenderBatch} batch Batch to render to.
         */
        const gx = this.gx;
        const gl = this.gx.gl;
        batch.addFunction(() => {
            gl.enable(gl.CULL_FACE);
        });
        if(!this._isDrawingForPicker) {
            batch.addFunction(() => {
                gx.setBlendMode(GX.BlendMode.BLEND, GX.BlendFactor.SRCALPHA,
                    GX.BlendFactor.INVSRCALPHA, GX.LogicOp.NOOP);
                gx.setZMode(true, GX.CompareMode.LEQUAL, true);

                for(let i=0; i<this.gx.MAX_TEXTURES; i++) {
                    gl.activeTexture(gl.TEXTURE0 + i);
                    this.gx.whiteTexture.bind();
                    gl.uniform1i(this.gx.programInfo.uniforms.uSampler[i], i);
                }
            });
        }
    }

    _makeCube(x, y, z, scale, id) {
        const vtxPositions = [ //x, y, z
            [x-scale, y-scale, z-scale], //[0] top left back
            [x+scale, y-scale, z-scale], //[1] top right back
            [x+scale, y+scale, z-scale], //[2] top right front
            [x-scale, y+scale, z-scale], //[3] top left front
            [x-scale, y-scale, z+scale], //[4] bot left back
            [x+scale, y-scale, z+scale], //[5] bot right back
            [x+scale, y+scale, z+scale], //[6] bot right front
            [x-scale, y+scale, z+scale], //[7] bot left front
        ];
        const vtxColors = [ //r, g, b, a
            [0x00, 0x00, 0x00, 0xCF], //[0] top left back
            [0xFF, 0x00, 0x00, 0xCF], //[1] top right back
            [0xFF, 0xFF, 0x00, 0xCF], //[2] top right front
            [0x00, 0xFF, 0x00, 0xCF], //[3] top left front
            [0x00, 0x00, 0xFF, 0xCF], //[4] bot left back
            [0xFF, 0x00, 0xFF, 0xCF], //[5] bot right back
            [0xFF, 0xFF, 0xFF, 0xCF], //[6] bot right front
            [0x00, 0xFF, 0xFF, 0xCF], //[7] bot left front
        ];
        const vtxIdxs = [
            0,1,3, 1,2,3, //top
            3,2,6, 3,6,7, //front
            0,3,4, 3,7,4, //left
            2,1,5, 2,5,6, //right
            7,5,4, 7,6,5, //bottom
            1,0,5, 0,4,5, //back
        ];
        const vtxs = [this.gx.gl.TRIANGLES];
        for(const idx of vtxIdxs) {
            vtxs.push({
                POS:  vtxPositions[idx],
                COL0: vtxColors[idx],
                COL1: vtxColors[idx],
                id:   id,
            });
        }
        return vtxs;
    }
}
