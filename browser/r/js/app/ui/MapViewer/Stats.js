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

const blockPropNames = {
    nRenderInstrsMain:       'RenderOpsMain',
    nRenderInstrsReflective: 'RenderOpsRefl',
    nRenderInstrsWater:      'RenderOpsWatr',
    nVtxs:                   'VtxPositions',
    nUnk:                    'Unknown',
    nColors:                 'Colors',
    nTextures:               'Textures',
    nTexCoords:              'TexCoords',
    nShaders:                'Shaders',
    nPolygons:               'Polys',
    nPolyGroups:             'PolyGroups',
    nDlists:                 'Dlists',
    nHits:                   'Hits',
};
const blockPropOrder = ['nRenderInstrsMain', 'nRenderInstrsReflective',
    'nRenderInstrsWater', 'nVtxs', 'nUnk', 'nColors', 'nTextures',
    'nTexCoords', 'nShaders', 'nPolygons', 'nPolyGroups', 'nDlists',
    'nHits',
];

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

        this._blockPropElems = {};
        this._tbl.append(E.tr(
            E.th(null, "BlockInfo"),
            E.th(null, "Selected", {colspan:2}),
            E.th(null, "Total",    {colspan:2}),
        ));
        for(const op of blockPropOrder) {
            const eCur   = E.td('number', {colspan:2});
            const eTotal = E.td('number', {colspan:2});
            this._blockPropElems[op] = {cur:eCur, total:eTotal};
            this._tbl.append(E.tr(null,
                E.th(null, blockPropNames[op]), eCur, eTotal));
        }
    }

    updateDrawCounts(blocks) {
        //update things that could change on each draw.
        //XXX really they should only change depending on settings like
        //which things are enabled, so we don't really need to do this
        //on every redraw.
        const curBlock = this.mapViewer.curBlock;
        const gb = {
            xMin: 999999999, xMax: -999999999,
            yMin: 999999999, yMax: -999999999,
            zMin: 999999999, zMax: -999999999,
        };
        const totals = {allStreams:{}};
        for(const op of blockPropOrder) totals[op] = 0;
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
                this._statElems[`${stat}.${stream}`].innerText =
                    totals[stream][stat].toLocaleString();
            }
        }

        for(let stream of streamOrder) {
            for(let stat of statOrder) {
                totals.allStreams[stat] += totals[stream][stat];
            }
        }
        for(let stat of statOrder) {
            this._statElems[`${stat}.total`].innerText =
                totals.allStreams[stat].toLocaleString();
        }

        this._statElems.xMin.innerText = gb.xMin.toLocaleString();
        this._statElems.xMax.innerText = gb.xMax.toLocaleString();
        this._statElems.yMin.innerText = gb.yMin.toLocaleString();
        this._statElems.yMax.innerText = gb.yMax.toLocaleString();
        this._statElems.zMin.innerText = gb.zMin.toLocaleString();
        this._statElems.zMax.innerText = gb.zMax.toLocaleString();
    }

    refresh() {
        //update things that only change when switching maps/blocks.
        const map = this.mapViewer.map;
        const curBlock = this.mapViewer.curBlock;

        let xMin = 999999999, xMax = -999999999;
        let zMin = 999999999, zMax = -999999999;
        const totals = {};
        for(const prop of blockPropOrder) totals[prop] = 0;
        for(let block of map.blocks) {
            if((!block) || block.mod >= 0xFF) continue;
            block.load(); //no-op if already loaded
            xMin = Math.min(xMin, block.x);
            zMin = Math.min(zMin, block.z);
            xMax = Math.max(xMax, block.x);
            zMax = Math.max(zMax, block.z);
            for(const prop of blockPropOrder) {
                if(block == curBlock) {
                    this._blockPropElems[prop].cur.innerText =
                        block.header[prop].toLocaleString();
                }
                totals[prop] += block.header[prop];
            }
        }
        for(const prop of blockPropOrder) {
            this._blockPropElems[prop].total.innerText = totals[prop].toLocaleString();
        }
    }
}
