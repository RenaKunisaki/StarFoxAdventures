export default class BaseRenderer {
    /** Base class that renderers should inherit.
     */
    constructor(ctx) {
        this.ctx = ctx;
    }

    reset() {
        /** Reset all state to default.
         */
        this._didRender = false;
        this.ctx.gx.reset();
    }

    render() {
        //override this method with your render logic
    }

    renderPickBuffer() {
        //override this method with your render logic
    }

    drawCube(cube) {
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
        let first  = this.vtxBuf.vtxCount;
        const gl   = this.ctx.gl;
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
        this.vtxBuf.bind();
        gl.drawArrays(cube.fill ? gl.TRIANGLES : gl.LINE_LOOP, first, idxs.length);
    }
}
