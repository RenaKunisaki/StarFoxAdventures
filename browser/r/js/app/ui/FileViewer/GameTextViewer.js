import { E, clearElement } from "../../../lib/Element.js";
import { addReverseMap, hex } from "../../../Util.js";
import Table from "../Table.js";
import GameTextFile from "../../../types/GameText.js";

const Languages = addReverseMap({
    English:  0,
    French:   1,
    German:   2,
    Italian:  3,
    Japanese: 4,
    Spanish:  5,
});
const Justify = addReverseMap({
    Left:   0,
    Right:  1,
    Center: 2,
    Full:   3,
})

class GameTextRenderer {
    constructor(app) {
        this.app = app;
    }

    render(text) {
        this._lang       = Languages[text.lang];
        this._curFontIdx = (text.lang == Languages.Japanese) ? 0 : 4;
        this._color      = [255, 255, 255, 255];
        this._justify    = Justify[Justify.Left]; //get the name
        this._scale      = 1.0;

        const list = E.ul('phrases');
        for(let phrase of text.phrases) {
            list.append(E.li('phrase',
                this._makeElemForPhrase(phrase)));
        }
        return list;
    }

    _makeNewElem() {
        const cls = ['gametext',
            `gametext-${this._lang}`,
            `justify-${this._justify}`,
            `font${this._curFontIdx}`,
        ];
        const elem = E.span(cls.join(' '));

        const c = this._color;
        elem.style.color = `rgba(${c[0]}, ${c[1]}, ${c[2]}, ${c[3]/255})`;
        elem.style.fontSize = `${this._scale*100}%`;
        return elem;
    }

    _makeElemForPhrase(phrase) {
        let elem = this._makeNewElem();
        let result = [elem];
        for(let str of phrase.str) {
            let startNew = false;
            if(typeof(str) == 'string') elem.append(str);
            else switch(str.cmd) {
                case 'seq':
                    elem.append(E.span('command', `[SEQ ${str.id}]`));
                    break;
                case 'time':
                    elem.append(E.span('command', `[TIME ${str.unk1}, ${str.time}, ${str.unk3}]`));
                    break;
                case 'hint':
                    elem.append(E.span('command', `[HINT ${str.id}]`));
                    break;
                case 'scale': {
                    //elem.append(E.span('command', `[SCALE ${str.scale}]`));
                    this._scale = parseInt(str.scale) / 256;
                    startNew = true;
                    break;
                }
                case 'font': {
                    //elem.append(E.span('command', `[FONT ${str.id}]`));
                    this._curFontIdx = parseInt(str.id);
                    startNew = true;
                    break;
                }
                case 'justifyLeft':
                    this._justify = Justify[Justify.Left];
                    startNew = true;
                    break;
                case 'justifyRight':
                    this._justify = Justify[Justify.Right];
                    startNew = true;
                    break;
                case 'justifyCenter':
                    this._justify = Justify[Justify.Center];
                    startNew = true;
                    break;
                case 'justifyFull':
                    this._justify = Justify[Justify.Full];
                    startNew = true;
                    break;
                case 'color': {
                    //elem.append(E.span('command', `[COLOR ${str.r},${str.g},${str.b},${str.a}]`));
                    this._color = [str.r, str.g, str.b, str.a];
                    startNew = true;
                    break;
                }
                case 'unkF8F2':
                case 'unkF8F3':
                    elem.append(E.span('command', `[${str.cmd} ${str.unk1}, ${str.unk2}]`));
                    break;
                case 'unkF8F5':
                case 'unkF8F6':
                    elem.append(E.span('command', `[${str.cmd} ${str.unk1}]`));
                    break;
                //case 'unkF8FC':
                //case 'unkF8FD':
                //case 'unkF8FE':
                //    elem.append(E.span('command', `[${str.cmd}]`));
                //    break;
                default:
                    elem.append(E.span('command', str.cmd));
                    break;
            } //switch
            if(startNew) {
                elem = this._makeNewElem();
                result.push(elem);
                startNew = false;
            }
        } //for
        return E.span('gametext', ...result);
    }
}

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
        this.table = new Table({columns: [
            {displayName:"#", name:'id',   type:'hex', length:4},
            {displayName:"W", name:'window',type:'hex', length:2,
                title:"Window type"},
            {displayName:"H", name:'alignH', type:'int', title:"Align H"},
            {displayName:"V", name:'alignV', type:'int', title:"Align V"},
            {displayName:"Text", name:'phrases',  type:'string',
                makeElem: (phrases, td, row) => {
                    const text = (new GameTextRenderer(this.app)).render(row);
                    clearElement(td).append(text);
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
