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
        let elem = E.span();
        let result = [elem];
        for(let str of phrase.str) {
            switch(str[0]) {
                case 'str': elem.append(str[1]); break;
                case 'seq':
                    elem.append(E.span('command', `[SEQ ${str[1]}]`));
                    break;
                case 'time':
                    elem.append(E.span('command', `[TIME ${str[1]}, ${str[2]}, ${str[3]}]`));
                    break;
                case 'hint':
                    elem.append(E.span('command', `[HINT ${str[1]}]`));
                    break;
                case 'scale': {
                    //elem.append(E.span('command', `[SCALE ${str[1]}]`));
                    let scale = parseInt(str[1]) / 256;
                    elem = E.span(null, {style:`font-size: ${scale*100}%`});
                    result.push(elem);
                    break;
                }
                case 'font': {
                    let font = parseInt(str[1]);
                    elem = E.span(`font${font}`);
                    result.push(elem);
                    break;
                }
                case 'justify':
                    elem.append(E.span('command', `[JUST ${str[1]}]`));
                    break;
                case 'color': {
                    //elem.append(E.span('command', `[COLOR ${str[1]}, ${str[2]}]`));
                    let fg = str[1], bg = str[2];
                    fg = `rgba(${fg[0]}, ${fg[1]}, ${fg[2]}, ${fg[3]/255})`;
                    let style = `color: ${fg};`;
                    if(bg) {
                        bg = `rgba(${bg[0]}, ${bg[1]}, ${bg[2]}, ${bg[3]/255})`;
                        style += `text-shadow:
                            -1px -1px 0 ${bg},
                             1px -1px 0 ${bg},
                             1px  1px 0 ${bg},
                            -1px  1px 0 ${bg};`;
                    }
                    elem = E.span(null, {style:style});
                    result.push(elem);
                    break;
                }
                default:
                    elem.append(E.span('command', `[${str}]`));
                    break;
            }
        }
        return E.span('gametext', ...result);
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
