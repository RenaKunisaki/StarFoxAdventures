import { E, clearElement } from "../../../lib/Element.js";
import ErrorMessage from "../ErrorMessage.js";
import HexViewer from "./HexViewer.js";
import { RomListViewer } from "./RomListViewer.js";
import { GameTextViewer } from "./GameTextViewer.js";

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
        this._makeFormatSelect();
        this.refresh();
    }

    _makeFormatSelect() {
        this.eFormatSel = E.select('formatList', {id:'formatSelect'},
            E.option(null, "Auto",     {value:'auto'}),
            E.option(null, "Hex",      {value:'hex'}),
            E.option(null, "RomList",  {value:'romlist'}),
            E.option(null, "GameText", {value:'gametext'}),
        );
        this.eFormatSel.addEventListener('change', e => this.refresh());

        this.eToolbar = E.div('toolbar',
            E.label(null, "View as:", {For:'formatSelect'}),
            this.eFormatSel,
        );
    }

    refresh() {
        let elem;
        let fmt = this.eFormatSel.value;

        const name = this.file.name;
        try {
            if(this.error) {
                this.viewer = new ErrorMessage(this.app, this.error);
            }
            else if((fmt == 'auto' && name.endsWith('.romlist.zlb'))
            || fmt == 'romlist') {
                this.viewer = new RomListViewer(this.app, this.view);
            }
            else if((fmt == 'auto' && this.file.path.startsWith('/gametext'))
            || fmt == 'gametext') {
                this.viewer = new GameTextViewer(this.app, this.view);
            }
            else this.viewer = new HexViewer(this.app, this.view);
        }
        catch(ex) {
            //XXX instead of showing "not a GameText file" when set to Auto,
            //we should just switch to hex view...
            console.error(ex);
            this.viewer = new ErrorMessage(this.app, ex.toString());
        }

        clearElement(this.element);
        if(this.showTitle) {
            this.element.append(
                E.h1(null, this.file.name),
                E.h2('path', `${this.file.path} (${this.file.getFormat()})`),
            );
        }
        this.element.append(this.eToolbar, this.viewer.element);
    }
}
