import { E, clearElement } from "../../../lib/Element.js";
import { hex } from "../../../Util.js";

const fmtSizes = {
    hex:   1, ascii:  1, hexscii: 1,
    s8:    1, u8:     1,
    s16:   2, u16:    2,
    s32:   4, u32:    4,
    float: 4, double: 8,
};
const fmtFuncs = {
    hex:   (view, offs) => hex(view.getUint8(offs), 2),
    ascii: (view, offs) => {
        let b = view.getUint8(offs);
        if(b < 0x20 || b > 0x7E) return '.';
        return String.fromCharCode(b);
    },
    hexscii: (view, offs) => {
        let b = view.getUint8(offs);
        if(b < 0x20 || b > 0x7E) return hex(b, 2);
        return ' '+String.fromCharCode(b);
    },
    s8:     (view, offs) => view.getInt8   (offs).toString().padStart(4),
    u8:     (view, offs) => view.getUint8  (offs).toString().padStart(3),
    s16:    (view, offs) => view.getInt16  (offs).toString().padStart(6),
    u16:    (view, offs) => view.getUint16 (offs).toString().padStart(5),
    s32:    (view, offs) => view.getInt32  (offs).toString().padStart(11),
    u32:    (view, offs) => view.getUint32 (offs).toString().padStart(10),
    float:  (view, offs) => view.getFloat32(offs).toFixed(5).padStart(10),
    double: (view, offs) => view.getFloat64(offs).toFixed(5).padStart(10),
};

export default class HexViewer {
    constructor(app, dataView) {
        this.app     = app;
        this.view    = dataView;
        this.offset  = 0;
        this.rows    = 64;
        this.eBody   = E.div('hexdump');
        this.element = E.div('hexview',
            this._makeToolbar(),
            this.eBody);
        this.refresh();
    }

    _makeToolbar() {
        this.eFormats = E.select(
            E.option(null, "hex", {value:'hex', selected:true}),
            E.option(null, "ASCII", {value:'ascii'}),
            E.option(null, "HexSCII", {value:'hexscii'}),
            E.option(null, "s8", {value:'s8'}),
            E.option(null, "u8", {value:'u8'}),
            E.option(null, "s16", {value:'s16'}),
            E.option(null, "u16", {value:'u16'}),
            E.option(null, "s32", {value:'s32'}),
            E.option(null, "u32", {value:'u32'}),
            E.option(null, "float", {value:'float'}),
            E.option(null, "double", {value:'double'}),
        );
        this.eFormats.addEventListener('change', e => this.refresh());

        this.eCols = E.input({type:'number', value:16, min:1});
        this.eCols.addEventListener('change', e => this.refresh());

        return E.div('tools',
            "Format:", this.eFormats,
            "Bytes/line:", this.eCols,
        );
    }

    refresh() {
        //not using table here because it would be a ton more elements
        const elem  = E.div('hexdump');
        let   offs;
        let   lines = [];
        const fmt   = this.eFormats.value;
        const fSize = fmtSizes[fmt];
        const fFunc = fmtFuncs[fmt];
        let   nCols = parseInt(this.eCols.value);
        if(isNaN(nCols) || nCols < 1) nCols = 16;
        for(offs=0; offs<this.view.byteLength; offs += nCols) {
            //add offset
            let eLine = E.div('line');
            eLine.append(E.span('offset', hex(offs, 6) + '  '));

            let data = '';
            //try {
                for(let i=0; i<nCols/fSize; i++) {
                    let it = fFunc(this.view, offs+(i*fSize));
                    data += it + (((i & 3) == 3) ? '  ' : ' ');
                }
            //}
            //catch(RangeError) {}
            eLine.append(E.span('data', data));

            lines.push(eLine);
            if(lines.length >= this.rows) break;
        }
        elem.append(...lines);
        if(offs+1 < this.view.byteLength) {
            const nMoreLines = Math.ceil((this.view.byteLength - offs) / nCols);
            const eMore = E.div('more',
                `${nMoreLines} more line${nMoreLines == 1 ? '' : 's'}...`);
            elem.append(eMore);
            eMore.addEventListener('click', e => {
                this.rows = Math.ceil(this.view.byteLength / nCols)+1;
                this.refresh();
            })
        }

        clearElement(this.eBody);
        this.eBody.append(elem);
    }
}
