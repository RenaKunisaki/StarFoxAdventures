import { E } from "../../../../lib/Element.js";
const CLAMP_ANGLE = r => {
    while(r < 0) r += 360;
    return r % 360;
};

export default class ViewController {
    /** Controls the "camera" of a GL context. */
    constructor(context) {
        this.context = context;
        this._createElements();
    }

    set(params) {
        /** Manually change parameters.
         */
        //XXX this doesn't always reflect the actual values. eg if we rotate
        //enough the actual rotation value can be -1 but we'll be displaying
        //it as 359. not a huge problem but could become one if something
        //expects these to display the real values...
        if(params.pos) {
            if(params.pos.x != undefined) this.txtPosX.value = params.pos.x;
            if(params.pos.y != undefined) this.txtPosY.value = params.pos.y;
            if(params.pos.z != undefined) this.txtPosZ.value = params.pos.z;
        }
        if(params.rot) {
            if(params.rot.x != undefined) this.txtRotX.value = CLAMP_ANGLE(params.rot.x);
            if(params.rot.y != undefined) this.txtRotY.value = CLAMP_ANGLE(params.rot.y);
            if(params.rot.z != undefined) this.txtRotZ.value = CLAMP_ANGLE(params.rot.z);
        }
        if(params.scale) {
            if(params.scale.x != undefined) this.txtScaleX.value = params.scale.x;
            if(params.scale.y != undefined) this.txtScaleY.value = params.scale.y;
            if(params.scale.z != undefined) this.txtScaleZ.value = params.scale.z;
        }
        if(params.zNear != undefined) this.txtZNear.value = params.zNear;
        if(params.zFar  != undefined) this.txtZFar .value = params.zFar;
        if(params.enableTextures != undefined) {
            this.chkEnableTex.checked = params.enableTextures;
        }
        if(params.useWireframe != undefined) {
            this.chkWireframe.checked = params.useWireframe;
        }
        if(params.enableBackfaceCulling != undefined) {
            this.chkEnableBackface.checked = params.enableBackfaceCulling;
        }
        if(params.frontFaceCW != undefined) {
            this.btnFrontFaceCW .checked =  params.frontFaceCW;
            this.btnFrontFaceCCW.checked = !params.frontFaceCW;
        }
        if(params.useSRT != undefined) {
            this.btnRotateCam.checked =  params.useSRT;
            this.btnRotateOrg.checked = !params.useSRT;
        }
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
            enableTextures: this.chkEnableTex.checked,
            useWireframe: this.chkWireframe.checked,
            enableBackfaceCulling: this.chkEnableBackface.checked,
            frontFaceCW: this.btnFrontFaceCW.checked,
            useSRT: this.btnRotateCam.checked,
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
        if(params.enableTextures != undefined) {
            this.chkEnableTex.checked = params.enableTextures;
        }
        if(params.useWireframe != undefined) {
            this.chkWireframe.checked = params.useWireframe;
        }
        if(params.enableBackfaceCulling != undefined) {
            this.chkEnableBackface.checked = params.enableBackfaceCulling;
        }
        if(params.frontFaceCW != undefined) {
            this.btnFrontFaceCW .checked =  params.frontFaceCW;
            this.btnFrontFaceCCW.checked = !params.frontFaceCW;
        }
        if(params.useSRT != undefined) {
            this.btnRotateCam.checked =  params.useSRT;
            this.btnRotateOrg.checked = !params.useSRT;
        }
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
        this.chkEnableTex.checked      = true;
        this.chkWireframe.checked      = false;
        this.chkEnableBackface.checked = true;
        this.btnFrontFaceCW.checked    = true;
        this.btnFrontFaceCCW.checked   = false;
        this.btnRotateCam.checked      = true;
        this.btnRotateOrg.checked      = false;
        this._onChange(null);
    }

    _onChange(event) {
        const F = parseFloat;
        this.context.view.pos.x      = F(this.txtPosX.value);
        this.context.view.pos.y      = F(this.txtPosY.value);
        this.context.view.pos.z      = F(this.txtPosZ.value);
        this.context.view.rotation.x = CLAMP_ANGLE(F(this.txtRotX.value));
        this.context.view.rotation.y = CLAMP_ANGLE(F(this.txtRotY.value));
        this.context.view.rotation.z = CLAMP_ANGLE(F(this.txtRotZ.value));
        this.context.view.scale.x    = F(this.txtScaleX.value);
        this.context.view.scale.y    = F(this.txtScaleY.value);
        this.context.view.scale.z    = F(this.txtScaleZ.value);
        this.context.zNear           = F(this.txtZNear.value);
        this.context.zFar            = F(this.txtZFar.value);
        this.context.fov             = F(this.txtFov.value);
        this.context.enableTextures  = this.chkEnableTex.checked;
        this.context.useWireframe    = this.chkWireframe.checked;
        this.context.enableBackfaceCulling = this.chkEnableBackface.checked;
        this.context.frontFaceCW = this.btnFrontFaceCW.checked;
        this.context.useSRT = this.btnRotateCam.checked;
        this.context.redraw();
    }

