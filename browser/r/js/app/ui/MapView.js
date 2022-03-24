import Game from "../../game/Game.js";
import { assertType } from "../../Util.js";
import { E, clearElement } from "../../lib/Element.js";
import { hex } from "../../Util.js";
import Context from "./gl/Context.js";
import GX from "./gl/gx/GX.js";

export default class MapView {
    /** Renders map geometry. */
    constructor(game) {
        this.game     = assertType(game, Game);
        this.app      = game.app;
        this.element  = document.getElementById('tab-mapView');
        this.canvas   = E.canvas('map-view-canvas');
        this.context  = null;
        this.eMapList = null;
        this.curMap   = null; //current map
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
        )
        if(!this.context) {
            this.context = new Context(this.canvas, () => this._draw());
            this.gx = new GX(this.context);
            this.context.init().then(() => this.context.redraw());
        }
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
        const map = this.game.maps[this.eMapList.value];
        if(!map) {
            console.error("No map selected");
            return;
        }
        if(!map.dirName) {
            console.error("Map has no directory");
            return;
        }
        this.curMap = map;
    }

    _draw() {
        /** Draw the map. Called by Context. */
        const gl = this.context.gl;
    }
}
