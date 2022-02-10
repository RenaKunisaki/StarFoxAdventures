import { E, clearElement } from "../../../lib/Element.js";
import { hex } from "../../../Util.js";

export default class HexViewer {
    constructor(app, dataView) {
        this.app     = app;
        this.view    = dataView;
        this.offset  = 0;
        this.columns = 16;
        this.rows    = 64;
        this.element = E.div('hexdump');
        this.refresh();
    }

    refresh() {
        //not using table here because it would be a ton more elements
        const elem = E.div('hexdump');
        let lines = [];
        let offs;
        for(offs=0; offs<this.view.byteLength; offs += this.columns) {
            //add offset
            let eLine = E.div('line');
            eLine.append(E.span('offset', hex(offs, 6) + ' '));

            //add hex
            let data = '';
            for(let i=0; i<this.columns; i++) {
                let offsI = offs+i;
                let b = '..';
                if(offsI < this.view.byteLength) {
                    b = hex(this.view.getUint8(offsI), 2);
                }
                data += b + (((i & 3) == 3) ? '  ' : ' ');
            }
            eLine.append(E.span('data', data));

            //add ASCII
            let text = '';
            for(let i=0; i<this.columns; i++) {
                let offsI = offs+i;
                let b = '.';
                if(offsI < this.view.byteLength) {
                    b = this.view.getUint8(offsI);
                    if(b >= 0x20 && b <= 0x7E) b = String.fromCharCode(b);
                    else b = '.';
                }
                text += b + (((i & 3) == 3) ? ' ' : '');
            }
            eLine.append(E.span('text', text));
            lines.push(eLine);
            //subtract 1 here because it's dumb to have "1 more line"
            //instead of just having the line
            if(lines.length >= this.rows-1) break;
        }
        elem.append(...lines);
        if(offs < this.view.byteLength) {
            const nMoreLines = Math.ceil((this.view.byteLength - offs) / this.columns);
            elem.append(E.div('more',
                `${nMoreLines} more line${nMoreLines == 1 ? '' : 's'}...`));
            //XXX show them when clicked
        }

        clearElement(this.element);
        this.element.append(elem);
    }
}
