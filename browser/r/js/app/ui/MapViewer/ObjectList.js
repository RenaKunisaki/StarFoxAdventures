import { clearElement, E } from "../../../lib/Element.js";
import Table from "../Table.js";
import { NUM_MAP_ACTS } from "./LayerChooser.js";

//struct types
let ObjCatId;

export default class ObjectList {
    /** Widget listing all objects in the map. */
    constructor(mapViewer) {
        this.mapViewer = mapViewer;
        this.game      = mapViewer.game;
        this.app       = mapViewer.game.app;

        this.table = this._makeTable();
        this.element = E.div('map-object-list',
            E.details(
                E.summary(null, "Objects"),
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
                    //XXX make the entire row clickable?
                    td.addEventListener('click',
                        e => this.mapViewer.showObject(row.entry));
                    return td;
                },
            },
            {displayName:"Cat", name:'category', type:'string'},
            {displayName:"Grp", name:'group', type:'int'},
            {displayName: "A", name:'act1',  type:'string'},
            {displayName: "c", name:'act2',  type:'string'},
            {displayName: "t", name:'act3',  type:'string'},
            {displayName: "4", name:'act4',  type:'string'},
            {displayName: "5", name:'act5',  type:'string'},
            {displayName: "6", name:'act6',  type:'string'},
            {displayName: "7", name:'act7',  type:'string'},
            {displayName: "8", name:'act8',  type:'string'},
            {displayName: "9", name:'act9',  type:'string'},
            {displayName:"10", name:'act10', type:'string'},
            {displayName:"11", name:'act11', type:'string'},
            {displayName:"12", name:'act12', type:'string'},
            {displayName:"13", name:'act13', type:'string'},
            {displayName:"14", name:'act14', type:'string'},
            {displayName:"15", name:'act15', type:'string'},
            {displayName:"X", name:'x', type:'float', decimals:0},
            {displayName:"Y", name:'y', type:'float', decimals:0},
            {displayName:"Z", name:'z', type:'float', decimals:0},
        ]});
    }

    _makeRow(entry) {
        const row = {
            entry: entry,
            idx:   entry.idx,
            id:    entry.id,
            name:  entry.object.header.name,
            x:     entry.position.x,
            y:     entry.position.y,
            z:     entry.position.z,
            group: entry.group,
            category: ObjCatId.valueToString(entry.object.header.catId),
        };
        for(let i=1; i<NUM_MAP_ACTS; i++) {
            row[`act${i}`] = entry.acts[i] ? '✔' : '';
        }
        return row;
    }

    refresh() {
        if(this.app.types) {
            ObjCatId = this.app.types.getType('sfa.objects.ObjCatId');
        }
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
