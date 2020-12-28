import {E} from './Element.js';
import Table from './Table.js';
import {hex, toHexString, hexStringToData, makeList, makeCollapsibleList, makeDescriptionAndNotes} from './Util.js';

import Context from './gl/Context.js';
import ViewControl from './gl/ViewControl.js';
import LightControl from './gl/LightControl.js';
import VertexBuffer from './gl/gx/VertexBuffer.js';

class Renderer {
    constructor(view) {
        this.view  = view;
        this.ctx   = view.ctx;
    }

    reset() {
        /** Reset all state to default.
         */
        this._didRender = false;
        this.ctx.gx.reset();
    }

    render() {
        const gl = this.ctx.gl;
        const gx = this.ctx.gx;
        this.reset();
        //gx._setShaderMtxs();

        this.vtxBuf = new VertexBuffer(gx);
        gx.program.use();
        gx._setShaderMtxs();
        //this._drawCube({
        //    x:0, y:0, z:0, size:10, fill:true,
        //})
        for(let item of this.view.romlist.items) {
            this._drawCube({
                x: item.objDef.x,
                y: item.objDef.y,
                z: item.objDef.z,
                color: [
                    ( item.objDef.type       & 0x0F) << 4,
                    ((item.objDef.type >> 4) & 0x0F) << 4,
                    ((item.objDef.type >> 8) & 0x0F) << 4,
                    255,
                ],
                size:10, fill:true,
            })
        }
        //this.vtxBuf.bind();
        //gx.program.use();
        //gx._setShaderMtxs();
        //gl.drawArrays(gl.LINE_LOOP, 0, 16*this.view.romlist.items.length);
    }

    renderPickBuffer() {
        //this._renderer.render();
    }

    _drawCube(cube) {
        /** Draw a cube.
         *  @param cube.x X coord of center.
         *  @param cube.y Y coord of center.
         *  @param cube.z Z coord of center.
         *  @param [cube.size=1] width of cube.
         *  @param [cube.color=[255,0,0,255]] cube color.
         *  @param [cube.fill=false] whether to fill rather than outline.
         */
        /*4________5
          /|      /.
         / |     / |
        .-------.  |
        |0 | __1|__|6
        |  /7   | /
        |3/    2|/
        '-------' */
        let first = this.vtxBuf.vtxCount;
        const gl = this.ctx.gl;
        const gx = this.ctx.gx;
        const vtxs = [
            [-1, -1, -1], [ 1, -1, -1], [ 1,  1, -1], [-1,  1, -1],
            [-1, -1,  1], [ 1, -1,  1], [ 1,  1,  1], [-1,  1,  1],
        ];
        let idxs;
        if(cube.fill) idxs = [ //triangles
            0, 1, 2,   0, 2, 3,   4, 5, 6,   4, 6, 7, //-Z, +Z faces
            0, 4, 7,   0, 7, 3,   1, 5, 6,   1, 6, 2, //-X, +X faces
            0, 4, 5,   0, 5, 1,   3, 7, 6,   3, 6, 2, //-Y, +Y faces
        ];
        else idxs = [ //line loop
            //follow the diagram above, connecting all lines.
            //it's not possible to do this without repeating some
            //lines; fortunately this doesn't matter.
            0, 1, 2, 3,  0, 4, 5, 6,  7, 4, 5, 1,  2, 6, 7, 3,
        ];
        const color = cube.color == undefined ? [255,0,0,255] : cube.color;
        const size  = cube.size == undefined ? 1 : cube.size;
        const s = size / 2;
        const x = cube.x - (size/2);
        const y = cube.y - (size/2);
        const z = cube.z - (size/2);
        for(let i of idxs) {
            let xs = vtxs[i][0], ys = vtxs[i][1], zs = vtxs[i][2];
            this.vtxBuf.addVtx({POS:[
                xs < 0 ? x-s : x+s,
                ys < 0 ? y-s : y+s,
                zs < 0 ? z-s : z+s,
            ], COL0:color});
        }
        //this.vtxBuf.bind();
        //gx.program.use();
        //gx._setShaderMtxs();
        //gl.drawArrays(cube.fill ? gl.TRIANGLES : gl.LINE_LOOP, 0, idxs.length);
        this.vtxBuf.bind();
        gl.drawArrays(cube.fill ? gl.TRIANGLES : gl.LINE_LOOP, first, idxs.length);
    }
}

export default class MapView {
    /** A page that attempts to render a map.
     */
    constructor(app) {
        this.app = app;
        this.element = E.div(E.canvas({id:'glCanvas'}));
    }

    getTitle() {
        const name = this.map ? this.map.internalName : this.romlistName;
        return `${name} - Map View`;
    }

    async run() {
        this.romlistName = this.app.params.get('romlist');
        if(this.romlistName == null) {
            const id = this.app.params.get('id');
            this.romlistName = this.app.game.maps[id].romlistName;
        }
        this.romlist = await this.app.game.getRomList(this.romlistName);

        this.map = this.app.game.maps[this.app.params.get('id')];
        await this.map.readMapsBin();

        this.ctx = new Context('#glCanvas');
        await this.ctx.loadPrograms();

        this.viewControl = new ViewControl(this.ctx);
        this.lightControl = new LightControl(this.ctx);
        this.element.append(
            E.div({id:'controls'},
                this.viewControl.element,
                this.lightControl.element,
                E.div({id:'glPicker'}),
            ),
        );

        this.renderer = new Renderer(this);
        this.ctx.setRenderer(this.renderer);

        this.viewControl.set({
            pos: {x:-2000, y:500, z:-1900},
            rot: {x:   75, y: 15, z:    0},
        });
        this.lightControl.set({
            ambColor: {r:255, g:255, b:255},
        });

        this.ctx.redraw();
        this.ctx.gx.printStats();
    }
}
