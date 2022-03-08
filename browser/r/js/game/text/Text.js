import { Language } from "./Language.js";
import Phrase from "./Phrase.js";
import { assertType, int } from "../../Util.js";

export default class Text {
    /** One "text" in a GameText file.
     *  This contains one or more phrases and tells which
     *  language they're in, which window type to use,
     *  and how to align them.
     *  Each text has an ID which, combined with a language,
     *  uniquely identifies it.
     */
    constructor(id=null, phrases=[], lang=Language.English,
    window=0, align=[0,0]) {
        this.id       = id;
        this.phrases  = [];
        this.language = lang;
        this.window   = window;
        this.align    = align; // [h, v]
        for(let phrase of phrases) this.addPhrase(phrase);
    }

    static fromXml(elem, lang=Language.English) {
        /** Read Text from XML <text> element. */
        const id     = int(elem.getAttribute('id'));
        const window = int(elem.getAttribute('window'));
        const align  = [
            int(elem.getAttribute('alignH')),
            int(elem.getAttribute('alignV')),
        ];
        const self = new Text(id, [], lang, window, align);
        for(let ePhrase of elem.getElementsByTagName('phrase')) {
            self.addPhrase(Phrase.fromXml(ePhrase));
        }
        return self;
    }

    addPhrase(phrase) {
        /** Called by readers to add a phrase to an existing Text.
         *  Used because when parsing binary GameText files, the
         *  actual string data can't be read at the same time as
         *  the Text structs.
         *  @param {Phrase} phrase phrase to add.
         */
        assertType(phrase, Phrase);
        this.phrases.push(phrase);
    }
}
