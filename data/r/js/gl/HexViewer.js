import {E, clearElement} from '/r/js/Element.js';

export default class HexViewer {
    constructor() {
        this.data    = null;
        this.nCols   = 16;
        this.nRows   = 16;
        this.offset  = 0;
        this.element = E.div('hexview');
    }

    show() { this.element.style.display = ''; return this }
    hide() { this.element.style.display = 'block'; return this }

    setData(data, offset=null) {
        this.data = new DataView(data);
        if(offset != null) this.offset = offset;
        this.refresh();
        return this;
    }

    setOffset(offs) {
        this.offset = offs;
        this.refresh();
        return this;
    }

    refresh() {
        this.table = E.table();
        let row = E.tr(null, E.th(null, "Offset"));
        for(let i=0; i<this.nCols; i++) {
            row.append(E.th('hex', i.toString(16).toUpperCase().padStart(2,'0')));
            if((i & 3) == 3 && (i+1) < this.nCols) {
                row.append(E.th(null, ' '));
            }
        }
        this.table.append(row);
        let offs = this.offset;
        for(let i=0; i<this.nRows; i++) {
            const offStr = offs.toString(16).toUpperCase().padStart(6, '0');
            let row = E.tr(null, E.th(null, offStr));
            for(let j=0; j<this.nCols; j++) {
                const b = this.data.getUint8(offs).toString(16).toUpperCase().padStart(2,'0');
                row.append(E.td('hex byte'+b, b));
                if((j & 3) == 3 && (j+1) < this.nCols) {
                    row.append(E.td(null, ' '));
                }
                offs++;
            }
            this.table.append(row);
        }
        clearElement(this.element).append(this.table);
        return this;
    }
}
