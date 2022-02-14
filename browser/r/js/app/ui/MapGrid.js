import { E, clearElement } from "../../lib/Element.js";
import { hex } from "../../Util.js";

const MAP_CELL_SIZE = 640;

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
        this.app.onSaveSlotChanged(slot => this.refresh());
    }

    _onIsoLoaded() {
        this._makeLayerPicker();
        this.refresh();
    }

    _makeLayerPicker() {
        const elem = E.select();
        for(let idx of Object.keys(this.app.game.mapGrid)) {
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

    _getWarps(layer, x, z) {
        let result = [];
        x *= MAP_CELL_SIZE;
        z *= MAP_CELL_SIZE;
        if(this.app.game.warpTab) {
            for(let [idx, warp] of Object.entries(this.app.game.warpTab)) {
                if(warp.x >= x && warp.x < (x+MAP_CELL_SIZE)
                && warp.z >= z && warp.z < (z+MAP_CELL_SIZE)
                && warp.layer == layer) {
                    result.push(parseInt(idx));
                }
            }
        }
        return result;
    }

    refresh() {
        const grid    = this.app.game.mapGrid;
        const elem    = E.table('mapGrid');
        const layerNo = this.eLayerPicker.value;
        const layer   = grid[layerNo];
        const xMin    = Math.min(...Object.keys(layer));
        const xMax    = Math.max(...Object.keys(layer));
        let posK = {x:9999, z:9999, layer:9999};
        let posF = {x:9999, z:9999, layer:9999};
        if(this.app.saveSlot) {
            posK = this.app.saveSlot.charPos[0];
            posF = this.app.saveSlot.charPos[1];
            posK = {
                x:     Math.floor(posK.pos.x/MAP_CELL_SIZE),
                z:     Math.floor(posK.pos.z/MAP_CELL_SIZE),
                layer: posK.mapLayer,
            };
            posF = {
                x:     Math.floor(posF.pos.x/MAP_CELL_SIZE),
                z:     Math.floor(posF.pos.z/MAP_CELL_SIZE),
                layer: posF.mapLayer,
            };
        }

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

                    //XXX check warps for out-of-bounds cells too
                    //or better yet put the elements into an array and
                    //then just iterate the warp list once.
                    let title = cell.map.name;
                    if(block) title = `${title} - mod${block.mod}.${block.sub}`;
                    else title += ' - Out of Bounds';

                    let bg    = mapIdToColor(cell.mapId);
                    let warps = this._getWarps(layerNo, x, z);
                    if(warps.length > 0) {
                        title += '\nWarps: ' +
                            [...(warps.map(w => `0x${hex(w,2)}`))].join(', ');
                        cls += ' hasWarp';
                    }

                    if(layerNo == posK.layer && x == posK.x && z == posK.z) {
                        cls += ' krystal';
                        title += "\nKrystal's saved position";
                    }
                    if(layerNo == posF.layer && x == posF.x && z == posF.z) {
                        cls += ' fox';
                        title += "\nFox's saved position";
                    }

                    td = E.td('cell'+cls, text, {
                        title: title,
                        style: `background-color: rgb(${bg[0]}, ${bg[1]}, ${bg[2]})`,
                    });

                    if(cell.relX == 0) td.classList.add('left');
                    if(cell.relX == cell.map.sizeX-1) td.classList.add('right');
                    if(cell.relZ == 0) td.classList.add('bottom');
                    if(cell.relZ == cell.map.sizeZ-1) td.classList.add('top');
                    if(cell.isOrigin) td.classList.add('origin');
                }
                else {
                    td = E.td('empty', ' ');
                    if(x == 0 || z == 0) td.classList.add('zero');
                }
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
