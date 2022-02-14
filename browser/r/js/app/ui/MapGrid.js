import { E, clearElement } from "../../lib/Element.js";
import { hex } from "../../Util.js";
import parseMapGrid from "../../types/MapGrid.js";

function mapIdToColor(id) {
    //arbitrary function for assigning map IDs a color.
    //I just picked something that looked nice.
    let n = id + 1;
    id = 0;
    for(let i=0; i<8; i++) id |= ((n >> i) & 1) << i;
    let r = ((id >> 1) & 3) / 3;
    let g = ((id >> 3) & 7) / 7;
    let b = ((id >> 6) & 3) / 3;
    r = Math.floor(r * 255);
    g = Math.floor(g * 255);
    b = Math.floor(b * 255);
    return [r, g, b];
}

export default class MapGrid {
    /** Displays global map grid.
     */
    constructor(app) {
        this.app     = app;
        this.layer   = 0;
        this.element = document.getElementById('tab-mapGrid');
        this.app.onIsoLoaded(iso => this._onIsoLoaded());
    }

    _onIsoLoaded() {
        this.grid = parseMapGrid(this.app);
        console.log("grid", this.grid);
        this._makeLayerPicker();
        this.refresh();
    }

    _makeLayerPicker() {
        const elem = E.select();
        for(let idx of Object.keys(this.grid)) {
            elem.append(E.option(null, `Layer ${idx}`, {value:idx}));
        }
        elem.value = 0;
        elem.addEventListener('change', e => this.refresh());
        this.eLayerPicker = elem;
    }

    _getCell(layer, x, z) {
        let cell = layer[x];
        if(cell) cell = cell[z];
        return cell;
    }

    refresh() {
        const elem  = E.table('mapGrid');
        console.log("show layer", this.eLayerPicker.value);
        const layer = this.grid[this.eLayerPicker.value];
        const xMin  = Math.min(...Object.keys(layer));
        const xMax  = Math.max(...Object.keys(layer));

        //find range
        let zMin = 999999, zMax = -999999;
        for(let x=xMin; x<=xMax; x++) {
            if(layer[x]) {
                zMin = Math.min(zMin, ...Object.keys(layer[x]));
                zMax = Math.max(zMax, ...Object.keys(layer[x]));
            }
        }

        //make top row
        let tr = E.tr('header', E.th());
        for(let x=xMin; x<=xMax; x++) {
            tr.append(E.th('coord', x));
        }
        tr.append(E.th());
        elem.append(tr);

        for(let z=zMax; z>=zMin; z--) { //upside down
            tr = E.tr('row', E.th('coord', z));
            for(let x=xMin; x<=xMax; x++) {
                let cell = this._getCell(layer, x, z);
                let td;
                if(cell != undefined) {
                    let block = cell.block;
                    let text = ' ';
                    let cls  = ' oob';
                    if(block != undefined) {
                        const mod = block.mod.toString().padStart(2);
                        const sub = block.sub.toString().padStart(2);
                        text = E.span(E.div(null, mod), E.div(null, sub));
                        cls  = '';
                    }
                    let bg = mapIdToColor(cell.mapId);
                    td = E.td('cell'+cls, text, {
                        title: cell.map.name,
                        style: `background-color: rgba(${bg[0]}, ${bg[1]}, ${bg[2]}, 0.5)`,
                    });

                    if(cell.relX == 0) td.classList.add('left');
                    if(cell.relX == cell.map.sizeX-1) td.classList.add('right');
                    if(cell.relZ == 0) td.classList.add('bottom');
                    if(cell.relZ == cell.map.sizeZ-1) td.classList.add('top');
                    if(cell.isOrigin) td.classList.add('origin');
                }
                else td = E.td('empty', ' ');
                tr.append(td);
            }
            tr.append(E.th('coord', z));
            elem.append(tr);
        }

        //make bottom row
        tr = E.tr('header', E.th());
        for(let x=xMin; x<=xMax; x++) {
            tr.append(E.th('coord', x));
        }
        tr.append(E.th());
        elem.append(tr);

        clearElement(this.element).append(this.eLayerPicker, elem);
    }
}
