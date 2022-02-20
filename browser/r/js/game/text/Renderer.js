import { E } from "../../lib/Element.js";
import { Language } from "./Language.js";
import Text from "./Text.js";
import Phrase from "./Phrase.js";
import { CommandBase, Command } from "./Command.js";

export default class GameTextRenderer {
    /** Builds HTML representation of a Text. */
    constructor(app) {
        this.app = app;
    }

    render(text) {
        /** Render the specified Text. */
        console.assert(text instanceof Text);

        const params = {
            font:    text.language == Language.Japanese ? 0 : 4,
            justify: 'Left',
            color:   [255,255,255,255],
            size:    100, //percent
            hint:    null,
        };

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
                params.color = [255, 255, 255, 255];
                //It's not necessary to reset other params since they
                //aren't changed by any hint texts.
            }
        }
        return list;
    }
}
