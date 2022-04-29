import Game from "../../game/Game.js";
import { E, clearElement } from "../../lib/Element.js";
import { assertType, hex } from "../../Util.js";
import Table from "./Table.js";

//struct types
let ObjInstance, ObjectData;

export default class RamInfo {
    /** Displays info from RAM dump.
     */
    constructor(game) {
        this.game    = assertType(game, Game);
        this.app     = game.app;
        this.element = document.getElementById('tab-ramInfo');
        this.app.onRamLoaded(ram => this.refresh());
    } //constructor

    refresh() {
        ObjInstance = this.app.types.getType('sfa.objects.ObjInstance');
        ObjectData = this.app.types.getType('sfa.objects.ObjectData');
        const elem = E.div('ramInfo', this._makeLoadedObjWidget());
        clearElement(this.element).append(elem);
    }

    _makeLoadedObjWidget() {
        /** Create the widget that will show the list of loaded objects. */
        const ram = this.app.ramDump;

        const aNObjs = ram.addrToOffset(this.game.addresses.nLoadedObjs.address);
        ram.data.seek(aNObjs);
        const nObjs = ram.data.readU32();

        let loaded = false;
        let eList = E.div('loading', "Loading...");
        const eObjs = E.details('objlist',
            E.summary(null, `Objects: ${nObjs.toLocaleString()}`),
            eList,
        );
        eObjs.addEventListener('toggle', e => {
            if(!eObjs.open) return;
            if(loaded) return;
            const lst = this._makeLoadedObjList();
            clearElement(eList).append(lst);
            loaded = true;
        })
        return eObjs;
    }

    _makeLoadedObjList() {
        /** Create the list of loaded objects. */
        const ram = this.app.ramDump;
        const tbl = this._makeObjListTable();

        //populate the table
        const objs = ram.getLoadedObjects();
        for(let iObj=0; iObj<objs.length; iObj++) {
            tbl.add(this._makeObjListRow(ram, iObj, objs[iObj]));
        }

        return tbl.element;
    }

    _makeObjListTable() {
        return new Table({title:"Objects", columns: [
            {displayName:"#", name:'idx', type:'hex', length:4,
                title:"Index in object list"},
            {displayName:"Def#", name:'defNo',   type:'hex', length:4,
                title:"ObjDef ID"},
            {displayName:"Name", name:'name',   type:'string',
                title:"ObjDef Name"},
            {displayName:"UniqueID", name:'id',   type:'hex', length:8,
                title:"Object unique ID"},
            {displayName:"RAM Addr", name:'addr',   type:'hex', length:8,
                title:"ObjInstance address"},
            {displayName:"DataAddr", name:'data',   type:'hex', length:8,
                title:"ObjData address"},
            {displayName:"Def Addr", name:'def',    type:'hex', length:8,
                title:"ObjDef address"},
            {displayName:"StateAdr", name:'state',  type:'hex', length:8,
                title:"Obj state address"},
            {displayName:"X", name:'x', type:'float', decimals:4, title:"X Coord"},
            {displayName:"Y", name:'y', type:'float', decimals:4, title:"Y Coord"},
            {displayName:"Z", name:'z', type:'float', decimals:4, title:"Z Coord"},
            {displayName:"Dist", name:'dist', type:'float', decimals:4, title:"Distance to Player"},
        ]});
    }

    _makeObjListRow(ram, iObj, obj) {
        const row = {
            idx:   iObj,
            addr:  obj.addr,
            data:  obj.ObjInstance.data,
            def:   obj.ObjInstance.objDef,
            defNo: obj.ObjInstance.defNo,
            name:  obj.ObjectData.name,
            id:    obj.RomListEntry ? obj.RomListEntry.id : null,
            state: obj.ObjInstance.state,
            x:     obj.ObjInstance.xf.pos.x,
            y:     obj.ObjInstance.xf.pos.y,
            z:     obj.ObjInstance.xf.pos.z,
        };

        if(ram.player) {
            const dx = obj.ObjInstance.xf.pos.x - ram.player.xf.pos.x;
            const dy = obj.ObjInstance.xf.pos.y - ram.player.xf.pos.y;
            const dz = obj.ObjInstance.xf.pos.z - ram.player.xf.pos.z;
            row.dist = Math.sqrt((dx*dx)+(dy*dy)+(dz*dz));
        }
        else row.dist = null;

        return row;
    }
}
