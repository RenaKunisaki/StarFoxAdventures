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
export const Percent = val => (val * 100).toFixed(0).padStart(3) + '%';
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
export function Table(...rows) {
    let elems = [];
    for(let row of rows) {
        let tr = E.tr(null);
        for(let cell of row) tr.append(E.td(null, cell));
        elems.push(tr);
    }
    return elems;
}
