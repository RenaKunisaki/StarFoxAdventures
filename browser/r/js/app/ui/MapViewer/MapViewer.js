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
import RenderBatch from "../gl/gx/RenderBatch.js";

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

        this.grid         = new Grid(this);
        this.stats        = new Stats(this);
        this.layerChooser = new LayerChooser(this);
        this.eSidebar     = E.div('sidebar');

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
        this.context = new Context(this.canvas, () => this._draw());
        await this.context.init();
        console.log("GL ctx init OK");

        this.gx = new GX(this.context);
        await this.gx.loadPrograms();

        this.viewController = new ViewController(this.context);
        this.canvas.addEventListener('mousemove', e => this._onMouseMove (e));
        this.canvas.addEventListener('mousedown', e => this._onMouseDown (e));
        this.canvas.addEventListener('mouseup',   e => this._onMouseUp   (e));
        this.canvas.addEventListener('wheel',     e => this._onMouseWheel(e));
        //must disable context menu to be able to right-drag
        this.canvas.addEventListener('contextmenu', e => e.preventDefault());

        this.eSidebar.append(this.viewController.element,
            this.layerChooser.element,
            this.stats.element,
        );

        this.blockRenderer = new BlockRenderer(this.gx);
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

    _draw() {
        /** Draw the map. Called by Context. */
        if(!this.map) return;
        const tStart = performance.now();
        const blockStreams = [
            ['main', this.layers.mainGeometry],
            ['water', this.layers.waterGeometry],
            ['reflective', this.layers.reflectiveGeometry],
        ];

        const blockStats = {totals:{}};
        this._beginRender();
        this._drawBlocks(blockStats, blockStreams);
        this._drawObjects();
        this._finishRender(blockStats, blockStreams);
        //console.log("block render OK", this.gx.context.stats);
        //console.log("GX logs:", this.gx.program.getLogs());
    }

    _beginRender() {
        this.gx.context.resetStats();
        this.gx.reset();

        const gl = this.gx.gl;
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
        this.gx.beginRender(mtxs);
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

    _drawObjects() {
        const gx  = this.gx;
        const gl  = this.gx.gl;
        const act = this.layers.actNo;
        if(act == 0) return;
        if(!this.map.romList) return;

        const batch = new RenderBatch(this.gx);

        let mv = mat4.clone(gx.context.matModelView);
        mat4.translate(mv, mv, vec3.fromValues(
            (this.map.originX * MAP_CELL_SIZE), 0,
            (this.map.originZ * MAP_CELL_SIZE) ));
        gx.setModelViewMtx(mv);

        gl.enable(gl.CULL_FACE);
        gx.setBlendMode(GX.BlendMode.BLEND, GX.BlendFactor.SRCALPHA,
            GX.BlendFactor.INVSRCALPHA, GX.LogicOp.NOOP);
        gx.setZMode(true, GX.CompareMode.LEQUAL, true);

        batch.addFunction(() => {
            for(let i=0; i<this.gx.MAX_TEXTURES; i++) {
                gl.activeTexture(gl.TEXTURE0 + i);
                this.gx.whiteTexture.bind();
                gl.uniform1i(this.gx.programInfo.uniforms.uSampler[i], i);
            }
        });

        for(let entry of this.map.romList.entries) {
            if(act == 'all' || entry.acts[act]) {
                batch.addFunction(this._drawObject(entry));
            }
        }
        this.gx.executeBatch(batch);
    }

    _drawObject(entry) {
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

        //just draw a cube
        const vtxPositions = [ //x, y, z
            [x-s, y-s, z-s], //[0] top left back
            [x+s, y-s, z-s], //[1] top right back
            [x+s, y+s, z-s], //[2] top right front
            [x-s, y+s, z-s], //[3] top left front
            [x-s, y-s, z+s], //[4] bot left back
            [x+s, y-s, z+s], //[5] bot right back
            [x+s, y+s, z+s], //[6] bot right front
            [x-s, y+s, z+s], //[7] bot left front
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
            });
        }
        batch.addVertices(...vtxs);
        return batch;
    }

    _onMouseDown(event) {
        event.preventDefault();
    }
    _onMouseUp(event) {
        event.preventDefault();
    }

    _onMouseMove(event) {
        //buttons are bitflag: 1=left 2=right 4=mid 8=back 16=fwd
        //viewController.set() will redraw the scene.
        if(event.buttons == 1) { //rotate
            if(this._mouseStartView) {
                this.viewController.set({
                    rot: {
                        x: this._mouseStartView.rot.x + (event.y - this._mouseStartPos[1]),
                        y: this._mouseStartView.rot.y + (event.x - this._mouseStartPos[0]),
                    },
                });
            }
            else {
                this._mouseStartView = this.viewController.get();
                this._mouseStartPos  = [event.x, event.y];
            }
        }
        else if(event.buttons == 2) { //move
            if(this._mouseStartView) {
                this.viewController.set({
                    pos: {
                        x: this._mouseStartView.pos.x + (event.x - this._mouseStartPos[0]),
                        z: this._mouseStartView.pos.z + (event.y - this._mouseStartPos[1]),
                    },
                });
            }
            else {
                this._mouseStartView = this.viewController.get();
                this._mouseStartPos  = [event.x, event.y];
            }
        }
        else this._mouseStartView = null;
        this._prevMousePos = [event.x, event.y];
    }

    _onMouseWheel(event) {
        this.viewController.adjust({pos:{y: event.deltaY}});
    }
}
