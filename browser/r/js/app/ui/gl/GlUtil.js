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
