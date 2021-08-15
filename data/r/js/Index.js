import {E} from './Element.js';
import Game from './Game.js';

export default class Index {
    /** The index page.
     */
    constructor(app) {
        let tbl = E.table();
        this.element = tbl;

        for(let [id, name] of Object.entries(Game.getVersions())) {
            let tr = E.tr(
                E.th(null, name),
                E.td(E.a(null, "DLLs",     {href:`?v=${id}&p=dlls`})),
                E.td(E.a(null, "GameBits", {href:`?v=${id}&p=gamebits`})),
                E.td(E.a(null, "GameText", {href:`?v=${id}&p=gametext`})),
                E.td(E.a(null, "Fonts",    {href:`?v=${id}&p=fonts`})),
                E.td(E.a(null, "Maps",     {href:`?v=${id}&p=maps`})),
                E.td(E.a(null, "Objects",  {href:`?v=${id}&p=objs`})),
            );
            tbl.append(tr);
        }
    }

    getTitle() { return "Index" }

    async run() {
        //nothing to do
    }
}
