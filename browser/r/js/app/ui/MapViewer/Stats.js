import { clearElement, E } from "../../../lib/Element.js";
import { DrawOpNames } from "../gl/gx/DlistParser.js";

export default class Stats {
    /** Widget displaying render stats. */
    constructor(mapViewer) {
        this.mapViewer = mapViewer;
        this.game      = mapViewer.game;
        this.app       = mapViewer.game.app;
        this._tbl      = E.table();
        this.element   = E.details('render-stats',
            E.summary(null, "Stats"),
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
        const stats = this.mapViewer.gx.stats;

        const Int = (name, val) => E.tr(E.th(null, name),
            E.td('int', val.toLocaleString(), {colspan:2}),
        );

        //build the table
        clearElement(this._tbl).append(
            Int('RenderOpsMain', curBlock.header.nRenderInstrsMain),
            Int('RenderOpsRefl', curBlock.header.nRenderInstrsReflective),
            Int('RenderOpsWatr', curBlock.header.nRenderInstrsWater),
            Int('VtxPositions', curBlock.header.nVtxs),
            Int('Colors', curBlock.header.nColors),
            Int('Textures', curBlock.header.nTextures),
            Int('TexCoords', curBlock.header.nTexCoords),
            Int('Shaders', curBlock.header.nShaders),
            Int('Polys', curBlock.header.nPolygons),
            Int('PolyGroups', curBlock.header.nPolyGroups),
            Int('Hits', curBlock.header.nHits),
            E.tr(E.th(null, 'BlockName'),
                E.td('string', curBlock.header.name, {colspan:2}),
            ),
            E.tr(E.th(null, 'X Bound'),
                E.td('float x-coord coord', xMin.toFixed(2)),
                E.td('float x-coord coord', xMax.toFixed(2)),
            ),
            E.tr(E.th(null, 'Z Bound'),
                E.td('float z-coord coord', zMin.toFixed(2)),
                E.td('float z-coord coord', zMax.toFixed(2)),
            ),
            Int('VtxsDrawn', stats.nVtxs),
            Int('Draw Ops', stats.nDrawCmds),
            Int('Dlists', stats.nDlists),
        );
        for(const [mode, count] of Object.entries(stats.drawModeCount)) {
            this._tbl.append(E.tr(E.th(null, DrawOpNames[mode]),
                E.td('int', count.toLocaleString(), {colspan:2}),
            ));
        }
    }
}
