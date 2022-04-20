import Game, {MAP_CELL_SIZE} from "../../../game/Game.js";
import { assertType } from "../../../Util.js";
import { E, clearElement } from "../../../lib/Element.js";
import { hex } from "../../../Util.js";
import Context from "../gl/Context.js";
import GX from "../gl/gx/GX.js";
import BlockRenderer from "./BlockRenderer.js";
import ViewController from "../gl/ui/ViewController.js";
import Grid from "./Grid.js";
import Stats from "./Stats.js";
import LayerChooser from "./LayerChooser.js";
import InfoWidget from "./InfoWidget.js";
import RenderBatch from "../gl/gx/RenderBatch.js";
import EventHandler from "./EventHandler.js";
import ObjectRenderer from "./ObjectRenderer.js";
import { makeCube, makeBox } from "../gl/GlUtil.js";

export default class MapViewer {
    /** Renders map geometry. */
    constructor(game) {
        this.game          = assertType(game, Game);
        this.app           = game.app;
        this.element       = document.getElementById('tab-mapView');
        this.canvas        = E.canvas('map-view-canvas');
        this.context       = null;
        this.eMapList      = null;
        this.map           = null; //current map
        this.curBlock      = null;
        this.grid          = new Grid(this);
        this.stats         = new Stats(this);
        this.layerChooser  = new LayerChooser(this);
        this.infoWidget    = new InfoWidget(this);
        this.eSidebar      = E.div('sidebar');
        this._eventHandler = new EventHandler(this);
        this._reset();
        this.app.onIsoLoaded(iso => this._onIsoLoaded());
    }

    _onIsoLoaded() {
        /** Set up the map viewer. */
        this._buildMapList();
        clearElement(this.element).append(
            E.div('controls',
                E.label(null, {For:'mapview-map-list'}, "Map:"),
                this.eMapList,
            ),
            this.canvas,
            this.eSidebar,
            this.grid.element,
        )
        if(!this.context) this._initContext();
    }

    async _initContext() {
        /** Set up the GL context. */
        if(this.context) return;
        this.context = new Context(this.canvas,
            (isPicker) => this._draw(isPicker));
        await this.context.init();
        console.log("GL ctx init OK");

        this.gx = new GX(this.context);
        await this.gx.loadPrograms();

        this.viewController = new ViewController(this.context);
        this.eSidebar.append(this.viewController.element,
            this.layerChooser.element,
            this.infoWidget.element,
            this.stats.element,
        );

        this._blockRenderer  = new BlockRenderer(this.gx);
        this._objectRenderer = new ObjectRenderer(this);
        this.context.canvas.focus();
        this._reset();
    }

    _buildMapList() {
        /** Build the map list widget. */
        this.eMapList = E.select({id:'mapview-map-list'});
        const maps = [];
        for(let [id, map] of Object.entries(this.game.maps)) {
            maps.push({id:id, map:map});
        }
        maps.sort((a, b) => { //sort by name, falling back to ID
            let nA = a.map.name;
            let nB = b.map.name;
            if(!nA) nA = `\x7F${a.id}`;
            if(!nB) nB = `\x7F${b.id}`;
            if(nA == nB) return 0;
            return (nA < nB) ? -1 : 1;
        });
        this.eMapList.append(E.option(null, "(no map selected)", {value:'null'}));
        for(let map of maps) { //build list with placeholder names as needed
            let name = map.map.name;
            if(!name) name = "(no name)";
            let id = map.id;
            if(isNaN(id)) id = '??';
            else id = hex(id,2);
            this.eMapList.append(E.option(null, `${id} ${name}`,
                {value:map.id}));
        }
        this.eMapList.addEventListener('change', e => this._reset());
    }

    _reset() {
        /** Reset viewer to display another map. */
        this._warpBatch        = null;
        this._warpBatchPicker  = null;
        this._hitPolyBatch     = null;
        this._polyGroupBatch   = null;
        this._blockBoundsBatch = null;
        this._isFirstDrawAfterLoadingMap = true;

        if(!this.context) return; //don't start if not initialized
        //don't start until user actually picks a map
        if(this.eMapList.value == 'null') return;
        this.game.unloadTextures();

        const map = this.game.maps[this.eMapList.value];
        if(!map) {
            console.error("Invalid map selected", this.eMapList.value);
            return;
        }

        console.log("Loading map", map);
        this.map = map;
        if(!map.dirName) {
            console.error("Map has no directory", map);
            //continue, so we can examine the romlists that
            //don't have any corresponding geometry.
        }

        this.gx.resetPicker();
        this._objectRenderer.reset();
        this._blockRenderer.reset();
        this.curBlock = this._findABlock();
        this.layerChooser.refresh();
        this.grid.refresh();
        this.redraw();
        this._updatedStats = false;
    }

