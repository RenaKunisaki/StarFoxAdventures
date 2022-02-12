import { E, clearElement } from "../../../lib/Element.js";
import ErrorMessage from "../ErrorMessage.js";
import HexViewer from "./HexViewer.js";
import { RomListViewer } from "./RomListViewer.js";

export default class FileViewer {
    constructor(app, file, showTitle=true) {
        this.app  = app;
        this.file = file;
        this.element = E.div('fileViewer');
        this.viewer = null;
        this.showTitle = showTitle;
        this.error = null;
        try {
            this.view = this.file.getData();
        }
        catch(ex) {
            this.view = null;
            this.error = ex;
        }
        this.refresh();
    }

    refresh() {
        let elem;

        const name = this.file.name;
        if(this.error) {
            this.viewer = new ErrorMessage(this.app, this.error);
        }
        else if(name.endsWith('.romlist.zlb')) {
            this.viewer = new RomListViewer(this.app, this.view);
        }
        else this.viewer = new HexViewer(this.app, this.view);

        clearElement(this.element);
        if(this.showTitle) {
            this.element.append(
                E.h1(null, this.file.name),
                E.h2('path', `${this.file.path} (${this.file.getFormat()})`),
            );
        }
        this.element.append(this.viewer.element);
    }
}
