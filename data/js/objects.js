/** This script reads in some JSON/XML files containing data dumped from the game
 *  and produces a nice formatted table of all objects from them.
  */
import {E} from './Element.js';
import {get, getObjects, getGameTexts, getMaps, getBits, finishLoading, setupMain} from './Util.js';

let gameTexts, maps, gameBits, modelInfo;

function makeTableForObject(objs, obj) {
    let   id    = obj.getAttribute('id').substr(2);
    let   defNo = obj.getAttribute('def').substr(2);
    let   name  = obj.getAttribute('name');
    let   dll   = obj.getAttribute('dll');
    if(dll == null) dll = '-';
    else dll = dll.substr(2);
    const tr    = E.tr('object', {id:'0x'+id});

    tr.append(E.td('id hex',    id));
    tr.append(E.td('defno hex', defNo));
    tr.append(E.td('name str',  name));
    tr.append(E.td('dll hex',   dll));

    //make models subtable
    const tModels = E.table('models');
    for(const mod of obj.getElementsByTagName('model')) {
        const mid = mod.getAttribute('id').substr(2);

        //get models.xml data
        let infoText = '', isMissing = false, modelType = 'unknown';
        const modInfo = modelInfo.getElementById('0x'+mid);
        if(modInfo) {
            isMissing = modInfo.getAttribute('missing');
            modelType = modInfo.getAttribute('type');
            try { infoText = modInfo.getElementsByTagName('description')[0].textContent; }
            catch(ex) {}
        }

        //get in-game text
        let gameText = null;
        let textId = mod.getAttribute('text');
        if(textId != null) {
            gameText = gameTexts[String(parseInt(textId) + 10000)];
            if(gameText) gameText = gameText.phrases.join('; ');
            else gameText = '<'+textId.substr(2)+'>';
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
    tr.append(E.td('models', tModels));

    //make maps subtable
    const tMaps = E.table('maps');
    for(const mapItem of obj.getElementsByTagName('map')) {
        const mid   = mapItem.getAttribute('id').substr(2);
        const count = mapItem.getAttribute('count');
        const map   = maps.getElementById('0x'+mid);

        const trMap = E.tr('map');
        trMap.append(E.td('id hex', mid));
        trMap.append(E.td('name str', map.textContent));
        trMap.append(E.td('count number', count));
        tMaps.append(trMap);
    }
    tr.append(E.td('maps', tMaps));

    //get description
    let descr = obj.getElementsByTagName('description');
    if(descr) descr = descr[0].textContent;
    else descr = '';
    tr.append(E.td('description str', descr));

    //make params subtable
    let params = obj.getElementsByTagName('params');
    if(params.length == 0) {
        tr.append(E.td('params unknown', ""));
    }
    else {
        let count = 0;
        const tParams = E.table('params');
        let pLen = params[0].getAttribute('length');
        for(const param of params[0].childNodes) {
            if(param.tagName) { //skip text nodes lol
                let offs = param.getAttribute('offset');
                if(offs != null) offs = offs.substr(2);
                tParams.append(E.tr('param '+param.tagName,
                    E.td('offset hex', offs),
                    E.td('datatype', param.tagName),
                    E.td('description str', param.textContent),
                ));
                count++;
            }
        }
        if(count || (pLen != null && parseInt(pLen) > 0x18)) {
            if(pLen != null) {
                pLen = pLen.substr(2);
                let example = obj.getElementsByTagName('exampleParams');
                if(example.length) example = example[0].textContent;
                else example = '';
                tParams.append(E.tr('param end',
                    E.td('offset hex', pLen),
                    E.td('datatype', "end"),
                    E.td('example hex', example),
                ));
            }
            tr.append(E.td('params', tParams));
        }
        else tr.append(E.td('params empty', "none"));
    }

    return tr;
}

function makeExampleObject() {
    return E.tr('example object',
        E.td('id',       "ID"),
        E.td('defno',    "Def#"),
        E.td('name str', "Example"),
        E.td('dll',      "DLL#"),
        E.td('models',
            E.table('models',
                E.tr('model',
                    E.td('id', "ID"),
                    E.td('gametext str', "In-Game Description Text"),
                ),
            ),
        ),
        E.td('maps',
            E.table('maps',
                E.tr('map',
                    E.td('id', "ID"),
                    E.td('name str', "Map Name"),
                    E.td('count', "Count"),
                ),
            ),
        ),
        E.td('description str', "A manually written description of the object"),
        E.td('params',
            E.table('params',
                E.tr('param',
                    E.td('offset hex', "Offs"),
                    E.td('datatype', "Type"),
                    E.td('description str', "Description"),
                ),
                E.tr('param end',
                    E.td('offset hex', "Offs"),
                    E.td('datatype', "end"),
                    E.td('example hex', "Example data"),
                ),
            ),
        ),
    );
}

function makeObjectsTable(objs) {
    const table = E.table(null, E.tr('header',
        E.th(null, "ID"),
        E.th(null, "Def"),
        E.th(null, "Name"),
        E.th(null, "DLL"),
        E.th(null, "Models"),
        E.th(null, "Maps"),
        E.th(null, "Description"),
        E.th(null, "Params"),
    ));
    table.append(makeExampleObject());
    //const iter = objs.evaluate('//object', objs, null, XPathResult.ORDERED_NODE_ITERATOR_TYPE);
    //for(const obj of iter) { //lol nope
    for(const obj of objs.getElementsByTagName('object')) {
        table.append(makeTableForObject(objs, obj));
    }
    document.body.append(table);
    finishLoading();
}

async function main() {
    const objs = await getObjects();
    gameTexts  = await getGameTexts();
    maps       = await getMaps();
    gameBits   = await getBits();
    modelInfo  = (await get({path:'models.xml', mimeType:'text/xml; charset=utf-8'})).responseXML;
    makeObjectsTable(objs);
}
setupMain(main);
