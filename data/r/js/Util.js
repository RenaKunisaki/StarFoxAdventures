import {E} from './Element.js';

export function get(params) {
    /** Fetch some remote resource.
     *  @param path URI to fetch.
     *  @param mimeType MIME type override to use.
     *  @param respType 'text' or 'arraybuffer' to override responseType.
     *  @return a Promise which resolves to the XHR object once the request
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

export async function getBin(path) {
    return (await get({
        path:         path,
        mimeType:     'application/octet-stream',
        responseType: 'arraybuffer',
    })).response;
}
export async function getXml(path) {
    return (await get({
        path:     path,
        mimeType: 'text/xml; charset=utf-8',
    })).responseXML;
}

export function hex(n, size=1) {
    /** Convert number `n` to hex, padded to given `size`.
     *  Result is uppercase without prefix, eg "0000BABE".
     */
    if(n == null || n == undefined) return String(n);
    return n.toString(16).toUpperCase().padStart(size, '0');
}

export function int(n, dflt=null) {
    /** Convert string `n` to int, returning `dflt` for null/undefined.
     */
    if(n == null || n == undefined) return dflt;
    return parseInt(n);
}
export function float(n, dflt=null) {
    /** Convert string `n` to float, returning `dflt` for null/undefined.
     */
    if(n == null || n == undefined) return dflt;
    return parseFloat(n);
}

export function round(n, dec=2) {
    //return Math.round(n * (10**dec)) / (10**dec);
    return n.toFixed(dec);
}

export function hsv2rgb(h, s, v) {
    //from https://en.wikipedia.org/wiki/HSL_and_HSV
    const c = v * s; //chroma
    h = h / 60; //convert degrees
    const x = c * (1 - Math.abs(h % 2 - 1));
    let r=0, g=0, b=0;
    if     (0 <= h && h <= 1) { r=c; g=x; b=0; }
    else if(1 <= h && h <= 2) { r=x; g=c; b=0; }
    else if(2 <= h && h <= 3) { r=0; g=c; b=x; }
    else if(3 <= h && h <= 4) { r=0; g=x; b=c; }
    else if(3 <= h && h <= 4) { r=0; g=x; b=c; }
    else if(4 <= h && h <= 5) { r=x; g=0; b=c; }
    else if(5 <= h && h <= 6) { r=c; g=0; b=x; }
    const m = v - c
    return [r+m, g+m, b+m];
}


export function getDescriptionAndNotes(self, elem) {
    /** Given an object and an XML element, set the object's
     *  `description` and `notes` fields.
     */
    let desc   = [];
    self.notes = [];
    for(let child of elem.childNodes) {
        switch(child.tagName) {
            case 'description': desc.push(child.textContent); break;
            case 'note': self.notes.push(child.textContent); break;
        }
    }
    self.description = desc.join('\n');
    return self;
}

export function makeDescriptionAndNotes(obj) {
    /** Given an object that has `description` and optionally `notes` fields,
     *  construct an HTML list element containing those fields.
     *  If only `description` is present, returns it as a string.
     *  Otherwise, returns an HTML list of the description plus the notes.
     *  Expects `description` to be a string, `notes` to be an array.
     */
    if((!obj.notes) || obj.notes.length == 0) return obj.description;
    const ul = E.ul(null, E.li(null, obj.description));
    for(let note of obj.notes) {
        ul.append(E.li('note', note));
    }
    return ul;
}

export function makeList(items, force=false) {
    /** Make an HTML list from an array.
     *  If the array has zero elements, and `force` is false, return the empty string.
     *  If the array has one element, and `force` is false, return the element.
     *  Otherwise, return an HTML list element.
     */
    if(!force) {
        if(items.length == 0) return '';
        if(items.length == 1) return String(items[0]);
    }
    const ul = E.ul();
    for(let item of items) ul.append(E.li(null, String(item)));
    return ul;
}

export function makeCollapsibleList(items) {
    /** Given a list of items, construct an HTML element.
     *  If the list is empty, the element contains just the text "-".
     *  If the list has one item, the element contains that item.
     *  Otherwise, the element contains a count, the first item, and
     *  an expander that can be clicked to display the other items.
     */
    if(items.length == 0) return "-";
    if(items.length == 1) return items[0];
    const ul = E.ul();
    for(let i=1; i<items.length; i++) {
        ul.append(E.li(null, items[i]));
    }
    return E.details(E.summary('count',
        `[${items.length}] ${items[0]}`
    ), ul);
}

