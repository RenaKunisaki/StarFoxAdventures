import { E } from "../../lib/Element.js";
import { hex } from "../../Util.js";
import Table from "./Table.js";

export default class Warptab {
    /** Displays all warp definitions.
     */
    constructor(app) {
        this.app = app;
        this.element = document.getElementById('tab-warpTab');
        this.app.onIsoLoaded(iso => this.refresh());
    } //constructor

    refresh() {
        let tbl = this._makeTable();
        for(let [idx, warp] of Object.entries(this.app.game.warpTab)) {
            tbl.add(this._makeRow(idx, warp));
        }
        const elem = E.div('warpTab', tbl.element);
        this.element.replaceWith(elem);
        this.element = elem;
    }

    _makeTable() {
        return new Table({columns: [
            {displayName:"#", name:'idx', type:'hex', length:2},
            {displayName:"X", name:'x',   type:'float'},
            {displayName:"Y", name:'y',   type:'float'},
            {displayName:"Z", name:'z',   type:'float'},
            {displayName:"L", name:'layer', type:'int', title:"Map Layer"},
            {displayName:"R", name:'xRot', type:'int', title:"X Rotation"},
            {displayName:"Map", name:'map', type:'string'},
        ]});
    }

    _makeRow(idx, warp) {
        const row = {
            idx:   parseInt(idx),
            x:     warp.x,
            y:     warp.y,
            z:     warp.z,
            layer: warp.layer,
            xRot:  warp.xRot,
        };
        if(this.app.game.mapGrid) {
            let map = this.app.game.getMapAt(warp.layer, warp.x, warp.z);
            if(map) row.map = map.name;
            else row.map = '-';
        }
        return row;
    }
}
