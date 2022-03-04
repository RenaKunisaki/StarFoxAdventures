import GameFile from "../../../game/GameFile.js";
import IsoFile from "../../../types/iso/isofile.js";
import { E, clearElement } from "../../../lib/Element.js";
import Table from "../Table.js";
import { Percent } from "../../../Util.js";

export default class ArchiveViewer {
    constructor(app, data) {
        this.app = app;
        if(data instanceof GameFile) this.file = data;
        else if(data instanceof IsoFile) {
            this.file = new GameFile(data.getRawData(), 0, data.size);
        }
        else if(data instanceof DataView) {
            this.file = new GameFile(data.buffer,
                data.byteOffset, data.byteLength);
        }
        else throw new Error("Unsupported object for file: "+(typeof data));
        this.eBody   = E.div();
        this.element = E.div('archiveviewer', this.eBody);
        this.refresh();
    }

    refresh() {
        const contents = this.file.getContents();
        console.log("File contents:", contents, this.file);
        let tbl = this._makeTable();
        let i = 0;
        for(let item of contents) {
            tbl.add(this._makeRow(i++, item));
            //rows.push([i++, item.fmt, fileSize(item.packedSize),
            //    fileSize(item.unpackedSize), hex(item.headerOffs, 6)]);
        }
        clearElement(this.eBody).append(tbl.element);
    }

    _makeTable() {
        return new Table({title:"Archive Contents", columns: [
            {displayName:"#", name:'idx', type:'int'},
            {displayName:"Offset", name:'headerOffs', type:'hex', length:6},
            {displayName:"Format", name:'fmt', type:'string'},
            {displayName:"Packed", name:'packedSize', type:'hex', length:6,
                title:"Packed size"},
            {displayName:"Raw",    name:'unpackedSize', type:'hex', length:6,
                title:"Unpacked size"},
            {displayName:"Pack",    name:'packRatio', type:'int',
                title:"Compression ratio"},
        ]});
    }

    _makeRow(idx, item) {
        return Object.assign({
            idx: idx,
            packRatio: Percent(item.packedSize / item.unpackedSize),
        }, item);
    }
}
