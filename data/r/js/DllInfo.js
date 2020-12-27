import {E} from './Element.js';
import Table from './Table.js';
import {get, hex, int, makeList, makeCollapsibleList, makeDescriptionAndNotes} from './Util.js';

export default class DllInfo {
    /** A page showing details of a DLL.
     */
    constructor(app) {
        this.app = app;
        this.element = E.div();
        const id = this.app.params.get('id');
        this.dll = this.app.game.dlls[id];
    }

    getTitle() {
        const name = this.dll.name ? this.dll.name : `DLL ${hex(this.dll.id,4)}`;
        return `${name} - DLL Info`;
    }

    async run() {
        this.element.append(this.makeSummaryTable());
        await this.app.game.getFuncParamRecords();
        this.element.append(this.makeObjList());
        this.element.append(this.makeFuncList());
    }

    makeSummaryTable() {
        const dll = this.dll;
        let name = this.dll.name;
        if(name == null) name = E.span('invalid', 'none');

        let iface = this.dll.interface;
        if(iface == null) iface = '-';

        return E.div('dll-summary',
            E.h2('header', "Summary"),
            E.table(
                E.tr(E.th(null, "Name"),        E.td('str', name)),
                E.tr(E.th(null, "Address"),     E.td('hex', hex(dll.address,8))),
                E.tr(E.th(null, "DOL Offset"),  E.td('hex', hex(dll.dolOffs,6))),
                E.tr(E.th(null, "Unk 08"),      E.td('hex', hex(dll.unk08,  8))),
                E.tr(E.th(null, "Unk 0E"),      E.td('hex', hex(dll.unk0E,  8))),
                E.tr(E.th(null, "Interface"),   E.td('str', iface)),
                E.tr(E.th(null, "Description"), E.td('str', makeDescriptionAndNotes(dll))),
            )
        );
    }

    makeObjLink(obj) {
        const v = this.app.game.version;
        return E.a(null, {href:`?v=${v}&p=obj&id=${obj.id}`},
            `${hex(obj.id,4)}/${hex(obj.defNo,4)} ${obj.name}`);
    }

    makeObjList() {
        const ul = E.ul();
        let count = 0;
        for(let [id, obj] of Object.entries(this.app.game.objects)) {
            if(obj.dllId == this.dll.id) {
                ul.append(E.li('object', this.makeObjLink(obj)));
                count++;
            }
        }
        return E.div('box dll-objlist', E.h2(null, `Loaded by ${count} objects`), ul);
    }

    makeFuncList() {
        const dll = this.dll;

        const tr = E.tr();
        for(let i=3; i<13; i++) tr.append(E.th(null, `r${i}`));

        const tbl = E.table('funclist', tr);
        for(let func of dll.functions) {
            if(!func.isValid) continue;

            let funcName = func.name;
            if(funcName == null) funcName = `Func 0x${hex(func.idx,2)}`;
            const addr = hex(func.address, 8);

            //make param name row
            const pNameRow = E.tr('param-names');
            let nParams=0;
            for(let param of func.params) {
                const reg = param.reg;
                if(reg != null && reg.startsWith('r')) {
                    let name = param.name;
                    if(name == null) name = '';
                    pNameRow.append(E.th('param-name name', name));
                    nParams++;
                }
            }
            while(nParams < 10) {
                pNameRow.append(E.th('param-name name', '-'));
                nParams++;
            }

            const entry  = this.makeFuncParamList(func);
            const params = entry.params;
            const count  = entry.count;
            if(params.length == 0) {
                tbl.append(E.tr('function',
                    E.th(null, `${addr} ${funcName} - Called ${count} times - `,
                        E.span('error', "NO RECORDS"),
                        {colspan:10}),
                ));
            }
            else {
                if(func.knownParams) {
                    tbl.append(E.tr('function',
                        E.th(null, `${addr} ${funcName} - Called ${count} times`, {colspan:10}),
                    ));
                    tbl.append(pNameRow);
                }
                else {
                    tbl.append(E.tr('function',
                        E.th(null, `${addr} ${funcName} - Called ${count} times - `,
                            E.span('warning', "Params unknown"),
                            {colspan:10}),
                    ));
                }
                for(let param of params) tbl.append(param);
            }
        }
        return E.div('box dll-funclist',
            E.h2('header', "Function Call Records"),
            tbl);
    }

    makeFuncParamList(func) {
        const addr   = hex(func.address, 8);
        const params = this.app.game.funcParams[func.address];
        if(!params) {
            console.log("No record found for address", addr, func);
            return {params:[], count:0};
        }
        const allocTags   = this.app.game.allocTags;
        const staticAddrs = this.app.game.staticAddrs;

        const rows = [];
        for(let record of params.records) {
            const row = E.tr();
            rows.push(row);
            for(let param of record) {
                let tag     = param.tag;
                let tHex    = hex(param.tag, 8);
                let val     = param.val;
                let vHex    = hex(val, 8);
                let offs    = param.offset;
                let name    = allocTags[tag];
                if(name) { //this alloc tag is known
                    if(offs != 0) {
                        const sign = (offs < 0) ? '-' : '+';
                        offs = Math.abs(offs).toString(16).toUpperCase().padStart(4, '0');
                        row.append(E.td(`allocTag allocTag-${name}`,
                            `${name}${sign}${offs}`, {title:`Tag ${tHex} val ${vHex}`}
                        ));
                    }
                    else row.append(E.td(`allocTag allocTag-${name}`,
                        name, {title:`Tag ${tHex} val ${vHex}`}
                    ));
                }
                else { //no known alloc tag for this value
                    if(val == 0) row.append(E.td('allocTag null', "null"));
                    else { //maybe it's a static address?
                        const staticAddr = staticAddrs[val];
                        if(staticAddr == null) row.append(E.td('allocTag null hex', vHex));
                        else {
                            const name = staticAddr.name;
                            row.append(E.td('allocTag staticAddr hex',
                                staticAddr.type,
                                {title: `${name} @ ${vHex}`}
                            ))
                        }
                    }
                }
            }
        }
        return {params:rows, count:params.count};
    }
}
