import DaeWriter from "../gl/dae/DaeWriter.js";
import { downloadXml } from "../../../Util.js";
import { createElement, E } from "../../../lib/Element.js";
import BlockRenderer from "./BlockRenderer.js";
import RenderBatch from "../gl/gx/RenderBatch.js";

export default class MapExporter {
    /** Exports map to 3D model file. */

    constructor(game, gx, map) {
        this.game = game;
        this.gx   = gx;
        this.map  = map;
    }

    export() {
        this.writer = new DaeWriter();

        for(let iBlock=0; iBlock < this.map.blocks.length; iBlock++) {
            const block = this.map.blocks[iBlock];
            if(!block || (block.mod >= 0xFF) || !block.load()) continue;
            this._blockToGeometry(block);
        }

        downloadXml(this.writer.toXml(), 'map', 'model/vnd.collada+xml', true);
    }

    _blockToGeometry(block) {
        const gl = this.gx.gl;
        const id = block.header.name;
        console.assert(id != undefined);

        const positions = new Int16Array(block.vtxPositions);
        const eSrcPos = E.source({id:`${id}.positions`},
            E.float_array(null, {id:`${id}.positions.array`}, positions.join(' ')),
            E.technique_common(null,
                E.accessor(null, {
                    source: `#${id}.positions.array`,
                    count: positions.length,
                    stride: 3,
                })
            ),
        );
        const eVtxs = E.vertices(null, {id:`${id}.vertices`},
            E.input(null, {semantic:'POSITION', source:`#${id}.positions`})
        );
        const eMesh = E.mesh(null, {id:id}, eSrcPos, eVtxs);

        const renderer = new BlockRenderer(this.game, this.gx);
        const batch = renderer.parse(block, 'main');
        this._parseRenderBatch(eMesh, batch);

        this.writer.addGeometry(id, eMesh);
    }

    _parseRenderBatch(eMesh, batch) {
        //this doesn't work because the batch only contains functions,
        //presumably because they call display lists.
        //we need to hook into GX for this...
        for(let op of batch.ops) {
            if(op instanceof RenderBatch) {
                this._parseRenderBatch(eMesh, op);
                continue;
            }
            else if(typeof(op) == 'function') continue;

            let name;
            const [mode, idx, count] = op;
            switch(mode) {
                case gl.TRIANGLES:      name = 'triangles'; break;
                case gl.TRIANGLE_FAN:   name = 'trifans'; break;
                case gl.TRIANGLE_STRIP: name = 'tristrips'; break;
                case gl.LINES:          name = 'lines'; break;
                case gl.LINE_STRIP:     name = 'linestrips'; break;
                default:
                    debugger;
                    throw new Error("Unsupported draw mode");
            }
            const idxs = batch._idxs.slice(idx, idx+count);
            const p = E.p(null, idxs.join(' '));
            const eOp = createElement(name, {count:count},
                E.input({
                    semantic: 'VERTEX',
                    source: `#${id}.vertices`,
                    offset: 0,
                }, p));
            eMesh.append(eOp);
        }
    }
}