    async _loadMap() {
        /** Load the map data. */
        this.app.progress.show({
            taskText:  "Loading Map",
            subText:   "Loading block textures...",
            stepsDone: 0,
            numSteps:  this.map.blocks.length,
        });

        //load block data
        for(let iBlock=0; iBlock < this.map.blocks.length; iBlock++) {
            const block = this.map.blocks[iBlock];
            await this.app.progress.update({stepsDone:iBlock});
            if(block && block.mod < 0xFF) block.load(this.gx);
        }

        //load object data
        await this._objectRenderer.loadObjects();
    }

    _findABlock() {
        /** Find a block to start at. */
        if(!this.map.blocks) {
            console.error("Map has no blocks", this.map);
            return null;
        }
        //if there's one at the origin, prefer it.
        let block = this.map.getBlock(0, 0);
        console.assert(this.gx);
        console.assert(this.gx.context);
        if(block && block.load(this.gx)) return block;

        //find the first non-empty, non-missing block.
        let iBlock = 0;
        while(iBlock < this.map.blocks.length) {
            if(this.map.blocks[iBlock]
            && this.map.blocks[iBlock].mod < 0xFF
            && this.map.blocks[iBlock].load(this.gx)) break;
            iBlock++;
        }
        block = this.map.blocks[iBlock];
        if(!block) {
            console.error("Map has no blocks", this.map);
            return null;
        }
        return block;
    }

    async redraw() {
        /** Signal the map viewer to redraw. */
        if(this._pendingDraw) return;
        this._pendingDraw = true;
        if(this._isFirstDrawAfterLoadingMap) await this._loadMap();

        window.requestAnimationFrame(() => {
            this._pendingDraw = false;
            //this.grid.refresh();
            this.context.redraw();
            //this.stats.refresh();
            if(!this._updatedStats) {
                this._updatedStats = true;
                this.stats.refresh();
            }
            if(this._isFirstDrawAfterLoadingMap) {
                this.app.progress.hide();
                this._isFirstDrawAfterLoadingMap = false;
            }
        });
    }

    async _draw(isPicker) {
        /** Draw the map. Called by Context. */
        if(!this.map) return;

        this._isDrawingForPicker = isPicker;

        //const tStart = performance.now();
        const LC = this.layerChooser;
        const blockStreams = [
            ['main',       LC.getLayer('mainGeometry')],
            ['water',      LC.getLayer('waterGeometry')],
            ['reflective', LC.getLayer('reflectiveGeometry')],
        ];

        const blockStats = {totals:{}};
        this._beginRender();
        this._drawBlocks(blockStats, blockStreams);
        if(LC.getLayer('blockHits')) this._drawBlockHits();
        await this._drawObjects();
        if(LC.getLayer('warps')) this._drawWarps();
        if(LC.getLayer('hitPolys')) this._drawHitPolys();
        if(LC.getLayer('polyGroups')) this._drawPolyGroups();
        if(LC.getLayer('blockBounds')) this._drawBlockBounds();
        this._finishRender(blockStats, blockStreams);
        //console.log("block render OK", this.gx.context.stats);
        //console.log("GX logs:", this.gx.program.getLogs());
        //if(isPicker) console.log("picker IDs", this.gx.pickerObjs);
    }

    _beginRender() {
        /** Set up to render a frame. */
        //const gl = this.gx.gl;
        if(!this.curBlock) {
            this.curBlock = this._findABlock();
            if(!this.curBlock) return;
        }

        //console.log("map block", this.curBlock);
        const ctx = this.gx.context;
        const mtxs = {
            projection: ctx.matProjection,
            modelView:  ctx.matModelView,
            normal:     ctx.matNormal,
        };

        this.gx.context.resetStats();
        this.gx.reset();
        this.gx.beginRender(mtxs, this._isDrawingForPicker);
    }

