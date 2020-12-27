/** This script reads in some JSON/XML files containing data dumped from the game
 *  and produces a nice formatted table comparing DLLs across versions.
  */
import {E} from './Element.js';
import {get, getObjects, getDlls, finishLoading} from './Util.js';

const MAX_DLL_ID = 0x2C1;
const funcNames = [
    'initialise',   // called on DLL load
    'release',      // called on DLL unload
    '',             //  0: unused?
    'create',       //  1: create object instance; called 'Do'
    'update',       //  2: update object instance
    'hitDetect',    //  3: custom hit detection
    'render',       //  4: render object instance
    'free',         //  5: free object instance
    '',             //  6: ?
    'getExtraSize', //  7: get extra size for object, called "setzscale"
    'setScale',     //  8: set object instance scale, called 'rayhit_Do'
    '',             //  9: ?
    '',             // 10: ? something with matrices
    '',             // 11: ? something with children
    //possible the mismatched names are old versions?
    //there's nothing really enforcing that the functions have to be these,
    //so those could be old DLLs
];

function makeObjLink(obj) {
    const objId  = obj.getAttribute('id').substr(2);
    const objDef = obj.getAttribute('def').substr(2);
    return E.a(null, {href:"objects.html#"+obj.getAttribute('id')},
        objId + '/' + objDef + ' ' + obj.getAttribute('name'));
}

function makeObjList(dll, objs) {
    if(objs.length == 1) { //don't bother with a list
        return makeObjLink(objs[0]);
    }
    const ul = E.ul();
    for(const obj of objs) {
        ul.append(E.li('object', makeObjLink(obj)));
    }
    if(objs.length > 1) {
        //make a twisty expander holding the list
        return E.details(
            E.summary('count', objs.length.toString()),
            ul);
    }
    else return ul;
}

function makeFuncList(dll) {
    let funcs = dll.functions;
    if(funcs == undefined) return '';
    if(funcs.length == 0) return '';

    const tbl = E.table('funclist', E.tr(
        E.th(null, '#'),
        E.th(null, "Address"),
        E.th(null, "Stub"),
    ));
    let i = 0;
    for(let func of funcs) {
        tbl.append(E.tr('function',
            E.th('idx number', func.name),
            E.td('address hex ' + (func.valid ? '' : 'in') + 'valid',
                func.address.substr(2)),
            E.td('stub', func.stub),
        ));
        i++;
    }
    return E.details(E.summary('count', funcs.length.toString()), tbl);
}

function makeTableHeader() {
    const verNames = ['K', 'U0'];
    const nCols    = verNames.length.toString();
    return E.table(null, E.tr('header',
        E.th(null, "Name"),
        E.th('sep'),
        E.th(null, "ID",             {colspan:nCols}),
        E.th('sep'),
        //E.th(null, "Objs",           {colspan:nCols}),
        E.th(null, "Addr",           {colspan:nCols}),
        E.th('sep'),
        E.th(null, "Funcs",          {colspan:nCols}),
        E.th('sep'),
        E.th(null, "Non Stub Funcs", {colspan:nCols}),
        E.th('sep'),
        //E.th(null, "Unk08",          {colspan:nCols}),
        //E.th(null, "Unk0E",          {colspan:nCols}),
        //E.th(null, "Prev",           {colspan:nCols}),
        //E.th(null, "Next",           {colspan:nCols}),
        E.th(null, "Description"),
    ), E.tr('header',
        E.th(null, ""), //name
        E.th('sep'),
        E.th(null, verNames[0]), E.th(null, verNames[1]), //ID
        E.th('sep'),
        //E.th(null, verNames[0]), E.th(null, verNames[1]), E.th(null, verNames[2]), //objs
        E.th(null, verNames[0]), E.th(null, verNames[1]), //addr
        E.th('sep'),
        E.th(null, verNames[0]), E.th(null, verNames[1]), //funcs
        E.th('sep'),
        E.th(null, verNames[0]), E.th(null, verNames[1]), //nonstub
        E.th('sep'),
        //E.th(null, verNames[0]), E.th(null, verNames[1]), E.th(null, verNames[2]), //Unk08
        //E.th(null, verNames[0]), E.th(null, verNames[1]), E.th(null, verNames[2]), //Unk0E
        //E.th(null, verNames[0]), E.th(null, verNames[1]), E.th(null, verNames[2]), //Prev
        //E.th(null, verNames[0]), E.th(null, verNames[1]), E.th(null, verNames[2]), //Next
        E.th(null, "") //descr
    ));
}

