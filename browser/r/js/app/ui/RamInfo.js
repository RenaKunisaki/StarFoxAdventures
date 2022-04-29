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
        const ram  = this.app.ramDump;
        const view = new DataView(ram.data.buffer);
        this._objDatas = {}; //objdef => data

        //get the object pointer list
        const aNObjs = ram.addrToOffset(this.game.addresses.nLoadedObjs.address);
        ram.data.seek(aNObjs);
        const nObjs = ram.data.readU32();
        const aObjs = ram.addrToOffset(this.game.addresses.loadedObjs.address)
        ram.data.seek(aObjs);
        const pObjs = ram.addrToOffset(ram.data.readU32());
        if(!(pObjs && nObjs)) {
            return E.div('error', "No objects loaded");
        }
        ram.data.seek(pObjs);
        const objList = ram.data.readU32(nObjs);

        //get the player object
        ram.data.seek(ram.addrToOffset(
            this.game.addresses.pPlayer.address));
        this.pPlayer = ram.data.readU32();
        if(this.pPlayer) {
            this.player = ObjInstance.fromBytes(view,
                ram.addrToOffset(this.pPlayer));
        }
        else this.player = null;

        //make the table
        const tbl = this._makeObjListTable();

        //populate the table
        for(let iObj=0; iObj<nObjs; iObj++) {
            tbl.add(this._makeObjListRow(ram, view, iObj, objList[iObj]));
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

    _makeObjListRow(ram, view, iObj, addr) {
        const obj = ObjInstance.fromBytes(view, ram.addrToOffset(addr));
        let data  = this._objDatas[obj.defNo];
        if(!data) {
            data = ObjectData.fromBytes(view, ram.addrToOffset(obj.data));
            this._objDatas[obj.defNo] = data;
        }
        const row = {
            idx:   iObj,
            addr:  addr,
            data:  obj.data,
            def:   obj.objDef,
            defNo: obj.defNo,
            name:  data.name,
            state: obj.state,
            x:     obj.xf.pos.x,
            y:     obj.xf.pos.y,
            z:     obj.xf.pos.z,
        };

        if(this.player) {
            const dx = obj.xf.pos.x - this.player.xf.pos.x;
            const dy = obj.xf.pos.y - this.player.xf.pos.y;
            const dz = obj.xf.pos.z - this.player.xf.pos.z;
            row.dist = Math.sqrt((dx*dx)+(dy*dy)+(dz*dz));
        }
        else row.dist = null;

        return row;
    }
}
