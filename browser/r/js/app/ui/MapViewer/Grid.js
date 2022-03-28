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

        //ensure blocks are loaded, and find bounds
        let xMin = 999999, xMax = -999999;
        let zMin = 999999, zMax = -999999;
        for(let block of map.blocks) {
            if((!block) || block.mod >= 0xFF) continue;
            block.load(); //no-op if already loaded
            xMin = Math.min(xMin, block.x);
            zMin = Math.min(zMin, block.z);
            xMax = Math.max(xMax, block.x);
            zMax = Math.max(zMax, block.z);
        }

        //build the grid
        clearElement(this._tbl);
        for(let z=zMax; z>=zMin; z--) {
            let row = E.tr();
            for(let x=xMin; x<=xMax; x++) {
                let cell;
                let block = map.getBlock(x, z);
                if(block) {
                    cell = E.td('block',
                        E.div(null, block.mod), E.div(null, block.sub));
                    if(block == curBlock) cell.classList.add('current');
                    //XXX add listener to jump to it on click
                }
                else cell = E.td('empty');
                row.append(cell);
            }
            this._tbl.append(row);
        }
    }
}
