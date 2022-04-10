import { clearElement, E } from "../../../lib/Element.js";
//import { DrawOpNames } from "../gl/gx/DlistParser.js";

const streamNames = { //key => display name
    main: "Main",
    water: "Water",
    reflective: "Refl",
};
const streamOrder = ['main', 'water', 'reflective'];
const statNames = {
    nVtxs: "Vtxs",
    nPolys: "Polys",
    nDrawCmds: "Ops",
};
const statOrder = ['nVtxs', 'nPolys', 'nDrawCmds'];

export default class Stats {
    /** Widget displaying render stats. */
    constructor(mapViewer) {
        this.mapViewer = mapViewer;
        this.game      = mapViewer.game;
        this.app       = mapViewer.game.app;
        this._tbl      = E.table();
        this._tBlock   = E.table();
        this.element   = E.details('render-stats', {open:'open'},
            E.summary(null, "Stats"),
            this._tbl, this._tBlock,
        );

        //make header
        const tr = E.tr(E.th(null, "Drawn"));
        for(const key of streamOrder) {
            tr.append(E.th(null, streamNames[key]));
        }
        tr.append(E.th(null, "Total"));
        this._tbl.append(tr);

        //make rows
        this._statElems = {};
        for(const stat of statOrder) {
            const tr = E.tr(E.th(null, statNames[stat]));
            for(const key of streamOrder) {
                const td = E.td('number');
                this._statElems[`${stat}.${key}`] = td;
                tr.append(td);
            }
            const td = E.td('number');
            this._statElems[`${stat}.total`] = td;
            tr.append(td);
            this._tbl.append(tr);
        }
        this._statElems.xMin = E.td('float x-coord coord', {colspan:2});
        this._statElems.xMax = E.td('float x-coord coord', {colspan:2});
        this._statElems.yMin = E.td('float y-coord coord', {colspan:2});
        this._statElems.yMax = E.td('float y-coord coord', {colspan:2});
        this._statElems.zMin = E.td('float z-coord coord', {colspan:2});
        this._statElems.zMax = E.td('float z-coord coord', {colspan:2});
        this._tbl.append(
            E.tr(E.th(null, "Geometry Bounds", {colspan:5})),
            E.tr(E.th(null, "X"), this._statElems.xMin, this._statElems.xMax),
            E.tr(E.th(null, "Y"), this._statElems.yMin, this._statElems.yMax),
            E.tr(E.th(null, "Z"), this._statElems.zMin, this._statElems.zMax),
        );
    }

    updateDrawCounts(blocks) {
        const curBlock = this.mapViewer.curBlock;
        const gb = {
            xMin: 999999999, xMax: -999999999,
            yMin: 999999999, yMax: -999999999,
            zMin: 999999999, zMax: -999999999,
        };
        const totals = {allStreams:{}};
        for(let stat of statOrder) totals.allStreams[stat] = 0;
        for(let stream of streamOrder) {
            totals[stream] = {};
            for(let stat of statOrder) totals[stream][stat] = 0;
            for(let block of blocks[stream]) {
                if(block.block == curBlock) {
                    //...
                }
                for(let stat of statOrder) {
                    totals[stream][stat] += block[stat];
                }
                gb.xMin = Math.min(gb.xMin, block.geomBounds.xMin);
                gb.xMax = Math.max(gb.xMax, block.geomBounds.xMax);
                gb.yMin = Math.min(gb.yMin, block.geomBounds.yMin);
                gb.yMax = Math.max(gb.yMax, block.geomBounds.yMax);
                gb.zMin = Math.min(gb.zMin, block.geomBounds.zMin);
                gb.zMax = Math.max(gb.zMax, block.geomBounds.zMax);
            }
            for(let stat of statOrder) {
                this._statElems[`${stat}.${stream}`].innerText = totals[stream][stat];
            }
        }
        for(let stream of streamOrder) {
            for(let stat of statOrder) {
                totals.allStreams[stat] += totals[stream][stat];
            }
        }
        for(let stat of statOrder) {
            this._statElems[`${stat}.total`].innerText = totals.allStreams[stat];
        }
        this._statElems.xMin.innerText = gb.xMin.toFixed(2);
        this._statElems.xMax.innerText = gb.xMax.toFixed(2);
        this._statElems.yMin.innerText = gb.yMin.toFixed(2);
        this._statElems.yMax.innerText = gb.yMax.toFixed(2);
        this._statElems.zMin.innerText = gb.zMin.toFixed(2);
        this._statElems.zMax.innerText = gb.zMax.toFixed(2);
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
        const Int = (name, val) => E.tr(E.th(null, name),
            E.td('int', val.toLocaleString(), {colspan:2}),
        );

        //build the table
        if(!(curBlock && curBlock.header)) return;
        clearElement(this._tBlock).append(
            Int('RenderOpsMain', curBlock.header.nRenderInstrsMain),
            Int('RenderOpsRefl', curBlock.header.nRenderInstrsReflective),
            Int('RenderOpsWatr', curBlock.header.nRenderInstrsWater),
            Int('VtxPositions',  curBlock.header.nVtxs),
            Int('Unknown',       curBlock.header.nUnk),
            Int('Colors',        curBlock.header.nColors),
            Int('Textures',      curBlock.header.nTextures),
            Int('TexCoords',     curBlock.header.nTexCoords),
            Int('Shaders',       curBlock.header.nShaders),
            Int('Polys',         curBlock.header.nPolygons),
            Int('PolyGroups',    curBlock.header.nPolyGroups),
            Int('Dlists',        curBlock.header.nDlists),
            Int('Hits',          curBlock.header.nHits),
            Int('yMin',          curBlock.header.yMin),
            Int('yMax',          curBlock.header.yMax),
            Int('yOffset',       curBlock.header.yOffset),
            E.tr(E.th(null, 'BlockName'),
                E.td('string', curBlock.header.name, {colspan:2}),
            ),

            //Int('VtxsDrawn', stats.nVtxs),
            //Int('PolysDrawn', stats.nPolys),
            //Int('DrawOps', stats.nDrawCmds),
            //Int('Dlists', stats.nDlists),
        );
        /* for(const [mode, count] of Object.entries(stats.drawModeCount)) {
            this._tbl.append(E.tr(E.th(null, DrawOpNames[mode]),
                E.td('int', count.toLocaleString(), {colspan:2}),
            ));
        } */
    }
}
