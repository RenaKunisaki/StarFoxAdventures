import Game from "../../../game/Game.js";
import { assertType } from "../../../Util.js";
import { E, clearElement } from "../../../lib/Element.js";
import { hex } from "../../../Util.js";
import Context from "../gl/Context.js";
import GX from "../gl/gx/GX.js";
import BlockRenderer from "../../../game/map/BlockRenderer.js";
import ViewController from "../gl/ui/ViewController.js";
import Grid from "./Grid.js";

export default class MapViewer {
    /** Renders map geometry. */
    constructor(game) {
        this.game     = assertType(game, Game);
        this.app      = game.app;
        this.element  = document.getElementById('tab-mapView');
        this.canvas   = E.canvas('map-view-canvas');
        this.context  = null;
        this.eMapList = null;
        this.map      = null; //current map
        this.curBlock = null;
        this.grid     = new Grid(this);
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
        this.element.append(this.viewController.element);
        this.canvas.addEventListener('mousemove', e => this._onMouseMove(e));
        this.canvas.addEventListener('mousedown', e => this._onMouseDown(e));
        this.canvas.addEventListener('mouseup',   e => this._onMouseUp  (e));
        //must disable context menu to be able to right-drag
        this.canvas.addEventListener('contextmenu', e => e.preventDefault());

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
        const map = this.game.maps[this.eMapList.value];
        if(!map) {
            console.error("Invalid map selected", this.eMapList.value);
            return;
        }
        console.log("Loading map", map);
        if(!map.dirName) {
            console.error("Map has no directory", map);
            return;
        }
        this.map = map;
        this.grid.refresh();
        this.context.redraw();
    }

    _findABlock() {
        /** Find a block to start at. */
        //if there's one at the origin, prefer it.
        let block = this.map.getBlock(0, 0);
        if(block) return block;

        //find the first non-empty block.
        let iBlock = 0;
        while(iBlock < this.map.blocks.length) {
            if(this.map.blocks[iBlock]
            && this.map.blocks[iBlock].mod < 0xFF) break;
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
        this.gx.reset();
        this.blockRenderer = new BlockRenderer(this.gx);

        //XXX move this
        const gl = this.gx.gl;
        gl.uniform1i(this.gx.programInfo.uniforms.useId, 0);
        gl.uniform1i(this.gx.programInfo.uniforms.useLights,
            this.context.lights.enabled ? 1 : 0);
        gl.uniform1i(this.gx.programInfo.uniforms.useTexture,
            this.context.enableTextures ? 1 : 0);

        if(!this.curBlock) {
            this.curBlock = this._findABlock();
            if(!this.curBlock) return;
            this.grid.refresh();
        }

        this.curBlock.load(); //ensure block model is loaded
        //console.log("map block", this.curBlock);
        this.gx.beginRender();
        this.blockRenderer.render(this.curBlock, 'main');
        //console.log("block render OK", this.gx.stats);
        //this.gx.printStats();
        //console.log("GX logs:", this.gx.program.getLogs());
    }

    _onMouseDown(event) {
        event.preventDefault();
    }
    _onMouseUp(event) {
        event.preventDefault();
    }

    _onMouseMove(event) {
        //buttons are bitflag: 1=left 2=right 4=mid 8=back 16=fwd
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
}
