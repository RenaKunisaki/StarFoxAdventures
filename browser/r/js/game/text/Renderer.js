import { E } from "../../lib/Element.js";
import { Language } from "./Language.js";
import Text from "./Text.js";
import Phrase from "./Phrase.js";
import { assertType } from "../../Util.js";

export default class GameTextRenderer {
    /** Builds HTML representation of a Text. */
    constructor(app) {
        this.app = app;
    }

    render(text) {
        /** Render the specified Text. */
        assertType(text, Text);

        const params = {hint:null};
        this._resetParams(params, text);

        const list = E.ul('phrases');
        for(let phrase of text.phrases) {
            console.assert(phrase instanceof Phrase);
            list.append(E.li('phrase', phrase.toHtml(params)));
            if(params.hint != null) {
                //hack: Normally, the first phrase of a hint text is the
                //summary shown on the file select screen, and the rest
                //are the hint shown in Slippy's page in the pause menu.
                //This means the game doesn't bother to set a color for
                //subsequent phrases, but expects them to be the default
                //white, even though the first line changes it to orange.
                //We mimic that behaviour by resetting the color after
                //a phrase that contains the Hint command (which is quite
                //convenient as the game seems to just ignore this command).
                this._resetParams(params, text);
            }
        }
        return list;
    }

    _resetParams(params, text) {
        params.font    = text.language == Language.Japanese ? 0 : 4;
        params.justify = 'Left';
        params.color   = [255,255,255,255];
        params.size    = 100; //percent
        //params.hint    = null;
    }
}
