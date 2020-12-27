import {E} from './Element.js';
import Table from './Table.js';
import {hex, makeCollapsibleList, makeDescriptionAndNotes} from './Util.js';

export default class DllList {
    /** A page listing the DLLs in the game.
     */
    constructor(app) {
        this.app = app;
        this._makeTable();
        this.element = this.tbl.element;
    }

    getTitle() { return "DLLs" }

    async run() {
        this._objMap = this._buildObjMap();
        for(let [id, dll] of Object.entries(this.app.game.dlls)) {
            this.tbl.add(this._makeRow(dll));
        }
    }

    _makeTable() {
        const v = this.app.game.version;
        this.tbl = new Table({
            columns: [
                {
                    displayName: "ID",
                    name: 'id',
                    type: 'hex',
                    format: (id, dll) => E.a(null, {
                        href:`?v=${v}&p=dll&id=${id}`
                    }, hex(id, 3)),
                }, {
                    displayName: "Name",
                    name: 'name',
                    type: 'string',
                }, {
                    displayName: "Objs",
                    name: 'objs',
                    type: 'string',
                    compareFunc: (a, b) => a.nObjs - b.nObjs,
                }, {
                    displayName: "Addr",
                    name: 'address',
                    type: 'hex',
                    length: 8,
                }, {
                    displayName: "Offs",
                    name: 'dolOffs',
                    type: 'hex',
                    length: 6,
                }, {
                    displayName: "Funcs",
                    name: 'functions',
                    type: 'string',
                    compareFunc: (a, b) => a.functions.length - b.functions.length,
                }, {
                    displayName: "Non Stub Funcs",
                    name: 'nNonStubs',
                    type: 'string',
                }, {
                    displayName: "Iface",
                    name: 'interface',
                    type: 'string',
                }, {
                    displayName: "Unk08",
                    name: 'unk08',
                    type: 'hex',
                    length: 8,
                }, {
                    displayName: "Unk0E",
                    name: 'unk0E',
                    type: 'hex',
                    length: 4,
                }, {
                    displayName: "Prev",
                    name: 'prev',
                    type: 'hex',
                    length: 8,
                }, {
                    displayName: "Next",
                    name: 'next',
                    type: 'hex',
                    length: 8,
                }, {
                    displayName: "Description",
                    name: 'description',
                    type: 'string',
                    format: (desc, col, row) => makeDescriptionAndNotes(row),
                },
            ]});
    }

    _makeFuncList(dll) {
        /** Make HTML list of a DLL's functions.
         */
        const tbl = E.table('funclist', E.tr(
            E.th(null, '#'),
            E.th(null, "Address"),
            E.th(null, "Stub?"),
        ));
        for(let func of dll.functions) {
            let stub = 'No';
            if(func.isStub) {
                const ret = func.stubReturn;
                if(ret != undefined) stub = `Yes (${ret})`;
                else stub = 'Yes';
            }
            tbl.append(E.tr('function',
                E.th('idx hex', hex(func.idx, 2)),
                E.td('address hex ' + (func.isValid ? '' : 'in') + 'valid',
                    hex(func.address, 8)),
                E.td('stub', stub),
            ));
        }
        return E.details(E.summary('count', dll.functions.length.toString()), tbl);
    }

    _buildObjMap() {
        /** Build a map of which objects reference each DLL.
         */
        const dlls = {};
        for(let [id, obj] of Object.entries(this.app.game.objects)) {
            if(!isNaN(obj.dllId)) {
                if(dlls[obj.dllId] == undefined) dlls[obj.dllId] = [];
                dlls[obj.dllId].push(obj);
            }
        }
        return dlls;
    }

    _makeRow(dll) {
        /** Make table row for DLL.
         */
        const objs = this._objMap[dll.id];
        return {
            cls:         'dll',
            id:          dll.id,
            address:     dll.address,
            dolOffs:     dll.dolOffs,
            interface:   dll.interface,
            name:        dll.name,
            unk08:       dll.unk08,
            unk0E:       dll.unk0E,
            prev:        dll.prev,
            next:        dll.next,
            objs:        objs ? this._makeObjList(dll, objs) : '-',
            nObjs:       objs ? objs.length : 0,
            description: dll.description,
            notes:       dll.notes,
            functions:   this._makeFuncList(dll),
            nNonStubs:   dll.nNonStubs,
        };
    }

    _makeObjLink(obj) {
        const v = this.app.game.version;
        return E.a(null, {href: `?v=${v}&p=obj&id=${obj.id}`},
            `${hex(obj.id,4)}/${hex(obj.defNo,4)} ${obj.name}`);
    }

    _makeObjList(dll, objs) {
        const items = [];
        for(let obj of objs) {
            items.push(this._makeObjLink(obj));
        }
        return makeCollapsibleList(items);
    }
}
