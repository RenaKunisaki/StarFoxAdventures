import {E} from '/js/Element.js';

export default class ModelDataView {
    /** Widget for exploring the data in a model, or really any object.
     *  Not currently used.
     */
    constructor(object) {
        this.obj = object;
        this.element = E.div('dataView');
    }

    refresh() {
        const tbl = E.table(E.tr(
            E.th(null, "Field"),
            E.th(null, "Type"),
            E.th(null, "Value"),
        ));
        for(let [name, val] of Object.entries(this.obj)) {
            tbl.append(E.tr(
                E.td(null, name),
                E.td(null, typeof val),
                E.td(null, String(val)),
            ));
        }
        this.element.innerHtml = '';
        this.element.append(tbl);
    }
}