export async function getAssets(version) {
    const path = `${version}/assets.xml`;
    return (await get({path:path, mimeType:'text/xml; charset=utf-8'})).responseXML;
}
export async function getBits(version) {
    const path = `${version}/gamebits.xml`;
    return (await get({path:path, mimeType:'text/xml; charset=utf-8'})).responseXML;
}
export async function getDlls(version) {
    const path = `${version}/dlls.xml`;
    return (await get({path:path, mimeType:'text/xml; charset=utf-8'})).responseXML;
}
export async function getGameTexts(version) {
    const path = `${version}/gametext.xml`;
    return (await get({path:path, mimeType:'text/xml; charset=utf-8'})).responseXML;
}
export async function getMaps(version) {
    const path = `${version}/maps.xml`;
    return (await get({path:path, mimeType:'text/xml; charset=utf-8'})).responseXML;
}
export async function getMapDirs(version) {
    const path = `${version}/mapdirs.xml`;
    return (await get({path:path, mimeType:'text/xml; charset=utf-8'})).responseXML;
}
export async function getModels(version) {
    const path = `${version}/models.xml`;
    return (await get({path:path, mimeType:'text/xml; charset=utf-8'})).responseXML;
}
export async function getObjects(version) {
    const path = `${version}/objects.xml`;
    return (await get({path:path, mimeType:'text/xml; charset=utf-8'})).responseXML;
}

function makePageHeader() {
    const versions = ['U0', 'U1', 'K'];
    const tbl = E.table();
    for(const ver of versions) {
        tbl.append(E.tr(
            E.th(null, ver),
            E.td(null, E.a(null, {href:`objects.html?ver=${ver}`},  "Objects")),
            E.td(null, E.a(null, {href:`dlls.html?ver=${ver}`},     "DLLs")),
            E.td(null, E.a(null, {href:`gamebits.html?ver=${ver}`}, "Bits")),
            E.td(null, E.a(null, {href:`gametext.html?ver=${ver}`}, "Text")),
            E.td(null, E.a(null, {href:`maplist.html?ver=${ver}`},  "Maps")),
        ));
    }
    tbl.append(E.tr(
        E.th(null, 'etc'),
        E.td(null, E.a(null, {href:"dllcompare.html"}, "DLL Compare")),
    ));
    return E.nav(null, {id:'mainNav'}, tbl);
}

export function finishLoading() {
    /** Remove loading indicator from page, and scroll to target element.
     *  We do this scrolling because the element may not have existed when the
     *  page first loaded, and some browsers aren't clever enough to jump to it
     *  when it first appears.
     */
    const loading = document.getElementById('loading');
    if(loading) loading.replaceWith(makePageHeader());
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
    if(!v) { debugger; throw new Error("Vector is "+String(v)); }
    for(const n of v) validNumber(n);
    return v;
}

export function isPowerOf2(value) {
    return (value & (value - 1)) == 0;
}

export function hexStringToData(str) {
    const r = str.match(/[0-9a-fA-F]{1,2}/g);
    if(r == null) return '';
    return new Uint8Array(r.map(byte => parseInt(byte, 16)));
}

export function toHexString(data, offset=0, length=null) {
    /** Convert data to raw hex string.
     */
    const view = new Uint8Array(data);
    if(length == null) length = view.byteLength;
    const res = [];
    let c=0;
    for(let offs=offset; offs<length; offs++) {
        const b = view[offs].toString(16).padStart(2, '0').toUpperCase();
        res.push(b);
        c = (c+1) & 3;
        if(!c) res.push(' ');
    }
    return res.join('');
}

export function hexdump(data, offset=0, length=null, cols=16) {
    /** Create hex dump of data.
     *  @param data ArrayBuffer to dump.
     *  @param offset First byte to dump.
     *  @param length Number of bytes to dump. Default: to end of data.
     *  @param cols Number of columns.
     *  @return an array of lines.
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
