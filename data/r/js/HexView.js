import {E, clearElement} from './Element.js';
import {hex} from './Util.js';

export default class HexView {
    /** A widget for displaying raw data in hex.
     */
    constructor(app) {
        this.app     = app;
        this.element = E.div('hex-view-main');
        this.nCols   = 16;
        this._offset = 0;
        this._data   = null;
    }

    get data() { return this._data; }
    set data(d) {
        this._data = d;
        this.refresh();
    }

    get offset() { return this._offset; }
    set offset(o) {
        this._offset = o;
        this.refresh();
    }

    refresh() {
        clearElement(this.element);
        if(this._data == null) {
            this.element.append("No data");
            return;
        }

        let offset = this._offset;
        while(offset < this._data.buffer.byteLength) {
            let line = [], text = [];
            for(let iCol=0; iCol < this.nCols; iCol++) {
                let cls = 'byte';
                if(offset + iCol >= this._data.buffer.byteLength) {
                    line.push(E.span(cls+' null', '..'));
                    text.push(E.span(cls+' null', '.'));
                }
                else {
                    let b = this._data.getUint8(offset + iCol);
                    if(b == 0 || b == 0xFF) cls += ' null';
                    line.push(E.span(cls, hex(b, 2)));

                    cls = 'char';
                    let c = '.';
                    if(b >= 0x20 && b <= 0x7E) c = String.fromCharCode(b);
                    else cls += ' null';
                    text.push(E.span(cls, c));
                }
            }

            this.element.append(E.div('line',
                E.span('address', hex(offset, 6)),
                E.span('data', line),
                E.span('text', text),
            ));
            offset += this.nCols;
        }
    }
}
