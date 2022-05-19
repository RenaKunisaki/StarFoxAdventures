import { E } from "../../../lib/Element.js";

export default class HelpBox {
    /** Widget displaying help/instructions for map viewer. */
    constructor(mapViewer) {
        this.mapViewer = mapViewer;
        this.game      = mapViewer.game;
        this.app       = mapViewer.game.app;
        this.element   = E.details('help',
            E.summary(null, "Help"),
            E.div('controls',
                E.h2(null, "Controls"),
                E.ul(null,
                    E.li(null, "Arrow keys: Move camera"),
                    E.li(null, "W/A/S/D: Move camera"),
                    E.li(null, "Left mouse: Select object"),
                    E.li(null, "Right mouse: Turn camera"),
                    E.li(null, "Middle mouse: Move camera"),
                    E.li(null, "Mouse wheel: Forward/back"),
                    E.li(null, "Mouse wheel + shift: Up/down"),
                ),
            ), //controls
        ); //help box
    }
}
