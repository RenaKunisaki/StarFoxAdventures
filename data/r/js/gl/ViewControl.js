import {E} from '/r/js/Element.js';

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
        this.chkEnableTex = E.input(null, {type:'checkbox', id:'chkEnableTex'});
        this.lblEnableTex = E.label(null, {'for':'chkEnableTex'}, "Textures");
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
        this.txtZNear  = E.input('coord float',          {type:'number', step:5});
        this.txtZFar   = E.input('coord float',          {type:'number', step:5});

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
                    E.td(this.chkEnableTex, this.lblEnableTex, {colspan:2}),
                ),
                E.tr(
                    E.th(null, "Near"),
                    E.td(null, this.txtZNear),
                    E.th(null, "Far"),
                    E.td(null, this.txtZFar),
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
        this.txtZNear .setAttribute('value', this.context.zNear);
        this.txtZFar  .setAttribute('value', this.context.zFar);
        //input event fires for *every* change.
        //change event only fires when committing, eg pressing Enter.
        this.txtPosX  .addEventListener('input', e => this._onChange(e));
        this.txtPosY  .addEventListener('input', e => this._onChange(e));
        this.txtPosZ  .addEventListener('input', e => this._onChange(e));
        this.txtScaleX.addEventListener('input', e => this._onChange(e));
        this.txtScaleY.addEventListener('input', e => this._onChange(e));
        this.txtScaleZ.addEventListener('input', e => this._onChange(e));
        this.txtRotX  .addEventListener('input', e => this._onChange(e));
        this.txtRotY  .addEventListener('input', e => this._onChange(e));
        this.txtRotZ  .addEventListener('input', e => this._onChange(e));
        this.txtFov   .addEventListener('input', e => this._onChange(e));
        this.txtZNear .addEventListener('input', e => this._onChange(e));
        this.txtZFar  .addEventListener('input', e => this._onChange(e));
        this.chkEnableTex.addEventListener('change', e => this._onChange(e));
    }

    set(params) {
        /** Manually change parameters.
         */
        if(params.pos) {
            if(params.pos.x != undefined) this.txtPosX.value = params.pos.x;
            if(params.pos.y != undefined) this.txtPosY.value = params.pos.y;
            if(params.pos.z != undefined) this.txtPosZ.value = params.pos.z;
        }
        if(params.rot) {
            if(params.rot.x != undefined) this.txtRotX.value = params.rot.x;
            if(params.rot.y != undefined) this.txtRotY.value = params.rot.y;
            if(params.rot.z != undefined) this.txtRotZ.value = params.rot.z;
        }
        if(params.scale) {
            if(params.scale.x != undefined) this.txtScaleX.value = params.scale.x;
            if(params.scale.y != undefined) this.txtScaleY.value = params.scale.y;
            if(params.scale.z != undefined) this.txtScaleZ.value = params.scale.z;
        }
        if(params.zNear != undefined) this.txtZNear.value = params.zNear;
        if(params.zFar  != undefined) this.txtZFar .value = params.zFar;
        this._onChange(null);
    }

    _onChange(event) {
        this.context.view.pos.x = this.txtPosX.value;
        this.context.view.pos.y = this.txtPosY.value;
        this.context.view.pos.z = this.txtPosZ.value;
        this.context.view.rotation.x = this.txtRotX.value;
        this.context.view.rotation.y = this.txtRotY.value;
        this.context.view.rotation.z = this.txtRotZ.value;
        this.context.view.scale.x = this.txtScaleX.value;
        this.context.view.scale.y = this.txtScaleY.value;
        this.context.view.scale.z = this.txtScaleZ.value;
        this.context.zNear = parseFloat(this.txtZNear.value);
        this.context.zFar =  parseFloat(this.txtZFar.value);
        this.context.fov =   parseFloat(this.txtFov.value);
        this.context.enableTextures = this.chkEnableTex.checked;
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
        this.txtZNear.value = 0.1;
        this.txtZFar.value = 10000;
        this._onChange(null);
    }
}
