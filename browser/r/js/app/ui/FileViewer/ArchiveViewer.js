import GameFile from "../../../game/GameFile.js";
import IsoFile from "../../../types/iso/isofile.js";
import { E, clearElement } from "../../../lib/Element.js";
import Table from "../Table.js";
import { download, hex, Percent } from "../../../Util.js";

export default class ArchiveViewer {
    constructor(app, data) {
        this.app = app;
        if(data instanceof GameFile) this.file = data;
        else if(data instanceof IsoFile) {
            this.file = new GameFile(data.getData(), 0, data.size);
        }
        else if(data instanceof DataView) {
            this.file = new GameFile(data.buffer,
                data.byteOffset, data.byteLength);
        }
        else throw new Error("Unsupported object for file: "+(typeof data));
        this.eBody   = E.div();
        this.element = E.div('archiveviewer', this.eBody);
        this.cbView  = null; //callback when we click View button.
        this.refresh();
    }

    refresh() {
        const contents = this.file.getContents();
        console.log("File contents:", contents, this.file);
        let tbl = this._makeTable();
        let i   = 0;
        for(let item of contents) {
            tbl.add(this._makeRow(i++, item));
        }
        clearElement(this.eBody).append(tbl.element);
    }

    _makeTable() {
        return new Table({title:"Archive Contents", columns: [
            {displayName:"#", name:'idx', type:'int'},
            {displayName:"Offset", name:'fileOffset', type:'hex', length:6},
            {displayName:"End",    name:'endOffset', type:'hex', length:6},
            {displayName:"Format", name:'fmt', type:'string'},
            {displayName:"Packed", name:'packedSize', type:'hex', length:6,
                title:"Packed size"},
            {displayName:"Raw",    name:'unpackedSize', type:'hex', length:6,
                title:"Unpacked size"},
            {displayName:"Pack",   name:'packRatio', type:'int',
                title:"Compression ratio"},
            {displayName:"Actions",name:'actions', type:'string',
                makeElem: (val, td, row) => this._makeActions(row)},
        ]});
    }

    _makeRow(idx, item) {
        return Object.assign({
            idx:       idx,
            endOffset: item.fileOffset + item.fileSize,
            packRatio: Percent(item.packedSize / item.unpackedSize),
        }, item); //copy
    }

    _makeActions(row) {
        const bView     = E.button('view', "View");
        const bDownload = E.button('download', "Download");
        bView    .addEventListener('click', e => this._viewFile(row));
        bDownload.addEventListener('click', e => this._downloadFile(row));
        return E.span(bView, bDownload);
    }

    _viewFile(row) {
        //called when clicking View button.
        if(this.cbView) this.cbView(row, this.file.decompress(row.fileOffset));
        else console.error("No view callback installed");
    }

    _downloadFile(row) {
        //called when clicking Download button.
        download(this.file.decompress(row.fileOffset),
            `${hex(row.fileOffset,6)}.bin`);
    }
}
