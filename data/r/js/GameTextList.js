import {E} from './Element.js';
import Table from './Table.js';
import {hex, makeCollapsibleList} from './Util.js';

export default class GameTextList {
    /** A page listing the texts in the game.
     */
    constructor(app) {
        this.app = app;
        this._makeTable();
        this.element = this.tbl.element;
    }

    getTitle() { return "GameText" }

    async run() {
        for(let [id, text] of Object.entries(this.app.game.texts)) {
            this.tbl.add(this._makeRow(text));
        }
    }

    _makeTable() {
        const v = this.app.game.version;
        this.tbl = new Table({
            columns: [
                {
                    displayName: "#",
                    name: 'id',
                    type: 'hex',
                    length: 4,
                }, {
                    displayName: "W",
                    name: 'window',
                    type: 'hex',
                    length: 2,
                }, {
                    displayName: "H",
                    name: 'alignH',
                    type: 'int',
                }, {
                    displayName: "V",
                    name: 'alignV',
                    type: 'int',
                }, {
                    displayName: "L",
                    name: 'langId',
                    type: 'int',
                }, {
                    displayName: "Dirs",
                    name: 'dirs',
                    type: 'string',
                    compareFunc: (a, b) => a.nDirs - b.nDirs,
                }, {
                    displayName: "Text",
                    name: 'phrases',
                    type: 'string',
                },
            ]
        });
    }

    _makeRow(text) {
        const phrases = E.ul();
        for(let phrase of text.phrases) {
            phrases.append(E.li(null, phrase));
        }
        return {
            id:      text.id,
            window:  text.window,
            alignH:  text.alignH,
            alignV:  text.alignV,
            langId:  text.langId,
            dirs:    makeCollapsibleList(text.dirs),
            nDirs:   text.dirs.length, //for sorting
            phrases: phrases,
        };
    }
}
