/** This script reads in some JSON/XML files containing data dumped from the game
 *  and produces a nice formatted table of which DLLs are used by which objects.
  */
import {E} from './Element.js';
import {get, getObjects, getDLLs, finishLoading} from './Util.js';

function makeDllsTable(dlls, objs) {
    const table = E.table(null, E.tr('header',
        E.th(null, "ID"),
        E.th(null, "Name"),
        E.th(null, "Objs"),
        E.th(null, "Description"),
    ));

    const rows = {};
    for(const obj of objs.getElementsByTagName('object')) {
        let dll = obj.getAttribute('dll');
        if(dll == null) continue;
        dll = parseInt(dll);
        if(rows[dll] == undefined) rows[dll] = [];
        rows[dll].push(obj);
    }

    for(let i=0; i<0x2C2; i++) {
        const id = '0x' + (i.toString(16).toUpperCase());
        const dllId = '0x'+(i.toString(16).toUpperCase().padStart(4, '0'));
        const dispId = i.toString(16).toUpperCase().padStart(3, '0');

        let dll = dlls.getElementById(dllId);
        const ul = E.ul();

        if(rows[i]) {
            for(const obj of rows[i]) {
                const objId  = obj.getAttribute('id').substr(2);
                const objDef = obj.getAttribute('def').substr(2);
                ul.append(E.li('object',
                    E.a(null, {href:"objects.html#"+obj.getAttribute('id')},
                    objId + '/' + objDef + ' ' + obj.getAttribute('name')),
                ));
            }
        }
        let name = dll.getAttribute('name');
        if(name == null) name = '';
        table.append(E.tr('dll', {id:id},
            E.th('id hex', dispId),
            E.td('name str', name),
            E.td('objlist', ul),
            E.td('description', dll.textContent),
        ));
    }

    document.body.append(table);
    finishLoading();
}

async function main() {
    const objs = await getObjects();
    const dlls = await getDLLs();
    makeDllsTable(dlls, objs);
}

if(document.readyState === "complete"
|| document.readyState === "interactive") {
    setTimeout(main, 1); //call on next available tick
} else {
    document.addEventListener("DOMContentLoaded", main);
}