    _finishRender(blockStats, blockStreams) {
        /** Finish rendering and record stats. */
        blockStats.streamTimes = {};
        const tEnd = performance.now();
        for(const [name, stream] of blockStreams) {
            blockStats.streamTimes[name] = 0;
            for(let iBlock=0; iBlock < this.map.blocks.length; iBlock++) {
                const obj = blockStats[name][iBlock];
                if(!obj) continue;
                blockStats.streamTimes[name] += obj.renderTime;
                for(const [k,v] of Object.entries(obj)) {
                    if(isNaN(v)) continue;
                    if(blockStats.totals[k] == undefined) blockStats.totals[k] = v;
                    else blockStats.totals[k] += v;
                }
            }
        }
        //console.log("Render msec:", tEnd-tStart, "stats:", blockStats);
        this.stats.updateDrawCounts(blockStats);
    }

    _drawBlocks(blockStats, blockStreams) {
        /** Draw the map geometry. */
        const params = {
            showHidden: this.layerChooser.getLayer('hiddenGeometry'),
            isGrass: false, //draw the grass effect instead of the geometry
            isPicker: this._isDrawingForPicker,
        };
        for(const [name, stream] of blockStreams) {
            blockStats[name] = [];
            for(let iBlock=0; iBlock < this.map.blocks.length; iBlock++) {
                const block = this.map.blocks[iBlock];
                if(!block || (block.mod >= 0xFF) || !block.load()) continue;
                let batch = null;
                if(stream) batch = this._drawBlock(block, params, name);
                this.gx.context.stats.renderTime = performance.now() -
                    this.gx.context.stats.renderStartTime;
                this.gx.context.stats.block = block;
                this.gx.context.stats.batch = batch;
                blockStats[name].push(this.gx.context.stats);
                this.gx.context.resetStats();
            }
        }
    }

    _drawBlock(block, params, stream) {
        /** Draw one map block. */
        const gl = this.gx.gl;
        block.load(this.gx); //ensure block model is loaded

        const offsX = block.x*MAP_CELL_SIZE;
        const offsY = block.header.yOffset;
        const offsZ = block.z*MAP_CELL_SIZE;
        let mv = mat4.clone(this.gx.context.matModelView);
        mat4.translate(mv, mv, vec3.fromValues(offsX, offsY, offsZ));
        this.gx.setModelViewMtx(mv);

        const batch = this._blockRenderer.render(block, stream, params);
        if(batch) { //there was in fact a block to render
            const gb = this.gx.context.stats.geomBounds;
            gb.xMin = Math.min(gb.xMin, batch.geomBounds.xMin+offsX);
            gb.xMax = Math.max(gb.xMax, batch.geomBounds.xMax+offsX);
            gb.yMin = Math.min(gb.yMin, batch.geomBounds.yMin+offsY);
            gb.yMax = Math.max(gb.yMax, batch.geomBounds.yMax+offsY);
            gb.zMin = Math.min(gb.zMin, batch.geomBounds.zMin+offsZ);
            gb.zMax = Math.max(gb.zMax, batch.geomBounds.zMax+offsZ);
        }
        return batch; //for stats
    }

    _drawBlockHits() {
        /** Draw the hitboxes for each block. */
        if(this._isDrawingForPicker) return;
        for(let iBlock=0; iBlock < this.map.blocks.length; iBlock++) {
            const block = this.map.blocks[iBlock];
            if(!block || (block.mod >= 0xFF)) continue;
            this._blockRenderer.renderHits(block, {
                isPicker: this._isDrawingForPicker,
            });
        }
    }

