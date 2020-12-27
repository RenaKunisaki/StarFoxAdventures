import {E} from './Element.js';
import Table from './Table.js';
import {hex, makeCollapsibleList, makeDescriptionAndNotes} from './Util.js';

export default class ObjList {
    /** A page listing the objects in the game.
     */
    constructor(app) {
        this.app = app;
        this._makeTable();
        this.element = this.tbl.element;
    }

    getTitle() { return "Objects" }

    async run() {
        for(let [id, obj] of Object.entries(this.app.game.objects)) {
            this.tbl.add(this._makeRow(obj));
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
                }, {
                    displayName: "Def",
                    name: 'defNo',
                    type: 'hex',
                    length: 4,
                }, {
                    displayName: "Name",
                    name: 'name',
                    type: 'string',
                    format: (val, col, row) => E.a(null, {
                        href:`?v=${v}&p=obj&id=${row.id}`
                    }, val),
                }, {
                    displayName: "DLL",
                    name: 'dll',
                    type: 'string',
                    format: (dll, col, row) => {
                        if(dll == null || dll == undefined) return '-';
                        return E.a(null, {
                            href: `?v=${v}&p=dll&id=${dll.id}`,
                        }, `${hex(dll.id,4)} ${dll.name}`);
                    },
                }, {
                    displayName: "NP",
                    name: 'nPlayerObjs',
                    type: 'int',
                }, {
                    displayName: "Description",
                    name: 'description',
                    type: 'string',
                    format: (desc, col, row) => makeDescriptionAndNotes(row),
                }, {
                    displayName: "Models",
                    name: 'modelTbl',
                    type: 'string',
                }, {
                    displayName: "Params",
                    name: 'paramTbl',
                    type: 'string',
                },
            ]});
    }

    _makeRow(obj) {
        /** Make table row for object.
         */
        return {
            cls:         'object',
            id:          obj.id,
            name:        obj.name,
            defNo:       obj.defNo,
            dll:         obj.dll,
            nPlayerObjs: obj.nPlayerObjs,
            type:        obj.type,
            description: obj.description,
            notes:       obj.notes,
            modelTbl:    this._makeModelsTable(obj),
            params:      obj.params,
            paramTbl:    this._makeParamsTable(obj),
        };
    }

    _makeParamsTable(obj) {
        /** Make params sub-table for object.
         */
        if(obj.params.length == 0) return '';
        const tbl = E.table('params',
            E.tr(
                E.th(null, 'Offs'),
                E.th(null, 'Type'),
                E.th(null, 'Name'),
                E.th(null, 'Description'),
            )
        );
        for(let param of obj.params) {
            let name = param.name;
            if(name == null) name = '';
            tbl.append(E.tr(
                E.td(null, hex(param.offset, 2)),
                E.td(null, param.type),
                E.td(null, name),
                E.td(null, param.description),
            ));
        }
        let example = obj.exampleParams;
        if(example == null) {
            example = obj.paramLength;
            if(example == undefined || example == null) example = '[...]';
            else example = `0x${hex(example,2)} bytes`;
        }
        return E.details('params', E.summary(null, example), tbl);
    }

    _makeModelsTable(obj) {
        const items = [];
        for(let i=0; i<obj.modelIds.length; i++) {
            let id   = obj.modelIds[i];
            let desc = obj.models[i];
            desc = desc ? desc.description : '-';
            items.push(`${hex(id,4)} ${desc}`);
        }
        return makeCollapsibleList(items);
    }

    _OLD_makeModelsTable(obj) {
        /** Make models sub-table for object.
         */
        const tModels = E.table('models');
        for(const mod of obj.getElementsByTagName('model')) {
            const mid = mod.getAttribute('id').substr(2);

            //get models.xml data
            let infoText = '', isMissing = false, modelType = 'unknown';
            const modInfo = modelInfo.getElementById('0x'+mid);
            if(modInfo) {
                modelType = modInfo.getAttribute('type');
                try { infoText = modInfo.getElementsByTagName('description')[0].textContent; }
                catch(ex) {}
            }

            //check if present in any map
            if(!assetList.evaluate(
                `//assets/models/model[@idx="0x${mid}"]/map`, assetList,
                null, XPathResult.ANY_UNORDERED_NODE_TYPE, null).singleNodeValue) {
                isMissing = true;
            }

            //get in-game text
            let gameText = null;
            let textId = mod.getAttribute('text');
            if(textId != null) {
                let id   = parseInt(textId) + 10000;
                let sid  = '0x' + id.toString(16).padStart(4, '0');
                gameText = gameTexts.getElementById(sid);
                if(gameText) {
                    let phrases = [];
                    for(let elem of gameText.getElementsByTagName('phrase')) {
                        phrases.push(elem.textContent);
                    }
                    gameText = phrases.join('\n');
                }
                else gameText = '<'+sid.substr(2)+'>';
            }

            //build row
            const trModel = E.tr('model model-type-'+modelType);

            //build link/ID cell
            trModel.append(
                E.td('id hex', E.a(isMissing ? 'missing-model' : '', {
                    href:'/modelview.html#'+mid,
                    title: infoText,
                }, mid)),
            );

            //build text cell
            if(gameText != null) {
                const tdText = E.td('gametext str', gameText);
                if(textId != null) tdText.setAttribute('title', "Text ID: "+textId);
                trModel.append(tdText);
            }
            else trModel.append(E.td('str', infoText));

            tModels.append(trModel);
        }
        return E.td('models', tModels);
    }

    _makeMapsTable(obj) {
        /** Make maps sub-table for object.
         */
        const tMaps = E.table('maps');
        for(const mapItem of obj.getElementsByTagName('map')) {
            const mid   = mapItem.getAttribute('id').substr(2);
            const count = mapItem.getAttribute('count');
            const map   = maps.getElementById('0x'+mid);
            let   name  = map.getAttribute('name');
            if(name == null) name = map.getAttribute('internalName');
            if(name == null) name = '-';

            const trMap = E.tr('map');
            trMap.append(E.td('id hex', mid));
            trMap.append(E.td('name str', name));
            trMap.append(E.td('count number', count));
            tMaps.append(trMap);
        }
        return E.td('maps', tMaps);
    }
}
