import {E} from './Element.js';
import {int, getDescriptionAndNotes} from './Util.js';

export default class GameText {
    /** An text message in the game.
     */
    constructor(game, eText) {
        /** Construct a GameText from a 'text' element from gametext.xml.
         */
        this.game    = game;
        this.id      = int(eText.getAttribute('id'));
        this.window  = int(eText.getAttribute('window'));
        this.alignH  = int(eText.getAttribute('alignH'));
        this.alignV  = int(eText.getAttribute('alignV'));
        this.langId  = int(eText.getAttribute('language'));
        this.phrases = [];
        this.dirs    = [];
        getDescriptionAndNotes(this, eText);

        for(let ePhrase of eText.getElementsByTagName('phrase')) {
            this.phrases.push(ePhrase.textContent);
        }
        for(let eDir of eText.getElementsByTagName('dir')) {
            this.dirs.push(eDir.getAttribute('name'));
        }
    }

    toList() {
        /** Build an HTML list of this text's phrases.
         */
        const ul = E.ul('gametext');
        for(let phrase of this.phrases) {
            ul.append(E.li(null, phrase));
        }
        return ul;
    }
}