    _drawBlockBounds() {
        /** Draw the bounding boxes for each block. */
        const gx = this.gx;
        const gl = this.gx.gl;
        if(this._isDrawingForPicker) return;

        let batch = this._blockBoundsBatch;
        if(batch) {
            this.gx.executeBatch(batch);
            return batch;
        }
        batch = new RenderBatch(gx);
        this._blockBoundsBatch = batch;

        batch.addFunction(() => {
            //blend on, face culling off
            gx.disableTextures(GX.BlendMode.BLEND, false);
            gx.setModelViewMtx(mat4.clone(gx.context.matModelView));
        });
        for(let iBlock=0; iBlock < this.map.blocks.length; iBlock++) {
            const block = this.map.blocks[iBlock];
            if(!block || (block.mod >= 0xFF)) continue;

            //top left
            const x1 = (block.x * MAP_CELL_SIZE);
            const y1 = (block && block.header) ? block.header.yMax :  1;
            const z1 = block.z * MAP_CELL_SIZE;

            //bottom right
            const x2 = (block.x+1) * MAP_CELL_SIZE;
            const y2 = (block && block.header) ? block.header.yMin : -1;
            const z2 = (block.z+1) * MAP_CELL_SIZE;

            batch.addVertices(...makeBox(gl, [x1,y1,z1], [x2,y2,z2], -1, 0x80));
        }
        this.gx.executeBatch(batch);
        return batch;
    }

    _drawWarps() {
        /** Draw WARPTAB entries. */
        const gx = this.gx;
        const gl = this.gx.gl;

        let mv = mat4.clone(this.gx.context.matModelView);
        this.gx.setModelViewMtx(mv);

        if(this._isDrawingForPicker) {
            if(this._warpBatchPicker) {
                this.gx.executeBatch(this._warpBatchPicker);
                return;
            }
        }
        else if(this._warpBatch) {
            if(this._warpBatch) {
                this.gx.executeBatch(this._warpBatch);
                return;
            }
        }

        const batch = new RenderBatch(this.gx);
        if(this._isDrawingForPicker) {
            this._warpBatchPicker = batch;
            batch.addFunction(() => {
                gl.disable(gl.BLEND);
                gx.setBlendMode(GX.BlendMode.NONE, GX.BlendFactor.SRCALPHA,
                    GX.BlendFactor.INVSRCALPHA, GX.LogicOp.NOOP);
            });
        }
        else {
            this._warpBatch = batch;
            batch.addFunction(() => {
                //blend on, face culling off
                gx.disableTextures(GX.BlendMode.BLEND, false);
                gx.setModelViewMtx(mat4.clone(gx.context.matModelView));
            });
        }
        for(let [idx, warp] of Object.entries(this.game.warpTab)) {
            const map = this.game.getMapAt(warp.layer, warp.pos.x, warp.pos.z);
            if(map != this.map) continue;
            const id = this.gx.addPickerObj({
                type: 'warp',
                warp: warp,
                idx:  idx,
            });
            const x = warp.pos.x - this.map.worldOrigin[0];
            const y = warp.pos.y;
            const z = warp.pos.z - this.map.worldOrigin[1];
            batch.addVertices(...makeCube(
                gl, x, y, z, 10, id, [[0xFF, 0x00, 0x00, 0xC0]]));
            //console.log("add warp", hex(idx), x, y, z, "orig", warp.pos);
        }
        batch.addVertices(...makeCube( //map origin
            gl, this.map.worldOrigin[0], 0, this.map.worldOrigin[1],
            10, -1, [[0x00, 0xFF, 0x00, 0xC0]]));
        this.gx.executeBatch(batch);
    }

    async _drawObjects() {
        /** Draw object positions. */
        let mv = mat4.clone(this.gx.context.matModelView);
        mat4.translate(mv, mv, vec3.fromValues(
            (this.map.originX * MAP_CELL_SIZE), 0,
            (this.map.originZ * MAP_CELL_SIZE) ));
        this.gx.setModelViewMtx(mv);
        const batch = await this._objectRenderer.drawObjects(
            this.layerChooser.getLayer('actNo'), this._isDrawingForPicker);
        if(batch) this.gx.executeBatch(batch);
    }

