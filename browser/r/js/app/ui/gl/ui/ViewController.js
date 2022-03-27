import { E } from "../../../../lib/Element.js";

export default class ViewController {
    /** Controls the "camera" of a GL context. */
    constructor(context) {
        this.context = context;
        this._createElements();
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
        this._onChange(null); //trigger an update
    }

    get() {
        /** Read parameters. */
        const F = parseFloat;
        return {
            pos: {
                x:F(this.txtPosX.value),
                y:F(this.txtPosY.value),
                z:F(this.txtPosZ.value),
            },
            rot: {
                x:F(this.txtRotX.value),
                y:F(this.txtRotY.value),
                z:F(this.txtRotZ.value),
            },
            scale: {
                x:F(this.txtScaleX.value),
                y:F(this.txtScaleY.value),
                z:F(this.txtScaleZ.value),
            },
            zNear: F(this.txtZNear.value),
            zFar:  F(this.txtZFar.value),
        };
    }

    adjust(params) {
        /** Add to parameters. */
        if(params.pos) {
            if(params.pos.x != undefined) this.txtPosX.value += params.pos.x;
            if(params.pos.y != undefined) this.txtPosY.value += params.pos.y;
            if(params.pos.z != undefined) this.txtPosZ.value += params.pos.z;
        }
        if(params.rot) {
            if(params.rot.x != undefined) this.txtRotX.value += params.rot.x;
            if(params.rot.y != undefined) this.txtRotY.value += params.rot.y;
            if(params.rot.z != undefined) this.txtRotZ.value += params.rot.z;
        }
        if(params.scale) {
            if(params.scale.x != undefined) this.txtScaleX.value += params.scale.x;
            if(params.scale.y != undefined) this.txtScaleY.value += params.scale.y;
            if(params.scale.z != undefined) this.txtScaleZ.value += params.scale.z;
        }
        if(params.zNear != undefined) this.txtZNear.value += params.zNear;
        if(params.zFar  != undefined) this.txtZFar .value += params.zFar;
        this._onChange(null); //trigger an update
    }

    reset() {
        this.txtPosX.value   =     0;
        this.txtPosY.value   =     0;
        this.txtPosZ.value   =  -100;
        this.txtScaleX.value =     1;
        this.txtScaleY.value =     1;
        this.txtScaleZ.value =     1;
        this.txtRotX.value   =     0;
        this.txtRotY.value   =     0;
        this.txtRotZ.value   =     0;
        this.txtFov.value    =    60;
        this.txtZNear.value  =     0.1;
        this.txtZFar.value   = 10000;
        this._onChange(null);
    }

    _onChange(event) {
        const F = parseFloat;
        this.context.view.pos.x      = F(this.txtPosX.value);
        this.context.view.pos.y      = F(this.txtPosY.value);
        this.context.view.pos.z      = F(this.txtPosZ.value);
        this.context.view.rotation.x = F(this.txtRotX.value);
        this.context.view.rotation.y = F(this.txtRotY.value);
        this.context.view.rotation.z = F(this.txtRotZ.value);
        this.context.view.scale.x    = F(this.txtScaleX.value);
        this.context.view.scale.y    = F(this.txtScaleY.value);
        this.context.view.scale.z    = F(this.txtScaleZ.value);
        this.context.zNear           = F(this.txtZNear.value);
        this.context.zFar            = F(this.txtZFar.value);
        this.context.fov             = F(this.txtFov.value);
        this.context.enableTextures  = this.chkEnableTex.checked;
        this.context.redraw();
    }

    _createElements() {
        /** Create elements for manual view parameter entry. */
        this._createInputFields();
        this._createMainElement();
    }

    _createNumericEntry(cls, step, val) {
        const elem = E.input(cls, {type:'number',step:step,value:val});
        //input event fires for *every* change.
        //change event only fires when committing, eg pressing Enter.
        elem.addEventListener('input', e => this._onChange(e));
        return elem;
    }

    _createInputFields() {
        //numeric entry fields for camera position, scale, angle, FOV, planes
        const F = (c,s,v) => this._createNumericEntry(c,s,v);
        const C = this.context;
        this.txtPosX  =F('x coord float',    1,    C.view.pos.x);
        this.txtPosY  =F('y coord float',    1,    C.view.pos.y);
        this.txtPosZ  =F('z coord float',    1,    C.view.pos.z);
        this.txtScaleX=F('x scale float',    0.01, C.view.scale.x);
        this.txtScaleY=F('y scale float',    0.01, C.view.scale.y);
        this.txtScaleZ=F('z scale float',    0.01, C.view.scale.z);
        this.txtRotX  =F('x angle float',   15,    C.view.rotation.x);
        this.txtRotY  =F('y angle float',   15,    C.view.rotation.y);
        this.txtRotZ  =F('z angle float',   15,    C.view.rotation.z);
        this.txtFov   =F('fov angle float',  5,    C.fov);
        this.txtZNear =F('znear coord float',5,    C.zNear);
        this.txtZFar  =F('zfar coord float', 5,    C.zFar);

        //checkbox to enable textures
        this.chkEnableTex = E.input(null, {type:'checkbox', id:'chkEnableTex'});
        this.lblEnableTex = E.label(null, {'for':'chkEnableTex'}, "Textures");

        //button to reset to default params
        this.btnReset = E.button('reset', "Reset");
        this.btnReset.addEventListener('click', e => this.reset());
    }

    _createMainElement() {
        this.element = E.details('gl-view-control', {open:'open'},
            E.summary(null, "View"),
            E.table(
                E.tr(E.th(this.btnReset),
                    E.th(null,'X'), E.th(null,'Y'), E.th(null,'Z'),
                ),
                E.tr(
                    E.th(null,'Pos'),
                    E.td(this.txtPosX), E.td(this.txtPosY), E.td(this.txtPosZ),
                ),
                E.tr(
                    E.th(null,'Scale'),
                    E.td(this.txtScaleX), E.td(this.txtScaleY), E.td(this.txtScaleZ),
                ),
                E.tr(
                    E.th(null,'Rot\u00B0'), //degree symbol
                    E.td(this.txtRotX), E.td(this.txtRotY), E.td(this.txtRotZ),
                ),
                E.tr(
                    E.th(null, "FOV"),
                    E.td(this.txtFov),
                    E.td(this.chkEnableTex, this.lblEnableTex, {colspan:2}),
                ),
                E.tr(
                    E.th(null, "Near"), E.td(null, this.txtZNear),
                    E.th(null, "Far"),  E.td(null, this.txtZFar),
                ),
            ),
        );
    }
}
