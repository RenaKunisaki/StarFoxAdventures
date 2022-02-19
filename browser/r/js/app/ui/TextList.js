import { E, clearElement } from "../../lib/Element.js";
import { hex } from "../../Util.js";
import Table from "./Table.js";
import { TEXT_LANGUAGES } from "../../game/game.js";
import { GameTextRenderer } from "./FileViewer/GameTextViewer.js";

export default class TextList {
    /** Displays all GameText definitions from XML.
     */
    constructor(app) {
        this.app = app;
        this.element = document.getElementById('tab-textList');
        this.eLang = E.select('language');
        for(let lang of TEXT_LANGUAGES) {
            this.eLang.append(E.option(null, lang, {value:lang}));
        }
        this.eLang.addEventListener('change', e => this.refresh());
        this.element.append(this.eLang);
        this.app.onIsoLoaded(iso => this.refresh());
        this.renderer = new GameTextRenderer(this.app);
    } //constructor

    refresh() {
        this.lang = this.eLang.value;
        let tbl = this._makeTable();
        for(let text of Object.values(this.app.game.texts)) {
            tbl.add(this._makeRow(text));
        }
        const elem = E.div('textList', tbl.element);
        clearElement(this.element).append(this.eLang, elem);
    }

    _makeTable() {
        return new Table({columns: [
            {displayName:"#", name:'id',   type:'hex', length:4},
            {displayName:"W", name:'window',type:'hex', length:2,
                title:"Window type"},
            {displayName:"H", name:'alignH', type:'int', title:"Align H"},
            {displayName:"V", name:'alignV', type:'int', title:"Align V"},
            {displayName:"Text", name:'phrases',  type:'string',
                makeElem: (phrases, td, row) => {
                    let list;
                    let p = phrases[this.lang];
                    if(p) list = this.renderer.render(p, this.lang);
                    else list = E.span('error', "no text");
                    clearElement(td).append(list);
                    return td;
                },
            },
        ]});
    }

    _makeRow(text) {
        const row = {
            id:      text.id,
            phrases: text.phrases,
            window:  text.window,
            alignH:  text.alignH,
            alignV:  text.alignV,
        };
        return row;
    }
}
