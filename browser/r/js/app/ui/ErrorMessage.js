import { E } from "../../lib/Element.js";

export default class ErrorMessage {
    //an element showing an error message.
    //this class mainly exists so it can be used in place
    //of the various classes FileViewer uses.
    constructor(app, err) {
        this.app     = app;
        this.error   = err;
        this.element = E.div('error', err);
    }
}
