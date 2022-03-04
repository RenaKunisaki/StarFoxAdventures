import { E, clearElement } from "../../../lib/Element.js";
import Table from "../Table.js";
import BinaryReader from "../../../game/text/BinaryReader.js";
import { Language } from "../../../game/text/Language.js";
import GameTextRenderer from "../../../game/text/Renderer.js";

export class GameTextViewer {
    //displays GameText file contents
    constructor(app, dataView) {
        this.app     = app;
        this.view    = dataView;
        this.element = E.div('gametext');
        this._makeTable();
        this.refresh();
    }

    _makeTable() {
        this.table = new Table({title:"GameText Data", columns: [
            {displayName:"#", name:'id',   type:'hex', length:4},
            {displayName:"W", name:'window',type:'hex', length:2,
                title:"Window type"},
            {displayName:"H", name:'alignH', type:'int', title:"Align H"},
            {displayName:"V", name:'alignV', type:'int', title:"Align V"},
            {displayName:"Text", name:'phrases',  type:'string',
                makeElem: (phrases, td, row) => {
                    const text = (new GameTextRenderer(this.app)).render(row.text);
                    clearElement(td).append(text);
                    return td;
                },
            },
        ]});
    }

    refresh() {
        this.textFile = new BinaryReader(this.app, this.view);
        for(let text of this.textFile.texts) {
            this.table.add(this._makeRow(text));
        }
        clearElement(this.element).append(this.table.element);
    }

    _makeRow(text) {
        const row = {
            text:    text,
            id:      text.id,
            lang:    text.language,
            phrases: text.phrases,
            window:  text.window,
            alignH:  text.align[0],
            alignV:  text.align[1],
        };
        return row;
    }
}
