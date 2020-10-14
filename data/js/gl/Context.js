import {get} from '/js/Util.js';
import {E} from '/js/Element.js';
//import Program from './Program.js';
import GX from './gx/GX.js';
import Model from './sfa/Model.js';
import DataBuffer from './gx/DataBuffer.js';

const RADIANS = Math.PI / 180;

export default class Context {
    /** GL context wrapper.
     */
    constructor(canvas) {
        this.fov   = 60; //degrees
        this.zNear = 0.1;
        this.zFar  = 10000;
        this.clearColor = [0.0, 0.5, 0.5, 1.0];
        this.clearDepth = 0.0;
        //XXX add getters/setters for these
        this._gl_extensions = {};

        //we could extend this to a Light object but we really don't need it...
        this.lights = {
            directional: {
                vector: [0.25, -0.5, 0.25],
                color:  [255, 255, 255],
            },
            ambient: {
                color:  [64, 64, 64],
            },
        }

        //Get the canvas and set up GL on it.
        if(typeof canvas == "string") canvas = document.querySelector(canvas);
        if(!canvas) {
            throw new Error("Invalid or missing canvas");
        }
        this.canvas = canvas;
        this.gl = canvas.getContext("webgl");
        const gl = this.gl;

        if(this.gl === null) {
            alert("Unable to initialize WebGL");
            throw new Error("GL init failed (maybe not supported on this browser?)");
        }
        console.log("GL init OK");
        canvas.addEventListener('resize', e => this._onResize(e));
        canvas.addEventListener('mousemove', e => this._onMouseMove(e));

        //setup state
        gl.clearColor(this.clearColor[0], this.clearColor[1],
            this.clearColor[2], this.clearColor[3]);
        gl.clearDepth(this.clearDepth);
        gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
        gl.enable(gl.DEPTH_TEST); // Enable depth testing
        gl.depthFunc(gl.GEQUAL);  // Near things obscure far things
        gl.enable(gl.CULL_FACE);
        gl.enable(gl.BLEND);
        gl.blendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA);
        this._initMatrices();
        this.setupViewport();
        this._setupDepthTexture();

        //enable DDS textures (though we don't currently use them)
        //https://developer.mozilla.org/en-US/docs/Web/API/WEBGL_compressed_texture_s3tc
        this._gl_extensions.compressed_texture_s3tc = (
            gl.getExtension('WEBGL_compressed_texture_s3tc') ||
            gl.getExtension('MOZ_WEBGL_compressed_texture_s3tc') ||
            gl.getExtension('WEBKIT_WEBGL_compressed_texture_s3tc'));

