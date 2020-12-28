import {E} from '/r/js/Element.js';

export default class Picker {
    /** UI widget for displaying info about picked item.
     */
    constructor(context) {
        this.context = context;
        this.gl      = context.gl;
        this._createUi();
    }

    _createUi() {
        /** Create the UI widgets.
         */
        this.lblInfo = E.div('debug', {id:'glPicker'}, "No object");
        this.element = E.div('box',
            E.details('gl-picker',
                E.summary(null, "Picker"), this.lblInfo,
            ),
        );
    }
}
