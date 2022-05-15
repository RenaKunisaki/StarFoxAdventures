import { clearElement, E } from "../../../lib/Element.js";
import Table from "../Table.js";

export default class ObjectList {
    /** Widget listing all objects in the map. */
    constructor(mapViewer) {
        this.mapViewer = mapViewer;
        this.game      = mapViewer.game;
        this.app       = mapViewer.game.app;

        this.table = this._makeTable();
        this.element = E.div('map-object-list',
            E.details(
                E.summary(null, "Object List"),
                this.table.element,
            ),
        );
    }

    _makeTable() {
        return new Table({title:"Objects", columns: [
            {displayName:"#",    name:'idx',  type:'int'},
            {displayName:"ID",   name:'id',   type:'hex', length:8},
            {displayName:"Name", name:'name', type:'string',
                makeElem: (val, td, row) => {
                    td.addEventListener('click',
                        e => this.mapViewer.showObject(row.entry));
                    return td;
                },
            },
        ]});
    }

    _makeRow(entry) {
        return {
            entry: entry,
            idx:   entry.idx,
            id:    entry.id,
            name:  entry.object.header.name,
        };
    }

    refresh() {
        if(!this.mapViewer.map) return;
        let entries = this.mapViewer.map.romList;
        if(entries) entries = entries.entries;
        if(!entries) entries = [];
        this.table.clear();
        for(let entry of entries) {
            this.table.add(this._makeRow(entry));
        }
    }
}
