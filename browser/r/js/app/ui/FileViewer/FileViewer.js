import { E, clearElement } from "../../../lib/Element.js";
import { hex } from "../../../Util.js";

export default class FileViewer {
    constructor(app, file) {
        this.app  = app;
        this.file = file;
        this.view = this.file.getData();
        this.element = E.div('fileViewer');
        this.refresh();
    }

    refresh() {
        let elem;

        const name = this.file.name;
        /* if(name.endsWith('.romlist.zlb')) {
            elem = "XXXX"; // TODO
        }
        else */ elem = this._makeHexDump(); //default behaviour

        clearElement(this.element);
        this.element.append(
            E.h1(null, this.file.name),
            E.h2('path', `${this.file.path} (${this.file.getFormat()})`),
            elem);
    }

    _makeHexDump() {
        //not using table here because it would be a ton more elements
        const elem = E.div('hexdump');
        for(let offs=0; offs<this.view.byteLength; offs += 16) {
            //add offset
            let eLine = E.div('line');
            eLine.append(E.span('offset', hex(offs, 6) + ' '));

            //add hex
            let data = '';
            for(let i=0; i<16; i++) {
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
            for(let i=0; i<16; i++) {
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
            elem.append(eLine);
        }
        return elem;
    }
}
