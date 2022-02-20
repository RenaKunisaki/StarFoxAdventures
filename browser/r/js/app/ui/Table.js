import { E, clearElement } from "../../lib/Element.js";

let _nextId = 0;
export default class Table {
    /** A table widget that the user can sort and filter.
     */
    constructor(params) {
        let cls = params.cls;
        if(cls == undefined || cls == null) cls = '';
        let title = params.title;
        if(title == undefined || title == null) title = '';
        this.columns        = params.columns;
        this._eControls     = E.div('controls', E.h1(null, title));
        this._eTable        = E.table();
        this._rows          = [];
        this._sortedBy      = null;
        this._sortedReverse = false;
        this._colHeaders    = {}; //col name => th
        this._colElems      = {}; //col name => [td, td...]
        this.element = E.div(`${cls} table`, this._eControls, this._eTable);
        for(let col of this.columns) this._colElems[col.name] = [];
        this._buildColumnPicker();
        this._buildColumns();
    }

    add(row) {
        this._rows.push(row);
        this._eTable.append(this._buildRow(row));
    }

    sort(col, reverse=false) {
        if(col.sortFunc) this._rows.sort(col.sortFunc);
        else if(col.compareFunc) {
            this._rows.sort((a, b) => col.compareFunc(a, b));
        }
        else {
            const k    = col.name;
            const dflt = col.defaultVal;
            let   key, sort;
            switch(col.type) {
                //this can probably be less repetitive, but introducing more
                //function calls inside the sort function would kill performance.
                case 'string':
                    key = (row) => {
                        let v = row[k];
                        if(v == undefined) v = dflt;
                        if(v == undefined || v == null) v = '';
                        return String(v).toLowerCase();
                    }
                    sort = (a,b) => key(a).localeCompare(key(b));
                    break;

                case 'int':
                case 'hex':
                    key = (row) => {
                        let v = row[k];
                        if(v == undefined) v = dflt;
                        v = parseInt(v);
                        if(isNaN(v)) v = 0;
                        return v;
                    }
                    sort = (a,b) => key(a) - key(b);
                    break;

                case 'float':
                    key = (row) => {
                        let v = row[k];
                        if(v == undefined) v = dflt;
                        return parseFloat(v);
                    }
                    sort = (a,b) => key(a) - key(b);
                    break;

                default:
                    key = (row) => {
                        let v = row[k];
                        if(v == undefined) v = dflt;
                        return v;
                    }
                    sort = (a,b) => key(a) - key(b);
                    break;
            }
            this._rows.sort(sort);
        }
        if(reverse) this._rows.reverse();
        this._rebuildContent();
    }

    _buildColumns() {
        const tr = E.tr('header');
        this._header = tr;
        for(let i=0; i<this.columns.length; i++) {
            const col  = this.columns[i];
            let   name = col.displayName;
            if(name == undefined) name = col.name;
            const th = E.th(col.cls, name);
            th.addEventListener('click', e => {
                if(this._sortedBy == col) {
                    this._sortedReverse = !this._sortedReverse;
                }
                else {
                    this._sortedBy = col;
                    this._sortedReverse = false;
                }
                this.sort(col, this._sortedReverse);
            });
            if(col.title) th.setAttribute('title', col.title);
            tr.append(th);
            this._colHeaders[col.name] = th;
        }
        this._eTable.append(tr);
    }

    _rebuildContent() { //called by sort()
        const elems = [];
        for(let row of this._rows) {
            elems.push(this._buildRow(row));
        }
        clearElement(this._eTable);
        this._eTable.append(this._header);
        for(let elem of elems) this._eTable.append(elem);
        console.log("_rebuildContent done");
    }

    _buildRow(row) {
        const tr = E.tr(row.cls);
        for(let i=0; i<this.columns.length; i++) {
            const col  = this.columns[i];
            let   val  = row[col.name];
            let   num  = null;
            if(col.format) val = col.format(val, col, row);
            else switch(col.type) {
                case 'int': {
                    if(val != null && val != undefined) {
                        num = parseInt(val);
                        //if(isNaN(num)) num = parseInt('0x'+val);
                        val = val.toString();
                    }
                    break;
                }
                case 'hex': {
                    num = parseInt(val);
                    if(isNaN(num)) val = '';
                    else {
                        val = val.toString(16);
                        if(val.startsWith('0x')) val = val.substr(2);
                        if(!col.lowercase) val = val.toUpperCase();
                        if(col.length != undefined) val = val.padStart(col.length, '0');
                        //XXX this won't work with 0x
                    }
                    break;
                }
                case 'float': {
                    num = parseFloat(val);
                    let decimals = col.decimals;
                    if(decimals == undefined) val = val.toString();
                    else val = val.toFixed(decimals);
                }
            }
            if(val == null || val == undefined) val = '';
            let td = E.td(col.type, val);
            if(col.makeElem) td = col.makeElem(val, td, row);
            if(num !== null) {
                if(num == 0) td.classList.add('zero');
                else if(num < 0) td.classList.add('negative');
                else if(isNaN(num)) td.classList.add('NaN');
                else if(!isFinite(num)) td.classList.add('infinite');
            }
            if(val == undefined || val == null || val == '') {
                td.classList.add('empty');
            }
            if(col.classes) td.classList.add(col.classes);
            if(col.onEdit) {
                td.setAttribute('contenteditable', 'true');
                td.addEventListener('input', e => col.onEdit(row, col, e, td));
            }
            tr.append(td);
            this._colElems[col.name].push(td);
        }
        return tr;
    }

    _toggleColumn(name) {
        const show = (this._colHeaders[name].style.display == 'none');
        this._colHeaders[name].style.display = show ? '' : 'none';
        for(let elem of this._colElems[name]) {
            elem.style.display = show ? '' : 'none';
        }
    }

    _buildColumnPicker() {
        this._colPicker = E.ul('columnPicker');
        for(let col of this.columns) {
            let name = col.displayName;
            if(name == undefined) name = col.name;
            let check = E.input({
                type:   'checkbox',
                id:     `check${_nextId}`,
                checked:'checked',
            });
            let label = E.label(null, {
                for:   `check${_nextId}`,
            }, name);
            check.addEventListener('click', e => this._toggleColumn(col.name));
            this._colPicker.append(E.li('column', check, label));
        }
        this._colPicker.style.display = 'none';

        this._colPickerButton = E.button(null, "Cols");
        this._colPickerButton.addEventListener('click', e => {
            if(this._colPicker.style.display == 'none') {
                this._colPicker.style.display = 'block';
            }
            else this._colPicker.style.display = 'none';
        });

        this._eControls.append(E.div(null, this._colPickerButton, this._colPicker));
    }
}
