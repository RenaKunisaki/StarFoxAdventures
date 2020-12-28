import {E, clearElement} from '/r/js/Element.js';

const formatNames = {
    0x0: 'I4',
    0x1: 'I8',
    0x2: 'IA4',
    0x3: 'IA8',
    0x4: 'RGB565',
    0x5: 'RGB5A3',
    0x6: 'RGBA32',
    0x8: 'C4',
    0x9: 'C8',
    0xA: 'C14X2',
    0xE: 'CMPR',
};

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
        this.eInfo      = E.table('info');
        this.element    = E.details('gl-texture-view',
            E.summary(null, "Textures"),
            E.div(null,
                this.lstTexture,
                this.eInfo,
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
        clearElement(this.lstTexture);
        for(let i=0; i<this.model.textures.length; i++) {
            const tex = this.model.textures[i];
            const id  = tex.id.toString(16).padStart(4, '0').toUpperCase();
            let text = `#${i} (0x${id}, ${tex.width}x${tex.height}, ${tex.nFrames} frames)`;
            this.lstTexture.append(E.option(null, {value:i}, text));
        }
        this._onChange(null);
    }

    _onChange(event) {
        /** Called when the value of an input changes.
         */
        const tex = this.model.textures[this.lstTexture.value];
        this.img.src = tex.image.src;
        clearElement(this.eInfo).append(
            E.tr(
                E.th(null, "Offset"),
                E.td('hex', tex.offset.toString(16).padStart(8, '0').toUpperCase()),
            ),
            E.tr(
                E.th(null, "Packed Size"),
                E.td('hex', tex.packedLength.toString(16).padStart(8, '0').toUpperCase()),
            ),
            E.tr(
                E.th(null, "Raw Size"),
                E.td('hex', tex.rawLength.toString(16).padStart(8, '0').toUpperCase()),
            ),
            E.tr(
                E.th(null, "Format"),
                E.td('str', formatNames[tex.format]),
            ),
        );
    }
}
