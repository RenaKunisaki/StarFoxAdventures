import {E} from './Element.js';
import Table from './Table.js';
import {hex, makeCollapsibleList} from './Util.js';
import GameTextBin from './GameTextBin.js';
import HexView from './HexView.js';
import {TexFmtName} from './Texture.js';

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
        this.font = new GameTextBin(this.app, this.app.params.get('path'));
        await this.font.load();

        //populate texts table
        for(let i=0; i<this.font.texts.length; i++) {
            let text = this.font.texts[i];
            //console.log("Phrases", text.phrases);
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

        //populate textures table
        for(let i=0; i<this.font.textures.length; i++) {
            let texture = this.font.textures[i];
            let fmt = TexFmtName[texture.format];
            if(fmt == undefined) fmt = `0x${hex(texture.format)}`;
            this.eTextures.append(E.tr('texture',
                E.td('number', i),
                E.td('str', fmt),
                E.td('hex', `0x${hex(texture.pixFmt,2)}`),
                E.td('number', texture.width),
                E.td('number', texture.height),
            ), E.tr(
                E.td({colspan:5}, E.div('image', texture.canvas)),
                //E.td({colspan:5}, E.div('image', texture.img)),
            ));
        }

        //populate unknown data
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
        this.eTextures = E.table(E.tr(
            E.th(null, "#"),
            E.th(null, "TexFmt"),
            E.th(null, "PixFmt"),
            E.th(null, "W"),
            E.th(null, "H"),
        ));
        this.element = E.div('fonts',
            E.div('box texts',
                E.h2('header', "Texts"),
                this.eTexts,
            ),
            E.div('box textures',
                E.h2('header', "Textures"),
                this.eTextures,
            ),
            E.div('box unknown-data',
                E.h2('header', "Unknown"),
                this.unkDataView.element,
            ),
        );
    }
}
