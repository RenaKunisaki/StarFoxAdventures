import { E } from "../../lib/Element.js";
import { hex, int, Percent } from "../../Util.js";
import Table from "./Table.js";

export default class FileList {
    /** Displays list of files in ISO.
     */
    constructor(app) {
        this.app = app;
        this.element = document.getElementById('tab-fileList');
        this.app.onIsoLoaded(iso => this.refresh(iso));
    }

    refresh(iso) {
        let tbl = this._makeTable();
        for(let file of iso.files) {
            console.log("file", file);
            tbl.add(this._makeRow(file));
        }
        const elem = E.div('fileList', tbl.element);
        this.element.replaceWith(elem);
        this.element = elem;
    }

    _makeTable() {
        return new Table({columns: [
            {displayName:"Path", name:'path', type:'str'},
            {displayName:"Size", name:'size', type:'int'},
        ]});
    }

    _makeRow(file) {
        return {
            path: file.path,
            size: file.size,
        };
    }
}
