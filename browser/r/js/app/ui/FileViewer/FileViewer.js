import { E, clearElement } from "../../../lib/Element.js";
import { hex } from "../../../Util.js";

export default class FileViewer {
    constructor(app, file) {
        this.app  = app;
        this.file = file;
        this.view = new DataView(this.file.buffer,
            this.file.bufferOffs, this.file.size);
        this.element = E.div('fileViewer');
        this.refresh();
    }

    refresh() {
        const elem = E.div('hex');
        for(let offs=0; offs<this.file.size; offs += 16) {
            let eLine = E.div('line');
            eLine.append(E.span('offset', hex(offs, 6)));

            let data = '';
            for(let i=0; i<16; i++) {
                let offsI = offs+i;
                let b = '..';
                if(offsI < this.file.size) {
                    b = hex(this.view.getUint8(offsI), 2);
                }
                data += b + (((i & 3) == 3) ? '  ' : ' ');
            }
            eLine.append(E.span('data', data));
            elem.append(eLine);
        }
        clearElement(this.element);
        this.element.append(elem);
    }
}
