import { E, clearElement } from "../../../lib/Element.js";
import { hex } from "../../../Util.js";
import Table from "../Table.js";
import GameTextFile from "../../../types/GameText.js";

export class GameTextViewer {
    //displays GameText file contents
    constructor(app, dataView) {
        this.app     = app;
        this.view    = dataView;
        this.element = E.div('gametext');
        this._makeTable();
        this.refresh();
    }

    _makeElemForPhrase(phrase) {
        let result = E.span('gametext');
        for(let str of phrase.str) {
            switch(str[0]) {
                case 'str': result.append(str[1]); break;
                case 'seq':
                    result.append(E.span('command', `[SEQ ${str[1]}]`));
                    break;
                case 'time':
                    result.append(E.span('command', `[TIME ${str[1]}, ${str[2]}, ${str[3]}]`));
                    break;
                case 'hint':
                    result.append(E.span('command', `[HINT ${str[1]}]`));
                    break;
                case 'scale':
                    result.append(E.span('command', `[SCALE ${str[1]}]`));
                    break;
                case 'font':
                    result.append(E.span('command', `[FONT ${str[1]}]`));
                    break;
                case 'justify':
                    result.append(E.span('command', `[JUST ${str[1]}]`));
                    break;
                case 'color':
                    result.append(E.span('command', `[COLOR ${str[1]}, ${str[2]}]`));
                    break;
                default:
                    result.append(E.span('command', `[${str}]`));
                    break;
            }
        }
        return result;
    }

    _makeTable() {
        this.table = new Table({columns: [
            {displayName:"#", name:'id',   type:'hex', length:4},
            {displayName:"W", name:'window',type:'hex', length:2,
                title:"Window type"},
            {displayName:"H", name:'alignH', type:'int', title:"Align H"},
            {displayName:"V", name:'alignV', type:'int', title:"Align V"},
            {displayName:"Text", name:'phrases',  type:'string',
                makeElem: (phrases, td, row) => {
                    let list = E.ul('phrases');
                    for(let phrase of phrases) {
                        list.append(E.li('phrase',
                            this._makeElemForPhrase(phrase)));
                    }
                    clearElement(td).append(list);
                    return td;
                },
            },
        ]});
    }

    refresh() {
        this.textFile = new GameTextFile(this.app, this.view);
        for(let text of this.textFile.texts) {
            this.table.add(this._makeRow(text));
        }
        clearElement(this.element).append(this.table.element);
    }

    _makeRow(text) {
        const row = {
            id:      text.id,
            lang:    text.language,
            phrases: text.phrases,
            window:  text.window,
            alignH:  text.alignH,
            alignV:  text.alignV,
        };
        return row;
    }
}
