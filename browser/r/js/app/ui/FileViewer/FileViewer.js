import { E, clearElement } from "../../../lib/Element.js";
import ErrorMessage from "../ErrorMessage.js";
import GameFile from "../../../game/GameFile.js";
import ArchiveViewer from "./ArchiveViewer.js";
import HexViewer from "./HexViewer.js";
import TextViewer from "./TextViewer.js";
import { RomListViewer } from "./RomListViewer.js";
import { GameTextViewer } from "./GameTextViewer.js";

export default class FileViewer {
    constructor(app, file, showTitle=true) {
        this.app        = app;
        this.file       = file;
        this.element    = E.div('fileViewer');
        this.viewer     = null;
        this.showTitle  = showTitle;
        this.error      = null;
        this.archiveIdx = null; //item idx of archive we're viewing (null=none)
        this.gameFile   = new GameFile(this.file);
        try {
            this.view = this.file.getData();
        }
        catch(ex) {
            this.view = null;
            this.error = ex;
            console.error(ex);
        }
        this._makeFormatSelect();
        this.refresh();
    }

    _makeFormatSelect() {
        this.eFormatSel = E.select('formatList', {id:'formatSelect'},
            E.option(null, "Auto",       {value:'auto'}),
            E.option(null, "Archive",    {value:'archive'}),
            E.option(null, "Hex",        {value:'hex'}),
            E.option(null, "Plain Text", {value:'text'}),
            E.option(null, "RomList",    {value:'romlist'}),
            E.option(null, "GameText",   {value:'gametext'}),
        );
        this.eFormatSel.addEventListener('change', e => this.refresh());

        this.eToolbar = E.div('toolbar',
            E.label(null, "View as:", {For:'formatSelect'}),
            this.eFormatSel,
        );
    }

    _makeViewer() {
        const fmt      = this.eFormatSel.value;
        const name     = this.file.name;
        const contents = this.gameFile.getContents();
        try {
            if(this.error) {
                this.viewer = new ErrorMessage(this.app, this.error.toString());
            }
            else if(this.view.byteLength == 0) {
                this.viewer = new ErrorMessage(this.app, "File is empty");
            }
            //if we're viewing an item in the archive, don't show the
            //archive's contents by default.
            //we can still switch the view mode to Archive to see them,
            //which is how we go back to the list from an item.
            else if((fmt == 'auto' && this.archiveIdx == null
            && contents.length > 1) || fmt == 'archive') {
                this.viewer = new ArchiveViewer(this.app, this.gameFile);
                this.viewer.cbView = (item, data) => {
                    //View button clicked. replace view with the item's data.
                    this.view = new DataView(data);
                    this.archiveIdx = item.idx;
                    this.eFormatSel.value = 'auto';
                    this.refresh();
                };
                this.archiveIdx = null; //we're not viewing an item
            }
            else if((fmt == 'auto' && name.endsWith('.romlist.zlb'))
            || fmt == 'romlist') {
                this.viewer = new RomListViewer(this.app, this.view);
            }
            else if((fmt == 'auto' && this.file.path.startsWith('/gametext'))
            || fmt == 'gametext') {
                this.viewer = new GameTextViewer(this.app, this.view);
            }
            else if(fmt == 'text') {
                this.viewer = new TextViewer(this.app, this.view);
            }
            else this.viewer = new HexViewer(this.app, this.view);
        }
        catch(ex) {
            //XXX instead of showing "not a GameText file" when set to Auto,
            //we should just switch to hex view...
            console.error(ex);
            this.viewer = new ErrorMessage(this.app, ex.toString());
        }
    }

    refresh() {
        this._makeViewer();
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