function makeDllsTable(dlls, objs) {
    const table = makeTableHeader();

    //make list of objects that use each DLL
    const dllObjs = {};
    for(const obj of objs.getElementsByTagName('object')) {
        let dll = obj.getAttribute('dll');
        if(dll == null) continue;
        dll = parseInt(dll);
        if(dllObjs[dll] == undefined) dllObjs[dll] = [];
        dllObjs[dll].push(obj);
    }

    //make list of names
    const names = Object.keys(dlls);
    names.sort((a, b) => a.toLowerCase().localeCompare(b.toLowerCase()));

    //display the list
    const versions = ['kiosk', '1.00_us'];
    for(let name of names) {
        const dll_U0 = dlls[name]['1.00_us'];
        const cells = {name: E.th('name str', name)};
        for(let version of versions) {
            let dll = dlls[name][version];
            if(dll == undefined) dll = {
                id:          '',
                address:     '',
                dolOffs:     '',
                unk08:       '',
                unk0E:       '',
                functions:   '',
                nonStubs:    '',
                description: '',
            };
            let   cAddr = 'address hex';
            const addr  = parseInt(dll.address);
            const offs  = parseInt(dll.dolOffs);
            if(isNaN(addr) || addr < 0x80000000 || addr > 0x81800000 || isNaN(offs)) {
                cAddr += ' invalid';
            }
            const c = {
                id:          E.td('id hex', dll.id.substr(2)),
                address:     E.td(cAddr, dll.address.substr(2)),
                //unk08:       E.td('hex', dll.unk08),
                //unk0E:       E.td('hex', dll.unk0E),
                nonStubs:    E.td('number', dll.nonStubs.toString()),
                functions:   E.td('funclist', makeFuncList(dll)),
                description: E.td('description str', dll.description),
            };
            if(dll_U0) {
                if(dll.id != dll_U0.id) c.id.classList.add('different');
                if(dll.nonStubs != dll_U0.nonStubs) c.nonStubs.classList.add('different');
            }
            cells[version] = c;
        }
        const tr = E.tr('dll', cells.name, E.td('sep'));
        for(let v of versions) tr.append(cells[v].id); tr.append(E.td('sep'));
        for(let v of versions) tr.append(cells[v].address); tr.append(E.td('sep'));
        for(let v of versions) tr.append(cells[v].functions); tr.append(E.td('sep'));
        for(let v of versions) tr.append(cells[v].nonStubs); tr.append(E.td('sep'));
        tr.append(cells['1.00_us'].description);
        table.append(tr);
    }

    document.body.append(table);
    finishLoading();
}

function buildDllList(versions) {
    /** Build a list of name => DLL. (Use ID when no name.)
     */
    const result = {};
    for(let version of Object.keys(versions)) {
        const dlls = versions[version];
        for(let i=0; i<=MAX_DLL_ID; i++) {
            const id  = '0x'+(i.toString(16).toUpperCase().padStart(4, '0'));
            const dll = dlls.getElementById(id);
            let name  = dll.getAttribute('name');
            if(name == null) name = id;
            //if name wasn't seen yet, create the entry.
            if(result[name] == undefined) result[name] = {};
            //populate the entry for this version.
            result[name][version] = {
                id:        id,
                address:   dll.getAttribute('address'),
                dolOffs:   dll.getAttribute('dolOffs'),
                unk08:     dll.getAttribute('unk08'),
                unk0E:     dll.getAttribute('unk0E'),
                functions: [],
            };

            let descr = dll.getElementsByTagName('description');
            if(descr && descr[0]) descr = descr[0].textContent.trim();
            if(descr) result[name][version].description = descr;

            //make function list and count non-stubs.
            let nonStubs = 0;
            let funcs = dll.getElementsByTagName('functions')[0];
            if(funcs == undefined) funcs = [];
            else funcs = funcs.getElementsByTagName('function');

            let iFunc = 0;
            for(let func of funcs) {
                let stub = '';
                if(func.getAttribute('stub') == '1') {
                    const ret = func.getAttribute('return');
                    if(ret != undefined) stub = ret.toString();
                    else stub = '✓';
                }
                const invalid = (func.getAttribute('valid') == '0');
                let funcName = func.getAttribute('name');
                if(!funcName) funcName = funcNames[iFunc];
                if(!funcName) funcName = iFunc.toString()

                result[name][version].functions.push({
                    name:    funcName,
                    address: func.getAttribute('address'),
                    valid:   !invalid,
                    stub:    stub,
                });
                if(!(stub || invalid)) nonStubs++;
                iFunc++;
            }
            result[name][version].nonStubs = nonStubs;
        }
    }
    return result;
}

async function main() {
    const objs = await getObjects('U0');
    const dlls = {
        '1.00_us': await getDlls('U0'),
        //'1.01_us': await getDlls('U1'),
        'kiosk':   await getDlls('kiosk'),
    };
    makeDllsTable(buildDllList(dlls), objs);
}

if(document.readyState === "complete"
|| document.readyState === "interactive") {
    setTimeout(main, 1); //call on next available tick
} else {
    document.addEventListener("DOMContentLoaded", main);
}
