import { E, clearElement } from "../../../lib/Element.js";
import BinaryFile from "../../../lib/BinaryFile.js";

export default class TextViewer {
    constructor(app, dataView) {
        this.app     = app;
        this.file    = new BinaryFile(dataView.buffer,
            dataView.byteOffset, dataView.byteLength);
        this.offset  = 0;
        this.rows    = 64;
        this.eBody   = E.div();
        this.element = E.div('textviewer', this.eBody);
        this.refresh();
    }

    refresh() {
        const elem = E.div('textdump');
        let str = this.file.readStr(1000000);
        elem.append(E.span(null, str));
        clearElement(this.eBody);
        this.eBody.append(elem);
    }
}
