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
import { makeCube } from "../gl/GlUtil.js";

export default class MapViewer {
    /** Renders map geometry. */
    constructor(game) {
        this.game      = assertType(game, Game);
        this.app       = game.app;
        this.element   = document.getElementById('tab-mapView');
        this.canvas    = E.canvas('map-view-canvas');
        this.context   = null;
        this.eMapList  = null;
        this.map       = null; //current map
        this.curBlock  = null;
        this.grid            = new Grid(this);
        this.stats           = new Stats(this);
        this.layerChooser    = new LayerChooser(this);
        this.infoWidget      = new InfoWidget(this);
        this.eSidebar        = E.div('sidebar');
        this._eventHandler   = new EventHandler(this);
        this._warpBatch      = null;
        this._warpBatchPicker= null;

        this._isFirstDrawAfterLoadingMap = true;
        this.app.onIsoLoaded(iso => this.refresh());
    }

    refresh() {
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

        this.blockRenderer = new BlockRenderer(this.gx);
        this._objectRenderer = new ObjectRenderer(this);

        this.context.canvas.focus();
        this._reloadMap();
    }

    _buildMapList() {
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
        this.eMapList.addEventListener('change', e => this._reloadMap());
    }

    _reloadMap() {
        /** Load and display the currently selected map. */
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
        }

        this._warpBatch = null;
        this._warpBatchPicker = null;
        this._isFirstDrawAfterLoadingMap = true;
        this.gx.resetPicker();
        this._objectRenderer.reset();
        this.blockRenderer.reset();
        this.curBlock = this._findABlock();
        this.reset();
        this.layerChooser.refresh();
        this.redraw();
    }

    reset() {
        /** Reset for displaying another block. */
        this.grid.refresh();
        this._updatedStats = false;
        this._pendingDraw  = false;
    }

    async _loadMap() {
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

    async redraw() {
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
        await this._drawObjects();
        if(LC.getLayer('warps')) this._drawWarps();
        if(LC.getLayer('blockBounds') && !this._isDrawingForPicker) {
            this._drawBlockBounds();
        }
        this._finishRender(blockStats, blockStreams);
        //console.log("block render OK", this.gx.context.stats);
        //console.log("GX logs:", this.gx.program.getLogs());
        //if(isPicker) console.log("picker IDs", this.gx.pickerObjs);
    }

    _beginRender() {
        //const gl = this.gx.gl;
        if(!this.curBlock) {
            this.curBlock = this._findABlock();
            if(!this.curBlock) return;
            this.reset();
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
        const gl = this.gx.gl;
        block.load(this.gx); //ensure block model is loaded

        const offsX = block.x*MAP_CELL_SIZE;
        const offsY = block.header.yOffset;
        const offsZ = block.z*MAP_CELL_SIZE;
        let mv = mat4.clone(this.gx.context.matModelView);
        mat4.translate(mv, mv, vec3.fromValues(offsX, offsY, offsZ));
        this.gx.setModelViewMtx(mv);

        const batch = this.blockRenderer.render(block, stream, params);
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

    _drawBlockBounds() {
        /** Draw the bounding boxes for each block. */
        const gx = this.gx;
        const gl = this.gx.gl;
        const batch = new RenderBatch(gx);

        let mv = mat4.clone(this.gx.context.matModelView);
        this.gx.setModelViewMtx(mv);

        batch.addFunction(() => {
            gl.disable(gl.CULL_FACE);
            gl.enable(gl.BLEND);
            gx.setBlendMode(GX.BlendMode.BLEND, GX.BlendFactor.SRCALPHA,
                GX.BlendFactor.INVSRCALPHA, GX.LogicOp.NOOP);
            gx.setZMode(true, GX.CompareMode.LEQUAL, true);

            for(let i=0; i<this.gx.MAX_TEXTURES; i++) {
                gl.activeTexture(gl.TEXTURE0 + i);
                this.gx.whiteTexture.bind();
                gl.uniform1i(this.gx.programInfo.uniforms.uSampler[i], i);
            }
        });

        //not using makeCube() because it's not necessarily a cube
        const vtxIdxs = [
            //top          //front        //left
            0,1,3, 1,2,3,  3,2,6, 3,6,7,  0,3,4, 3,7,4,
            //right        //bottom       //back
            2,1,5, 2,5,6,  7,5,4, 7,6,5,  1,0,5, 0,4,5,
        ];
        const colors = [ //r, g, b, a
            [0x00, 0x00, 0x00, 0x7F], //[0] top left back
            [0xFF, 0x00, 0x00, 0x7F], //[1] top right back
            [0xFF, 0xFF, 0x00, 0x7F], //[2] top right front
            [0x00, 0xFF, 0x00, 0x7F], //[3] top left front
            [0x00, 0x00, 0xFF, 0x7F], //[4] bot left back
            [0xFF, 0x00, 0xFF, 0x7F], //[5] bot right back
            [0xFF, 0xFF, 0xFF, 0x7F], //[6] bot right front
            [0x00, 0xFF, 0xFF, 0x7F], //[7] bot left front
        ];
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

            const vtxPositions = [ //x, y, z
                //back left, back right, front right, front left
                [x1, y1, z1], [x2, y1, z1], [x2, y2, z1], [x1, y2, z1], //top
                [x1, y1, z2], [x2, y1, z2], [x2, y2, z2], [x1, y2, z2], //bot
            ];

            const vtxs = [this.gx.gl.TRIANGLES];
            for(const idx of vtxIdxs) {
                vtxs.push({
                    POS:  vtxPositions[idx],
                    COL0: colors[idx],
                    COL1: colors[idx],
                    id:   -1,
                });
            }
            batch.addVertices(...vtxs);
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
                gl.enable(gl.BLEND);
                gx.setBlendMode(GX.BlendMode.BLEND, GX.BlendFactor.SRCALPHA,
                    GX.BlendFactor.INVSRCALPHA, GX.LogicOp.NOOP);
            });
        }
        batch.addFunction(() => {
            gl.enable(gl.CULL_FACE);
            gx.setZMode(true, GX.CompareMode.LEQUAL, true);
            for(let i=0; i<this.gx.MAX_TEXTURES; i++) {
                gl.activeTexture(gl.TEXTURE0 + i);
                this.gx.whiteTexture.bind();
                gl.uniform1i(this.gx.programInfo.uniforms.uSampler[i], i);
            }
        });

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
        let mv = mat4.clone(this.gx.context.matModelView);
        mat4.translate(mv, mv, vec3.fromValues(
            (this.map.originX * MAP_CELL_SIZE), 0,
            (this.map.originZ * MAP_CELL_SIZE) ));
        this.gx.setModelViewMtx(mv);
        const batch = await this._objectRenderer.drawObjects(
            this.layerChooser.getLayer('actNo'), this._isDrawingForPicker);
        if(batch) this.gx.executeBatch(batch);
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