    _createElements() {
        /** Create elements for manual view parameter entry. */
        this._createInputFields();
        this._createMainElement();
    }

    _createNumericEntry(cls, step, val, min=null, max=null) {
        const elem = E.input(cls, {type:'number',step:step,value:val});
        if(min != null) elem.setAttribute('min', min);
        if(max != null) elem.setAttribute('max', max);
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
        this.txtFov   =F('fov angle float',  5,    C.fov, 1, 360);
        this.txtZNear =F('znear coord float',0.01, C.zNear);
        this.txtZFar  =F('zfar coord float', 5,    C.zFar);

        //checkbox to enable textures
        this.chkEnableTex = E.input(null, {type:'checkbox', id:'chkEnableTex'});
        this.lblEnableTex = E.label(null, {'for':'chkEnableTex'}, "Textures");
        this.chkEnableTex.checked = C.enableTextures;
        this.chkEnableTex.addEventListener('change', e => this._onChange(e));

        //checkbox to enable wireframe
        this.chkWireframe = E.input(null, {type:'checkbox', id:'chkWireframe'});
        this.lblWireframe = E.label(null, {'for':'chkWireframe'}, "Wireframe");
        this.chkWireframe.checked = C.useWireframe;
        this.chkWireframe.addEventListener('change', e => this._onChange(e));

        //checkbox to enable backface culling
        this.chkEnableBackface = E.input(null,
            {type:'checkbox', id:'chkEnableBcakface'});
        this.lblEnableBackface = E.label(null,
            {'for':'chkEnableBackface'}, "Cull Backfaces");
        this.chkEnableBackface.checked = C.enableBackfaceCulling;
        this.chkEnableBackface.addEventListener('change', e => this._onChange(e));

        //button to reset to default params
        this.btnReset = E.button('reset', "Reset");
        this.btnReset.addEventListener('click', e => this.reset());

        //radio buttons to select front face order
        this.btnFrontFaceCW = E.input({type:'radio', name:'frontFace',
            id:'frontFaceCW'});
        this.lblFrontFaceCW = E.label(null, {'for':'frontFaceCW'}, "CW");
        this.btnFrontFaceCW.checked = C.frontFaceCW;
        this.btnFrontFaceCW.addEventListener('change', e => this._onChange(e));

        this.btnFrontFaceCCW = E.input({type:'radio', name:'frontFace',
            id:'frontFaceCCW'});
        this.lblFrontFaceCCW = E.label(null, {'for':'frontFaceCCW'}, "CCW");
        this.btnFrontFaceCCW.checked = !C.frontFaceCW;
        this.btnFrontFaceCCW.addEventListener('change', e => this._onChange(e));

        //radio buttons to select rotation point
        this.btnRotateCam = E.input({type:'radio', name:'rotPoint',
            id:'rotPointCam'});
        this.lblRotateCam = E.label(null, {'for':'rotPointCam'}, "Camera");
        this.btnRotateCam.checked = C.useSRT;
        this.btnRotateCam.addEventListener('change', e => this._onChange(e));

        this.btnRotateOrg = E.input({type:'radio', name:'rotPoint',
            id:'rotPointOrg'});
        this.lblRotateOrg = E.label(null, {'for':'rotPointOrg'}, "Origin");
        this.btnRotateOrg.checked = !C.useSRT;
        this.btnRotateOrg.addEventListener('change', e => this._onChange(e));
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
                    E.th(null,'Rot°'),
                    E.td(this.txtRotX), E.td(this.txtRotY), E.td(this.txtRotZ),
                ),

                E.tr(
                    E.td(this.chkEnableTex, this.lblEnableTex, {colspan:2}),
                    E.td(this.chkWireframe, this.lblWireframe, {colspan:2}),
                ),
            ),

            E.details(null, E.summary(null, "Advanced"),
                E.table(
                    E.tr(
                        E.th(null,'Scale'),
                        E.td(this.txtScaleX), E.td(this.txtScaleY), E.td(this.txtScaleZ),
                    ),
                    E.tr(
                        E.th(null, "FOV°"),
                        E.td(this.txtFov),
                        E.td(null, " ", {colspan:2}),
                    ),
                    E.tr(
                        E.th(null, "Near"), E.td(null, this.txtZNear),
                        E.th(null, "Far"),  E.td(null, this.txtZFar),
                    ),
                    E.tr(
                        E.th(null, "Front Faces", {colspan:2}),
                        E.td({colspan:2},
                            this.btnFrontFaceCW, this.lblFrontFaceCW,
                            this.btnFrontFaceCCW, this.lblFrontFaceCCW,
                        ),
                    ),
                    E.tr(
                        E.th(null, "Rotation Axis", {colspan:2}),
                        E.td({colspan:2},
                            this.btnRotateCam, this.lblRotateCam,
                            this.btnRotateOrg, this.lblRotateOrg,
                        ),
                    ),
                    E.tr(
                        E.td({colspan:2},
                            this.chkEnableBackface, this.lblEnableBackface, {
                            title:"Enable backface culling",
                        }),
                    ),
                ),
            ),
        );
    }
}
