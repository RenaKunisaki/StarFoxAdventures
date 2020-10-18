import {E} from '/js/Element.js';

export default class ViewControl {
    /** UI widget to control the viewport.
     */
    constructor(context) {
        this.context = context;
        this.gl      = context.gl;
        this._createUi();
    }

    _createUi() {
        /** Create the UI widgets.
         */
        this.txtPosX   = E.input('x-coord coord float',  {type:'number', step:1});
        this.txtPosY   = E.input('y-coord coord float',  {type:'number', step:1});
        this.txtPosZ   = E.input('z-coord coord float',  {type:'number', step:1});
        this.txtScaleX = E.input('x-scale scale float',  {type:'number', step:0.01});
        this.txtScaleY = E.input('y-scale scale float',  {type:'number', step:0.01});
        this.txtScaleZ = E.input('z-scale scale float',  {type:'number', step:0.01});
        this.txtRotX   = E.input('x-rotate angle float', {type:'number', step:15});
        this.txtRotY   = E.input('y-rotate angle float', {type:'number', step:15});
        this.txtRotZ   = E.input('z-rotate angle float', {type:'number', step:15});
        this.txtFov    = E.input('fov angle float',      {type:'number', step:5});

        this.btnReset = E.button('reset', "Reset");
        this.btnReset.addEventListener('click', e => this._reset());

        this.element = E.details('gl-view-control', {open:'open'},
            E.summary(null, "View"),
            E.table(
                E.tr(E.th(this.btnReset),
                    E.th(null,'X'),
                    E.th(null,'Y'),
                    E.th(null,'Z'),
                ),
                E.tr(
                    E.th(null,'Pos'),
                    E.td(this.txtPosX),
                    E.td(this.txtPosY),
                    E.td(this.txtPosZ),
                ),
                E.tr(
                    E.th(null,'Scale'),
                    E.td(this.txtScaleX),
                    E.td(this.txtScaleY),
                    E.td(this.txtScaleZ),
                ),
                E.tr(
                    E.th(null,'Rot\u00B0'),
                    E.td(this.txtRotX),
                    E.td(this.txtRotY),
                    E.td(this.txtRotZ),
                ),
                E.tr(
                    E.th(null, "FOV"),
                    E.td(this.txtFov),
                ),
            ),
        );
        this.txtPosX  .setAttribute('value', this.context.view.pos.x);
        this.txtPosY  .setAttribute('value', this.context.view.pos.y);
        this.txtPosZ  .setAttribute('value', this.context.view.pos.z);
        this.txtScaleX.setAttribute('value', this.context.view.scale.x);
        this.txtScaleY.setAttribute('value', this.context.view.scale.y);
        this.txtScaleZ.setAttribute('value', this.context.view.scale.z);
        this.txtRotX  .setAttribute('value', this.context.view.rotation.x);
        this.txtRotY  .setAttribute('value', this.context.view.rotation.y);
        this.txtRotZ  .setAttribute('value', this.context.view.rotation.z);
        this.txtFov   .setAttribute('value', this.context.fov);
        //input event fires for *every* change.
        //change event only fires when committing, eg pressing Enter.
        this.txtPosX  .addEventListener('input', e => this._onPosChange(e));
        this.txtPosY  .addEventListener('input', e => this._onPosChange(e));
        this.txtPosZ  .addEventListener('input', e => this._onPosChange(e));
        this.txtScaleX.addEventListener('input', e => this._onScaleChange(e));
        this.txtScaleY.addEventListener('input', e => this._onScaleChange(e));
        this.txtScaleZ.addEventListener('input', e => this._onScaleChange(e));
        this.txtRotX  .addEventListener('input', e => this._onRotChange(e));
        this.txtRotY  .addEventListener('input', e => this._onRotChange(e));
        this.txtRotZ  .addEventListener('input', e => this._onRotChange(e));
        this.txtFov   .addEventListener('input', e => this._onFovChange(e));
    }

    _onPosChange(event) {
        /** Called when the value of X, Y or Z coord input changes.
         */
        this.context.view.pos.x = this.txtPosX.value;
        this.context.view.pos.y = this.txtPosY.value;
        this.context.view.pos.z = this.txtPosZ.value;
        this.context.redraw();
    }
    _onScaleChange(event) {
        /** Called when the value of X, Y or Z scale input changes.
         */
        this.context.view.scale.x = this.txtScaleX.value;
        this.context.view.scale.y = this.txtScaleY.value;
        this.context.view.scale.z = this.txtScaleZ.value;
        this.context.redraw();
    }
    _onRotChange(event) {
        /** Called when the value of X, Y or Z rotation input changes.
         */
        this.context.view.rotation.x = this.txtRotX.value;
        this.context.view.rotation.y = this.txtRotY.value;
        this.context.view.rotation.z = this.txtRotZ.value;
        this.context.redraw();
    }
    _onFovChange(event) {
        /** Called when the value of FOV input changes.
         */
        this.context.fov = this.txtFov.value;
        this.context.redraw();
    }

    _reset() {
        this.txtPosX.value = 0;
        this.txtPosY.value = 0;
        this.txtPosZ.value = -100;
        this.txtScaleX.value = 1;
        this.txtScaleY.value = 1;
        this.txtScaleZ.value = 1;
        this.txtRotX.value = 0;
        this.txtRotY.value = 0;
        this.txtRotZ.value = 0;
        this.txtFov.value = 60;
        this.context.view.pos.x = this.txtPosX.value;
        this.context.view.pos.y = this.txtPosY.value;
        this.context.view.pos.z = this.txtPosZ.value;
        this.context.view.scale.x = this.txtScaleX.value;
        this.context.view.scale.y = this.txtScaleY.value;
        this.context.view.scale.z = this.txtScaleZ.value;
        this.context.view.rotation.x = this.txtRotX.value;
        this.context.view.rotation.y = this.txtRotY.value;
        this.context.view.rotation.z = this.txtRotZ.value;
        this.context.redraw();
    }
}
