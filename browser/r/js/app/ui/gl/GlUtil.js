export function validNumber(...args) {
    /** Check if argument(s) is a valid number, and throw an error if not.
     *  Accepts one or more arguments.
     *  If value is a string, tries to convert it to a float.
     *  If value is undefined, null, infinite, NaN, or a non-numeric type
     *  (after converting strings), throws an error.
     *  Otherwise, returns the number. (If given more than one argument, returns
     *  the numbers as an array.)
     */
    for(const n of args) {
        //lol break on exception doesn't work
        if(n == undefined) {debugger; throw new Error("Value is undefined"); }
        if(n == null) {debugger; throw new Error("Value is null"); }

        //isNaN returns true for non-numeric types.
        if(typeof n == 'string') n = parseFloat(n);
        if(isNaN(n)) {debugger; throw new Error("Value is NaN"); }
        if(!isFinite(n)) {debugger; throw new Error("Value is infinite"); }
    }
    if(args.length == 1) return args[0];
    return args;
}

export function validMatrix(mtx) {
    /** Check if argument is a valid matrix, and throw an error if not.
     *  Throws an error if:
     *  - argument is not an array
     *  - argument length is not between 1 and 16
     *  - any element is infinite, NaN, or non-numeric type (including strings)
     *  - all elements are zero
     *  Otherwise, returns the argument.
     */
    if(mtx.length == undefined) throw new Error("Matrix is not an array");
    if(mtx.length < 1 || mtx.length > 16) {
        throw new Error("Matrix length is "+String(mtx.length));
    }
    let v = 0;
    for(let i=0; i<mtx.length; i++) v |= validNumber(mtx[i]);
    if(!v) {debugger; throw new Error("Matrix is zero"); }
    return mtx;
}

export function validVector(v) {
    /** Check if argument is a valid vector (array of numbers),
     *  and throw an error if not.
     *  Throws an error if:
     *  - argument is not an array
     *  - argument is of length 0
     *  - any element is infinite, NaN, or non-numeric type (including strings)
     *  Otherwise, returns the argument.
     *  This differs from validMatrix() in that it does not throw an error
     *  if all elements are zero or if there are more than 16 elements.
     */
    if(!v) { debugger; throw new Error("Vector is "+String(v)); }
    for(const n of v) validNumber(n);
    return v;
}

export function makeCube(gl, x, y, z, scale, id, colors=null) {
    /** Generate a cube.
     *  @param {WebGL2RenderingContext} gl WebGL instance to use.
     *  @param {number} x X position of centre.
     *  @param {number} y Y position of centre.
     *  @param {number} z Z position of centre.
     *  @param {number} scale Size of cube.
     *  @param {integer} id Vertex ID for picker.
     *  @param {Array} colors Array of vertex colors.
     *  @returns {Array} vertex data.
     */
    const vtxPositions = [ //x, y, z
        [x-scale, y-scale, z-scale], //[0] top left back
        [x+scale, y-scale, z-scale], //[1] top right back
        [x+scale, y+scale, z-scale], //[2] top right front
        [x-scale, y+scale, z-scale], //[3] top left front
        [x-scale, y-scale, z+scale], //[4] bot left back
        [x+scale, y-scale, z+scale], //[5] bot right back
        [x+scale, y+scale, z+scale], //[6] bot right front
        [x-scale, y+scale, z+scale], //[7] bot left front
    ];
    if(!colors) colors = [ //r, g, b, a
        [0x00, 0x00, 0x00, 0xCF], //[0] top left back
        [0xFF, 0x00, 0x00, 0xCF], //[1] top right back
        [0xFF, 0xFF, 0x00, 0xCF], //[2] top right front
        [0x00, 0xFF, 0x00, 0xCF], //[3] top left front
        [0x00, 0x00, 0xFF, 0xCF], //[4] bot left back
        [0xFF, 0x00, 0xFF, 0xCF], //[5] bot right back
        [0xFF, 0xFF, 0xFF, 0xCF], //[6] bot right front
        [0x00, 0xFF, 0xFF, 0xCF], //[7] bot left front
    ];
    //if only one color is given, use it properly.
    if(!Array.isArray(colors[0])) colors = [colors];
    const vtxIdxs = [
        //top          //front        //left
        0,1,3, 1,2,3,  3,2,6, 3,6,7,  0,3,4, 3,7,4,
        //right        //bottom       //back
        2,1,5, 2,5,6,  7,5,4, 7,6,5,  1,0,5, 0,4,5,
    ];
    const vtxs = [gl.TRIANGLES];
    for(const idx of vtxIdxs) {
        vtxs.push({
            POS:  vtxPositions[idx],
            COL0: colors[idx % colors.length],
            COL1: colors[idx % colors.length],
            id:   id,
        });
    }
    return vtxs;
}

export function makeBox(gl, p1, p2, id, colors=null) {
    /** Generate a box between two points.
     *  @param {WebGL2RenderingContext} gl WebGL instance to use.
     *  @param {Array} p1 Position [x,y,z] of first corner.
     *  @param {Array} p2 Position [x,y,z] of opposite corner.
     *  @param {integer} id Vertex ID for picker.
     *  @param {Array} colors Array of vertex colors.
     *  @returns {Array} vertex data.
     */
    const [x1, y1, z1] = p1;
    const [x2, y2, z2] = p2;
    const vtxPositions = [ //x, y, z
        //back left, back right, front right, front left
        [x1, y1, z1], [x2, y1, z1], [x2, y2, z1], [x1, y2, z1], //top
        [x1, y1, z2], [x2, y1, z2], [x2, y2, z2], [x1, y2, z2], //bot
    ];
    if(!colors) colors = [ //r, g, b, a
        [0x00, 0x00, 0x00, 0xCF], //[0] top left back
        [0xFF, 0x00, 0x00, 0xCF], //[1] top right back
        [0xFF, 0xFF, 0x00, 0xCF], //[2] top right front
        [0x00, 0xFF, 0x00, 0xCF], //[3] top left front
        [0x00, 0x00, 0xFF, 0xCF], //[4] bot left back
        [0xFF, 0x00, 0xFF, 0xCF], //[5] bot right back
        [0xFF, 0xFF, 0xFF, 0xCF], //[6] bot right front
        [0x00, 0xFF, 0xFF, 0xCF], //[7] bot left front
    ];
    //if only one color is given, use it properly.
    if(!Array.isArray(colors[0])) colors = [colors];
    const vtxIdxs = [
        //top          //front        //left
        0,1,3, 1,2,3,  3,2,6, 3,6,7,  0,3,4, 3,7,4,
        //right        //bottom       //back
        2,1,5, 2,5,6,  7,5,4, 7,6,5,  1,0,5, 0,4,5,
    ];
    const vtxs = [gl.TRIANGLES];
    for(const idx of vtxIdxs) {
        vtxs.push({
            POS:  vtxPositions[idx],
            COL0: colors[idx % colors.length],
            COL1: colors[idx % colors.length],
            id:   id,
        });
    }
    return vtxs;
}
