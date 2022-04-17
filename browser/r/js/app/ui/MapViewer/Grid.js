import {MAP_CELL_SIZE} from "../../../game/Game.js";
import { clearElement, E } from "../../../lib/Element.js";

export default class Grid {
    /** Widget displaying the grid of this map's blocks. */
    constructor(mapViewer) {
        this.mapViewer = mapViewer;
        this.game      = mapViewer.game;
        this.app       = mapViewer.game.app;
        this.curBlock  = null;
        this._tbl      = E.table();
        this.element   = E.details('map-grid',
            E.summary(null, "Grid"),
            this._tbl,
        );
    }

    refresh() {
        const map = this.mapViewer.map;
        const curBlock = this.mapViewer.curBlock;

        //build the grid
        clearElement(this._tbl);
        for(let z=map.sizeZ-1; z>=0; z--) {
            let row = E.tr();
            for(let x=0; x<map.sizeX; x++) {
                let cell;
                let block = map.getBlock(x, z);
                let name  = "(no block)";
                if(block) {
                    block.load(this.mapViewer.gx);
                    cell = E.td('block',
                        E.div(null, block.mod), E.div(null, block.sub));
                    if(block == curBlock) cell.classList.add('current');
                    if(!block.load(this.mapViewer.gx)) {
                        cell.classList.add('invalid');
                        name = "(missing)";
                    }
                    else name = block.header.name;
                    cell.addEventListener('click', e => {
                        const y = (block && block.header ? block.header.yOffset : 0);
                        this.mapViewer.curBlock = block;
                        this.mapViewer.viewController.set({pos:{
                            x: MAP_CELL_SIZE * x,
                            y: y + 1000,
                            z: MAP_CELL_SIZE * z,
                        }, rot:{x:90, y:180, z:0}});
                        this.mapViewer.reset();
                        this.mapViewer.redraw();
                    })
                }
                else cell = E.td('empty');
                let rx = x - map.originX;
                let rz = z - map.originZ;
                if(!(rx || rz)) cell.classList.add('origin');
                cell.setAttribute('title', `${rx},${rz}: ${name}`);
                row.append(cell);
            }
            this._tbl.append(row);
        }
    }
}
