import { E } from "./lib/Element.js";

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
    /** Download a binary file. */
    return (await get({
        path:         path,
        mimeType:     'application/octet-stream',
        responseType: 'arraybuffer',
    })).response;
}
export async function getXml(path) {
    /** Download an XML file. */
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
    if(n < 0) n = (((1 << (size*4)) - 1) ^ -n) + 1;
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
export const Percent = val => (val * 100).toFixed(0).padStart(3) + '%';
export function fileSize(val) {
    /** Convert `val` to human-readable number of bytes. */
    const units = [' ', 'K', 'M', 'G', 'T'];
    let unit = 0;
    while(unit < units.length && val > 9999) {
        val = Math.floor(val / 1024);
        unit++;
    }
    return `${val.toString().padStart(4)}${units[unit]}`;
}

export function hsv2rgb(h, s, v) {
    /** Convert Hue, Saturation, Value to RGB.
     *  @param h hue, in degrees (0..360)
     *  @param s saturation (0..1)
     *  @param v value (0..1)
     *  @return [r, g, b] (0..1)
     *  @note copied from https://en.wikipedia.org/wiki/HSL_and_HSV
     */
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

export function Table(...rows) {
    /** Create the tr and td elements of a table from some arrays. */
    let elems = [];
    for(let row of rows) {
        let tr = E.tr(null);
        for(let cell of row) tr.append(E.td(null, cell));
        elems.push(tr);
    }
    return elems;
}

export function CollapseList(...items) {
    /** Given some strings or HTML elements, return a list.
     *  If there is more than one item, the list is a
     *  collapsible element (<summary> and <details> elements).
     *  If one item, the list is a <span>. If zero,
     *  the list is an empty string.
     */
    if(items.length == 0) return '';
    if(items.length == 1) return E.span('list', items[0]);
    const eList = E.ul();
    const elem = E.details('collapseList', E.summary(null,
        E.span('count', items.length), items[0]), eList);
    for(let i=1; i<items.length; i++) {
        eList.append(E.li(null, items[i]));
    }
    return elem;
}

export function addReverseMap(obj) {
    /** Given a dict with keys eg a:1, b:2... add keys 1:a, 2:b... */
    const obj2 = {}; //don't change while iterating
    for(let [k, v] of Object.entries(obj)) obj2[v] = k;
    for(let [v, k] of Object.entries(obj2)) obj[v] = k;
    return obj;
}
