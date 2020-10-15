import {E} from '/js/Element.js';

export default class TextureView {
    /** UI widget to show textures.
     */
    constructor(context) {
        this.context = context;
        this.gl      = context.gl;
        this.model   = null;
        this._createUi();
    }

    _createUi() {
        /** Create the UI widgets.
         */
        this.lstTexture = E.select();
        this.img        = E.img();
        this.element    = E.details('gl-texture-view',
            E.summary(null, "Textures"),
            E.div(null,
                this.lstTexture,
                this.img,
            ),
        );
        this.lstTexture.addEventListener('change', e => this._onChange(e));
    }

    setModel(model) {
        this.model = model;
        this.refresh();
    }

    refresh() {
        this.lstTexture.innerHtml = '';
        for(let i=0; i<this.model.textures.length; i++) {
            this.lstTexture.append(E.option(null, {value:i}, i.toString()));
        }
        this._onChange(null);
    }

    _onChange(event) {
        /** Called when the value of an input changes.
         */
        this.img.src = this.model.textures[this.lstTexture.value].image.src;
    }
}
