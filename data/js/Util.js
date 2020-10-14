export function get(params) {
    /** Fetch some remote resource.
     *  path: URI to fetch.
     *  type: MIME type override to use.
     *  respType: 'text' or 'arraybuffer' to override responseType.
     *  Returns a Promise which resolves to the XHR object once the request
     *  is completed, which you can use to retrieve the response in the
     *  desired format.
     */
    if(typeof params == 'string') params = {path:params};

    let firefux = '_='+performance.now();
    if(params.path.indexOf('?') >= 0) firefux = '&'+firefux;
    else firefux='?'+firefux;

    return new Promise(function (resolve, reject) {
        const xhr = new XMLHttpRequest();
        if(params.mimeType != undefined) xhr.overrideMimeType(params.mimeType);
        if(params.responseType != undefined) xhr.responseType = params.responseType;
        xhr.open('GET', params.path+firefux, true);
        xhr.onreadystatechange = () => {
            if(xhr.readyState == 4) resolve(xhr);
        };
        xhr.onerror = () => {
            reject({
                status:     xhr.status,
                statusText: xhr.statusText
            });
        };
        xhr.send(null);
    });
}

export async function getGameTexts() {
    const xhr = await get('gametext.json', 'application/json');
    return JSON.parse(xhr.responseText);
}
export async function getObjects() {
    const xhr = await get('objects.xml', 'text/xml; charset=utf-8');
    return xhr.responseXML;
}
export async function getMaps() {
    const xhr = await get('maps.xml', 'text/xml; charset=utf-8');
    return xhr.responseXML;
}
export async function getBits() {
    const xhr = await get('gamebits.xml', 'text/xml; charset=utf-8');
    return xhr.responseXML;
}
export async function getDLLs() {
    const xhr = await get('dlls.xml', 'text/xml; charset=utf-8');
    return xhr.responseXML;
}

export function finishLoading() {
    /** Remove loading indicator from page, and scroll to target element.
     *  We do this scrolling because the element may not have existed when the
     *  page first loaded, and some browsers aren't clever enough to jump to it
     *  when it first appears.
     */
    const loading = document.getElementById('loading');
    if(loading) loading.remove();
    const id = window.location.hash;
    if(id == null || id.length < 1) return;
    const elem = document.getElementById(id.substr(1));
    if(elem) {
        elem.scrollIntoView();
        window.scrollBy(0, -24); //don't hide under sticky header
    }
}

export function setupMain(main) {
    if(document.readyState === "complete"
    || document.readyState === "interactive") {
        setTimeout(main, 1); //call on next available tick
    } else {
        document.addEventListener("DOMContentLoaded", main);
    }

}

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
    for(const n of v) validNumber(n);
    return v;
}

export function isPowerOf2(value) {
    return (value & (value - 1)) == 0;
}

export function hexdump(data, offset=0, length=null, cols=16) {
    /** Create hex dump of data.
     *  data: ArrayBuffer to dump.
     *  offset: First byte to dump.
     *  length: Number of bytes to dump. Default: to end of data.
     *  cols: Number of columns.
     *  Returns an array of lines.
     */
    const view = new Uint8Array(data);
    const res  = [];
    if(length == null) length = view.byteLength;
    for(let offs=offset; offs<length; offs += cols) {
        const line = [offs.toString(16).padStart(4, '0').toUpperCase()+' '];
        for(let col=0; col<cols && col+offs < length; col++) {
            const b = view[offs+col].toString(16).padStart(2, '0').toUpperCase();
            line.push(b+' ');
            if((col & 3) == 3) line.push(' ');
        }
        res.push(line.join(''));
    }
    return res; //.join('\n');
}
