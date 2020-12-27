import {E} from '/js/Element.js';

export default class LightControl {
    /** UI widget to control the lighting.
     */
    constructor(context) {
        this.context = context;
        this.gl      = context.gl;
        this._createUi();
    }

    _createUi() {
        /** Create the UI widgets.
         */
        this.txtVecX = E.input('x-coord coord float', {type:'number', step:0.01});
        this.txtVecY = E.input('y-coord coord float', {type:'number', step:0.01});
        this.txtVecZ = E.input('z-coord coord float', {type:'number', step:0.01});
        this.txtDirR = E.input('color-r color u8',    {type:'number', step:16, min:0, max:255});
        this.txtDirG = E.input('color-g color u8',    {type:'number', step:16, min:0, max:255});
        this.txtDirB = E.input('color-b color u8',    {type:'number', step:16, min:0, max:255});
        this.txtAmbR = E.input('color-r color u8',    {type:'number', step:16, min:0, max:255});
        this.txtAmbG = E.input('color-g color u8',    {type:'number', step:16, min:0, max:255});
        this.txtAmbB = E.input('color-b color u8',    {type:'number', step:16, min:0, max:255});

        const L = this.context.lights;
        this.txtVecX.setAttribute('value', L.directional.vector[0]);
        this.txtVecY.setAttribute('value', L.directional.vector[1]);
        this.txtVecZ.setAttribute('value', L.directional.vector[2]);
        this.txtDirR.setAttribute('value', L.directional.color[0]);
        this.txtDirG.setAttribute('value', L.directional.color[1]);
        this.txtDirB.setAttribute('value', L.directional.color[2]);
        this.txtAmbR.setAttribute('value', L.ambient.color[0]);
        this.txtAmbG.setAttribute('value', L.ambient.color[1]);
        this.txtAmbB.setAttribute('value', L.ambient.color[2]);

        this.btnReset = E.button('reset', "Reset");
        this.btnReset.addEventListener('click', e => this._reset());

        this.element = E.details('gl-view-control',
            E.summary(null, "Lights"),
            E.table(
                E.tr(E.th(this.btnReset),
                    E.th(null,'X'),
                    E.th(null,'Y'),
                    E.th(null,'Z'),
                ),
                E.tr(
                    E.th(null,'Dirctn'),
                    E.td(this.txtVecX),
                    E.td(this.txtVecY),
                    E.td(this.txtVecZ),
                ),
                E.tr(E.th(null, 'Color'),
                    E.th(null,'R'),
                    E.th(null,'G'),
                    E.th(null,'B'),
                ),
                E.tr(
                    E.th(null,'Dirctn'),
                    E.td(this.txtDirR),
                    E.td(this.txtDirG),
                    E.td(this.txtDirB),
                ),
                E.tr(
                    E.th(null,'Ambnt'),
                    E.td(this.txtAmbR),
                    E.td(this.txtAmbG),
                    E.td(this.txtAmbB),
                ),
            ),
        );
        //input event fires for *every* change.
        //change event only fires when committing, eg pressing Enter.
        this.txtVecX.addEventListener('input', e => this._onChange(e));
        this.txtVecY.addEventListener('input', e => this._onChange(e));
        this.txtVecZ.addEventListener('input', e => this._onChange(e));
        this.txtDirR.addEventListener('input', e => this._onChange(e));
        this.txtDirG.addEventListener('input', e => this._onChange(e));
        this.txtDirB.addEventListener('input', e => this._onChange(e));
        this.txtAmbR.addEventListener('input', e => this._onChange(e));
        this.txtAmbG.addEventListener('input', e => this._onChange(e));
        this.txtAmbB.addEventListener('input', e => this._onChange(e));
    }

    _onChange(event) {
        /** Called when the value of an input changes.
         */
        const L = this.context.lights;
        L.directional.vector[0] = this.txtVecX.value;
        L.directional.vector[1] = this.txtVecY.value;
        L.directional.vector[2] = this.txtVecZ.value;
        L.directional.color [0] = this.txtDirR.value;
        L.directional.color [1] = this.txtDirG.value;
        L.directional.color [2] = this.txtDirB.value;
        L.ambient    .color [0] = this.txtAmbR.value;
        L.ambient    .color [1] = this.txtAmbG.value;
        L.ambient    .color [2] = this.txtAmbB.value;
        this.context.redraw();
    }

    _reset() {
        this.txtVecX.setAttribute('value', 0.85);
        this.txtVecY.setAttribute('value', 0.80);
        this.txtVecZ.setAttribute('value', 0.75);
        this.txtDirR.setAttribute('value', 255);
        this.txtDirG.setAttribute('value', 255);
        this.txtDirB.setAttribute('value', 255);
        this.txtAmbR.setAttribute('value', 64);
        this.txtAmbG.setAttribute('value', 64);
        this.txtAmbB.setAttribute('value', 64);
        this._onChange(null);
    }
}