        this.gx = new GX(this);
    }

    async loadPrograms() {
        await this.gx.loadPrograms();
    }

    setRenderer(renderer) {
        /** Set the class used to render the scene.
         *  This object's render() and renderPickBuffer() methods are called
         *  to render the scene after setting it up.
         */
        this.renderer = renderer;
    }

    _setupDepthTexture() {
        /** Set up depth-render-to-texture for picking.
         */
        const gl = this.gl;

        this._gl_extensions.depth_texture = gl.getExtension('WEBGL_depth_texture');

        // Create a texture to render to
        this._targetTexture = gl.createTexture();
        gl.bindTexture(gl.TEXTURE_2D, this._targetTexture);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);

        // create a depth renderbuffer
        this._depthBuffer = gl.createRenderbuffer();
        gl.bindRenderbuffer(gl.RENDERBUFFER, this._depthBuffer);

        this._setFramebufferAttachmentSizes();

        // Create and bind the framebuffer
        this._frameBuffer = gl.createFramebuffer();
        gl.bindFramebuffer(gl.FRAMEBUFFER, this._frameBuffer);

        // attach the texture as the first color attachment
        const attachmentPoint = gl.COLOR_ATTACHMENT0;
        const level = 0;
        gl.framebufferTexture2D(gl.FRAMEBUFFER, attachmentPoint, gl.TEXTURE_2D,
            this._targetTexture, level);

        // make a depth buffer and the same size as the targetTexture
        gl.framebufferRenderbuffer(gl.FRAMEBUFFER, gl.DEPTH_ATTACHMENT,
            gl.RENDERBUFFER, this._depthBuffer);
    }

    _setFramebufferAttachmentSizes() {
        /** Set up framebuffer after window is resized.
         *  Required for depth buffer picker.
         */
        const gl = this.gl;
        const width=gl.canvas.clientWidth, height=gl.canvas.clientHeight;

        gl.bindTexture(gl.TEXTURE_2D, this._targetTexture);
        // define size and format of level 0
        const level          = 0;
        const internalFormat = gl.RGBA;
        const border         = 0;
        const format         = gl.RGBA;
        const type           = gl.UNSIGNED_BYTE;
        const data           = null;
        gl.texImage2D(gl.TEXTURE_2D, level, internalFormat,
            width, height, border, format, type, data);
        gl.bindRenderbuffer(gl.RENDERBUFFER, this._depthBuffer);
        gl.renderbufferStorage(gl.RENDERBUFFER, gl.DEPTH_COMPONENT16,
            width, height);
    }

    _initMatrices() {
        /** Create modelview and projection matrices.
         */
        this.matProjection = mat4.create();
        this.matModelView  = mat4.create();
        this.matNormal     = null; //generated when redrawing
        this.view = {
            pos:      {x:0, y:-20, z:-40},
            scale:    {x:1, y:1,   z:1},
            rotation: {x:0, y:0,   z:0}, //degrees
        };
    }

    redraw() {
        /** Redraw the scene.
         */
        const gl = this.gl;
        const P  = this.view.pos;
        const S  = this.view.scale;
        const R  = this.view.rotation;
        const aspect = gl.canvas.clientWidth / gl.canvas.clientHeight;

        //set up projection matrix
        mat4.perspective(this.matProjection, //destination matrix
            this.fov * RADIANS, //to radians
            aspect, this.zNear, this.zFar);

        //set up modelview matrix
        //it can help to think of this as moving the "drawing position"
        //rather than thew camera.
        mat4.translate(this.matModelView, // destination matrix
            mat4.create(),     // matrix to translate
            [P.x, P.y, P.z]);  // amount to translate
        mat4.scale(this.matModelView, this.matModelView,
            [S.x, S.y, S.z]);
        mat4.rotateX(this.matModelView, this.matModelView, R.x * RADIANS);
        mat4.rotateY(this.matModelView, this.matModelView, R.y * RADIANS);
        mat4.rotateZ(this.matModelView, this.matModelView, R.z * RADIANS);

        //set up normal matrix, for lighting.
        this.matNormal = mat4.create();
        mat4.invert(this.matNormal, this.matModelView);
        mat4.transpose(this.matNormal, this.matNormal);

        //clear buffers and render.
        //gl.enable(gl.BLEND);
        gl.disable(gl.BLEND);
        gl.clearColor(this.clearColor[0], this.clearColor[1],
            this.clearColor[2], this.clearColor[3]);
        gl.bindFramebuffer(gl.FRAMEBUFFER, null);
        gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
        this.gx.reset();
        //gl.uniform1i(this.gx.programInfo.uniforms.useId, 1);
        this.renderer.render();
        //this.gx.drawBoundCube();

        //now render again to our depth buffer.
        gl.disable(gl.BLEND);
        gl.clearColor(1.0, 1.0, 1.0, 1.0);
        gl.bindFramebuffer(gl.FRAMEBUFFER, this._frameBuffer);
        gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
        this.gx.reset();
        gl.uniform1i(this.gx.programInfo.uniforms.useId, 1);
        this.renderer.renderPickBuffer();
    }

    setupViewport() {
        /** Set up viewport.
         *  Called at startup and on resize.
         */
        const gl = this.gl;

        //we need to set the size through HTML attributes, not CSS,
        //or else it blurs, because lol.
        //why -32? no idea, but it makes the canvas fit into the viewport.
        gl.canvas.setAttribute('width', window.innerWidth - 32);
        gl.canvas.setAttribute('height', window.innerHeight - 32);

        const width=gl.canvas.clientWidth, height=gl.canvas.clientHeight;
        gl.viewport(0, 0, width, height);
    }

    _onResize(event) {
        this.setupViewport();
        this._setFramebufferAttachmentSizes();
        this.redraw();
    }

    _onMouseMove(event) {
        //XXX some of this ought to be moved to the Picker class.
        if(!this.gx.vtxLog) {
            //nothing was drawn yet
            return;
        }
        const gl     = this.gl;
        const rect   = gl.canvas.getBoundingClientRect();
        const mouseX = event.clientX - rect.left;
        const mouseY = event.clientY - rect.top;
        const pixelX = mouseX * gl.canvas.width / gl.canvas.clientWidth;
        const pixelY = gl.canvas.height - mouseY * gl.canvas.height / gl.canvas.clientHeight - 1;
        const data   = new Uint8Array(4);
        gl.readPixels(
            pixelX,            // x
            pixelY,            // y
            1,                 // width
            1,                 // height
            gl.RGBA,           // format
            gl.UNSIGNED_BYTE,  // type
            data);             // typed array to hold result
        const id = data[3] + (data[2] << 8) + (data[1] << 16) + (data[0] << 24);
        const vtx = this.gx.vtxLog[id];
        const picker = document.getElementById('glPicker');
        picker.innerText = '';

        if(vtx) {
            const items = E.table();
            for(let [name, val] of Object.entries(vtx)) {
                /* if(typeof val == 'object') {
                    let d = [];
                    for(let [k, v] of Object.entries(val)) {
                        if(typeof v == 'number') v = Math.round(v*100)/100;
                        d.push(`${k}: ${v}`);
                    }
                } */
                items.append(E.tr(
                    E.th(null, name),
                    E.td(null, String(val)),
                ));
            }
            picker.append(items);
        }
        else picker.innerText = "Nothing here";
    }
}
