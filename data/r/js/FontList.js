import {E} from './Element.js';
import Table from './Table.js';
import {hex, makeCollapsibleList} from './Util.js';
import GameTextBin from './GameTextBin.js';
import HexView from './HexView.js';

const languages = [
    "🇬🇧", //English
    "🇫🇷", //French
    "🇩🇪", //German
    "🇮🇹", //Italian
    "🇯🇵", //Japanese
    "🇪🇸" //Spanish
];

export default class FontList {
    /** A page listing the texts in the game.
     */
    constructor(app) {
        this.app = app;
        this._makeElement();
    }

    getTitle() { return "Fonts" }

    async run() {
        this.font = new GameTextBin(this.app, "?");
        await this.font.load();
        for(let i=0; i<this.font.texts.length; i++) {
            let text = this.font.texts[i];
            console.log("Phrases", text.phrases);
            this.eTexts.append(E.tr('text',
                E.td('number', i),
                E.td('hex', hex(text.identifier)),
                //E.td('number', text.numPhrases),
                E.td('number', text.window),
                E.td('number', text.alignH),
                E.td('number', text.alignV),
                E.td('str', languages[text.language]),
                E.td(null, makeCollapsibleList(text.phrases)),
            ));
        }
        this.unkDataView.data = new DataView(this.font.unkData);
    }

    _makeElement() {
        this.unkDataView = new HexView(this.app);
        this.eTexts = E.table(E.tr(
            E.th(null, "#"),
            E.th(null, "ID"),
            //E.th(null, "#Phrases"),
            E.th(null, "Win"),
            E.th(null, "AlignH"),
            E.th(null, "AlignV"),
            E.th(null, "Lang"),
            E.th(null, "Phrases"),
        ));
        this.element = E.div(null,
            E.div('box texts',
                E.h2('header', "Texts"),
                this.eTexts,
            ),
            E.div('box unknown-data',
                E.h2('header', "Unknown"),
                this.unkDataView.element,
            ),
        );
    }
}
