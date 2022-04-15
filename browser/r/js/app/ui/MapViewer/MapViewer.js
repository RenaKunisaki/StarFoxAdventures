import Game, {MAP_CELL_SIZE} from "../../../game/Game.js";
import { assertType } from "../../../Util.js";
import { E, clearElement } from "../../../lib/Element.js";
import { hex } from "../../../Util.js";
import Context from "../gl/Context.js";
import GX from "../gl/gx/GX.js";
import BlockRenderer from "../../../game/map/BlockRenderer.js";
import ViewController from "../gl/ui/ViewController.js";
import Grid from "./Grid.js";
import Stats from "./Stats.js";
import LayerChooser from "./LayerChooser.js";
import InfoWidget from "./InfoWidget.js";
import RenderBatch from "../gl/gx/RenderBatch.js";
import EventHandler from "./EventHandler.js";
import ObjectRenderer from "./ObjectRenderer.js";

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
        this.layers    = { //which layers to show
            mainGeometry:       true,  //normal geometry
            waterGeometry:      true,  //translucent polygons
            reflectiveGeometry: true,  //opaque reflectve polygons
            hiddenGeometry:     false, //normally hidden polygons
            actNo:              0,  //act number (which objects)
            hiddenObjects:      false, //normally hidden objects
        };

        this.grid            = new Grid(this);
        this.stats           = new Stats(this);
        this.layerChooser    = new LayerChooser(this);
        this.infoWidget      = new InfoWidget(this);
        this.eSidebar        = E.div('sidebar');
        this._eventHandler   = new EventHandler(this);

        this._prevMousePos  = [0, 0];
        this._mouseStartPos = null;
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

        this.gx.resetPicker();
        this._objectRenderer.reset();
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

    redraw() {
        if(this._pendingDraw) return;
        this._pendingDraw = true;
        window.requestAnimationFrame(() => {
            this._pendingDraw = false;
            //this.grid.refresh();
            this.context.redraw();
            //this.stats.refresh();
            if(!this._updatedStats) {
                this._updatedStats = true;
                this.stats.refresh();
            }
        });
    }

    _findABlock() {
        /** Find a block to start at. */
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

    _draw(isPicker) {
        /** Draw the map. Called by Context. */
        if(!this.map) return;

        this._isDrawingForPicker = isPicker;

        //const tStart = performance.now();
        const blockStreams = [
            ['main', this.layers.mainGeometry],
            ['water', this.layers.waterGeometry],
            ['reflective', this.layers.reflectiveGeometry],
        ];

        const blockStats = {totals:{}};
        this._beginRender();
        this._drawBlocks(blockStats, blockStreams);
        this._objectRenderer.drawObjects(isPicker);
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
            showHidden: this.layers.hiddenGeometry,
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