    _drawHitPolys() {
        /** Draw hit detect polygons. */
        const gx = this.gx;
        const gl = this.gx.gl;

        if(this._isDrawingForPicker) return;
        if(this._hitPolyBatch) {
            gx.executeBatch(this._hitPolyBatch);
            return;
        }
        const batch = new RenderBatch(gx);
        this._hitPolyBatch = batch;

        //blending on, face culling off
        batch.addFunction(() => { gx.disableTextures(GX.BlendMode.BLEND, false) });
        for(let iBlock=0; iBlock < this.map.blocks.length; iBlock++) {
            const block = this.map.blocks[iBlock];
            if(!block || (block.mod >= 0xFF) || !block.polygons) continue;
            batch.addFunction(() => { this._setMtxForBlock(block) });

            const vtxs = [gl.TRIANGLES];
            for(let poly of block.polygons) {
                const positions = new DataView(block.vtxPositions);
                const color = [
                    Math.trunc((((poly._06 >> 11) & 0x1F) / 31) * 255),
                    Math.trunc((((poly._06 >>  5) & 0x3F) / 63) * 255),
                    Math.trunc((((poly._06 >>  0) & 0x1F) / 31) * 255),
                    0xC0];
                for(let i=0; i<3; i++) {
                    let pIdx = poly.vtxs[i] * 6;
                    //division by 8 is hardcoded, not derived from POSSHFT
                    vtxs.push({
                        POS: [
                            positions.getInt16(pIdx)   / 8,
                            positions.getInt16(pIdx+2) / 8,
                            positions.getInt16(pIdx+4) / 8,
                        ],
                        COL0: color, COL1: color, id: -1,
                    });
                }
            }
            batch.addVertices(...vtxs);
        }
        gx.executeBatch(batch);
    }

    _drawPolyGroups() {
        /** Draw poly group bounds. */
        const gx = this.gx;
        const gl = this.gx.gl;

        if(this._isDrawingForPicker) return;
        if(this._polyGroupBatch) {
            gx.executeBatch(this._polyGroupBatch);
            return;
        }
        const batch = new RenderBatch(gx);
        this._polyGroupBatch = batch;

        //blending on, face culling off
        batch.addFunction(() => { gx.disableTextures(GX.BlendMode.BLEND, false) });
        for(let iBlock=0; iBlock < this.map.blocks.length; iBlock++) {
            const block = this.map.blocks[iBlock];
            if(!block || (block.mod >= 0xFF) || !block.polyGroups) continue;
            batch.addFunction(() => { this._setMtxForBlock(block) });

            let vtxs = [gl.TRIANGLES];
            vtxs.length += block.polyGroups.length * 36;
            let iVtx = 1;
            for(let group of block.polyGroups) {
                const color = [
                    //neither of these work well. they're nearly always 0.
                    Math.trunc((((group.id >> 5) & 0x07) / 7) * 255),
                    Math.trunc((((group.id >> 2) & 0x07) / 7) * 255),
                    Math.trunc((((group.id >> 0) & 0x03) / 3) * 255),
                    //Math.trunc((((group.flags >> 11) & 0x1F) / 31) * 255),
                    //Math.trunc((((group.flags >>  5) & 0x3F) / 63) * 255),
                    //Math.trunc((((group.flags >>  0) & 0x1F) / 31) * 255),
                    0x40];
                let box = makeBox(gl,
                    [group.x1, group.y1, group.z1],
                    [group.x2, group.y2, group.z2],
                    -1, 0x40);
                //box.shift(); //remove TRIANGLES
                //vtxs.splice(iVtx, 36, ...box);
                //iVtx += 36;
                //console.log("polyGroup",group, box);
                batch.addVertices(...box);
            }
            //batch.addVertices(...vtxs);
        }
        gx.executeBatch(batch);
    }

    _setMtxForBlock(block) {
        /** Set the ModelView matrix to position at a block.
         *  @param {MapBlock} block Block to position at.
         */
        let mv = mat4.clone(this.gx.context.matModelView);
        if(block) {
            const offsX = block.x*MAP_CELL_SIZE;
            const offsY = block.header.yOffset;
            const offsZ = block.z*MAP_CELL_SIZE;
            mat4.translate(mv, mv, vec3.fromValues(offsX, offsY, offsZ));
        }
        this.gx.setModelViewMtx(mv);
    }

    _getObjAt(x, y) {
        /** Get object at given screen coords.
         *  @param {integer} x X coordinate relative to canvas.
         *  @param {integer} y Y coordinate relative to canvas.
         *  @returns {object} Dict explaining what's at this coordinate,
         *   or null.
         */
        const id = this.gx.context.readPickBuffer(x, y);
        let   obj = this.gx.getPickerObj(id);
        if(obj == undefined) obj = null;
        console.log("pick", x, y, hex(id,8), id, obj);
        return obj;
    }
}
