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
        try {
            this.app        = app;
            this.file       = file;
            this._startOffs = 0;
            this.element    = E.div('fileViewer');
            this.viewer     = null;
            this.showTitle  = showTitle;
            this.error      = null;
            this.archiveIdx = null; //item idx of archive we're viewing (null=none)
            this.gameFile   = new GameFile(this.file);
            this.view       = this.gameFile;
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

        this.eOffset = E.input('offset hex', {id:'viewOffset', value:'0'});
        this.eOffset.addEventListener('change', e => {
            const offs = parseInt(this.eOffset.value, 16);
            if(!isNaN(offs)) {
                this._startOffs = offs;
                this.refresh();
            }
        })

        this.eToolbar = E.div('toolbar',
            E.label(null, "View as:", {For:'formatSelect'}),
            this.eFormatSel,
            E.label(null, "Offset:", {for:'viewOffset'}),
            this.eOffset,
        );
    }

    _makeViewer() {
        const fmt      = this.eFormatSel.value;
        const name     = this.file.name;
        let   offs     = this._startOffs;
        const contents = this.gameFile.getContents(offs);

        //if this is an archive with only one item (like many compressed
        //files) then just show that item. otherwise show the raw data and
        //let user choose an item.
        //this means we can't view the compressed form of such an archive,
        //but I don't think that's very important.
        let buf = this.view;
        if(contents.length == 1) buf = this.gameFile.getItem(0, offs);
        else if(this.archiveIdx != null) {
            buf  = this.gameFile.getItem(this.archiveIdx, offs);
        }
        else buf = this.gameFile.getView(offs);
        if(!(buf instanceof DataView)) buf = new DataView(buf);

        try {
            if(this.error) {
                this.viewer = new ErrorMessage(this.app, this.error.toString());
            }
            else if(buf.byteLength == 0) {
                this.viewer = new ErrorMessage(this.app, "File is empty");
            }
            //if we're viewing an item in the archive, don't show the
            //archive's contents by default.
            //we can still switch the view mode to Archive to see them,
            //which is how we go back to the list from an item.
            else if((fmt == 'auto' && this.archiveIdx == null
            && contents.length > 1) || fmt == 'archive') {
                this.viewer = new ArchiveViewer(this.app, buf);
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
                this.viewer = new RomListViewer(this.app, buf);
            }
            else if((fmt == 'auto' && this.file.path.startsWith('/gametext'))
            || fmt == 'gametext') {
                this.viewer = new GameTextViewer(this.app, buf);
            }
            else if(fmt == 'text') {
                this.viewer = new TextViewer(this.app, buf);
            }
            else this.viewer = new HexViewer(this.app, buf);
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
