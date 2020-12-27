import {E} from './Element.js';
import Table from './Table.js';
import {get, hex, int, makeList, makeCollapsibleList, makeDescriptionAndNotes} from './Util.js';

export default class ObjInfo {
    /** A page showing details of a GameObject.
     */
    constructor(app) {
        this.app = app;
        this.element = E.div('objinfo');
        const id = this.app.params.get('id');
        this.obj = this.app.game.objects[id];
    }

    getTitle() { return `${this.obj.name} - Object Info` }

    async run() {
        this.element.append(this.makeSummaryTable());
        this.element.append(this.makeParamsTable());
        this.element.append(this.makeModelsTable());
        this.element.append(this.makeMapsTable());
    }

    makeSummaryTable() {
        const obj = this.obj;
        return E.div('box object-summary',
            E.h2('header', "Summary"),
            E.table(
                E.tr(E.th(null, "Name"),          E.td('str', obj.name)),
                E.tr(E.th(null, "ID"),            E.td('hex', hex(obj.id, 4))),
                E.tr(E.th(null, "DefNo"),         E.td('hex', hex(obj.defNo, 4))),
                E.tr(E.th(null, "DLL"),           E.td('hex', hex(obj.dllId, 4))),
                E.tr(E.th(null, "# Player Objs"), E.td('hex', hex(obj.nPlayerObjs, 4))),
                E.tr(E.th(null, "Type"),          E.td('str', obj.type)),
                E.tr(E.th(null, "Spawns"),        E.td('str', makeList(obj.spawns))),
                E.tr(E.th(null, "Description"),   E.td('str', makeDescriptionAndNotes(obj))),
            )
        );
    }

    makeParamsTable() {
        const obj = this.obj;
        const title = obj.paramLength == null ? "Params" : `Params (0x${hex(obj.paramLength,2)})`;
        const tbl = new Table({
            title: title,
            columns: [
                {
                    displayName: "Offs",
                    name: 'offset',
                    type: 'hex',
                    length: 4,
                }, {
                    displayName: "Type",
                    name: 'type',
                    type: 'string',
                }, {
                    displayName: "Name",
                    name: 'name',
                    type: 'string',
                }, {
                    displayName: "Description",
                    name: 'description',
                    type: 'string',
                    format: (desc, col, row) => makeDescriptionAndNotes(row),
                },
            ]});
        for(let param of obj.params) {
            tbl.add({
                offset:      param.offset,
                type:        param.type,
                name:        param.name,
                description: param.description,
                notes:       param.notes,
            });
        }
        if(obj.exampleParams) {
            tbl.add({
                offset:      null,
                type:        null,
                name:        "Example",
                description: obj.exampleParams,
            });
        }
        return E.div('box object-params', tbl.element);
    }

    makeModelsTable() {
        const obj = this.obj;
        const tbl = E.table('model-list',
            E.tr(
                E.th(null, "ID"),
                E.th(null, "Description"),
            ),
        );
        for(let i=0; i<obj.modelIds.length; i++) {
            let id   = obj.modelIds[i];
            let desc = obj.models[i];
            desc = desc ? desc.description : '-';
            tbl.append(E.tr(
                E.td('hex', hex(id,4)),
                E.td('str', desc),
            ));
        }
        return E.div('box object-models', E.h2(null, "Models"), tbl);
    }

    makeMapsTable() {
        const v = this.app.game.version;
        const obj = this.obj;
        const tbl = E.table('map-list',
            E.tr(
                E.th(null, "Map", {colspan:2}),
                E.th(null, "Count"),
            ),
        );
        for(let [id, count] of Object.entries(obj.maps)) {
            const name = this.app.game.maps[id].internalName;
            tbl.append(E.tr(
                E.td('hex', hex(id,2)),
                E.td('str', E.a('maplink', name, {
                    href: `?v=${v}&p=map&id=${id}`,
                })),
                E.td('int', count.toString()),
            ));
        }
        return E.div('box object-maps', E.h2(null, "Maps"), tbl);
    }
}
