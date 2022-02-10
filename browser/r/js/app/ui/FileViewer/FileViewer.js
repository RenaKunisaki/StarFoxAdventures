import { E, clearElement } from "../../../lib/Element.js";
import HexViewer from "./HexViewer.js";

export default class FileViewer {
    constructor(app, file) {
        this.app  = app;
        this.file = file;
        this.view = this.file.getData();
        this.element = E.div('fileViewer');
        this.viewer = null;
        this.refresh();
    }

    refresh() {
        let elem;

        const name = this.file.name;
        /* if(name.endsWith('.romlist.zlb')) {
            elem = "XXXX"; // TODO
        }
        else */ this.viewer = new HexViewer(this.app, this.view);

        clearElement(this.element);
        this.element.append(
            E.h1(null, this.file.name),
            E.h2('path', `${this.file.path} (${this.file.getFormat()})`),
            this.viewer.element);
    }
}
