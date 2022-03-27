const RADIANS = Math.PI / 180;

export default class Context {
    /** GL context wrapper.
     */
    constructor(canvas, drawFunc=null) {
        /** Construct GL context.
         *  @param {HTMLCanvasElement} canvas The canvas to render to.
         *  @param {function} drawFunc Function to call to render the scene.
         */
        this.fov   = 60; //degrees
        this.zNear = 0.1;
        this.zFar  = 10000;
        this.clearColor = [0.0, 0.5, 0.5, 1.0];
        this.clearDepth = 0.0;
        this.drawFunc   = drawFunc; //callback to draw the scene
        //XXX add getters/setters for these
        this._gl_extensions = {};
        this.enableTextures = false;

        this._setupCanvas(canvas);
        this._setupExtensions();
        this._setupLights();
    }

    async init() {
        //load programs etc
        this._setupState();
    }

    _setupLights() {
        //we could extend this to a Light object but we really don't need it...
        this.lights = {
            directional: {
                vector: [0.25, -0.5, 0.25],
                color:  [255, 255, 255],
            },
            ambient: {
                color:  [64, 64, 64],
            },
            enabled: false,
        }
    }

    _setupCanvas(canvas) {
        //Get the canvas and set up GL on it.
        if(typeof canvas == "string") canvas = document.querySelector(canvas);
        if(!canvas) {
            throw new Error("Invalid or missing canvas");
        }
        this.canvas = canvas;
        this.gl = canvas.getContext("webgl2");

        if(this.gl === null) {
            alert("Unable to initialize WebGL");
            throw new Error("GL init failed (maybe not supported on this browser?)");
        }
        console.log("GL init OK");

        canvas.addEventListener('resize',    e => this._onResize(e));
        canvas.addEventListener('mousemove', e => this._onMouseMove(e));
        this._setupViewport();
    }

    _setupExtensions() {
        const gl = this.gl;
        //enable DDS textures (though we don't currently use them)
        //https://developer.mozilla.org/en-US/docs/Web/API/WEBGL_compressed_texture_s3tc
        this._gl_extensions.compressed_texture_s3tc = (
            gl.getExtension('WEBGL_compressed_texture_s3tc') ||
            gl.getExtension('MOZ_WEBGL_compressed_texture_s3tc') ||
            gl.getExtension('WEBKIT_WEBGL_compressed_texture_s3tc'));

        const lose = this.gl.getExtension('WEBGL_lose_context');
        window.addEventListener('beforeunload', (event) => {
            //unload to free some resources
            lose.loseContext();
        });
        //XXX lose context when switching tabs? (our UI tabs + browser tabs)
    }

    _setupState() {
        const gl = this.gl;
        gl.clearColor(this.clearColor[0], this.clearColor[1],
            this.clearColor[2], this.clearColor[3]);
        gl.clearDepth(this.clearDepth);
        gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
        //gl.enable(gl.DEPTH_TEST); // Enable depth testing
        gl.disable(gl.DEPTH_TEST);
        gl.depthFunc(gl.GEQUAL);  // Near things obscure far things
        gl.disable(gl.CULL_FACE);
        //gl.enable(gl.BLEND);
        gl.disable(gl.BLEND);
        gl.blendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA);
        this._initMatrices();
        this._setupViewport();
        //this._setupDepthTexture();
    }

    _initMatrices() {
        /** Create modelview and projection matrices.
         */
        this.matProjection = mat4.create();
        this.matModelView  = mat4.create();
        this.matNormal     = null; //generated when redrawing
        this.view = {
            pos:      {x:-10, y:-10, z:-100},
            scale:    {x:1,   y:1,   z:1},
            rotation: {x:20,  y:20,  z:0}, //degrees
        };
    }

    _setupViewport() {
        /** Set up viewport.
         *  Called at startup and on resize.
         */
        const gl = this.gl;

        //we need to set the size through HTML attributes, not CSS,
        //or else it blurs, because lol.
        //and apparently this gives us wrong coords so we need to add some
        //fudge factor to make it actually fit on screen.
        const rect = gl.canvas.getBoundingClientRect();
        console.log("GL rect", rect, "win", window.innerWidth, window.innerHeight);
        gl.canvas.setAttribute('width', window.innerWidth   - (rect.left + 32));
        gl.canvas.setAttribute('height', window.innerHeight - (rect.top  + 96));

        const tStart = performance.now();
        const lol = () => {
            //wait for the size to actually change.
            //XXX find a less stupid way to do this.
            //const width=gl.canvas.clientWidth, height=gl.canvas.clientHeight;
            //MDN example just uses these
            const width=gl.canvas.width, height=gl.canvas.height;
            if(width < 1 || height < 1) {
                if(performance.now() - tStart > 15000) {
                    //just in case something even more dumb happens,
                    //don't loop forever wasting CPU power.
                    console.error("GL canvas not responding to resize");
                }
                else window.requestAnimationFrame(lol);
            }
            else {
                console.log("GL viewport", width, height);
                gl.viewport(0, 0, width, height);
            }
        };
        window.requestAnimationFrame(lol);
    }

    _onResize(event) {
        this._setupViewport();
        this._setFramebufferAttachmentSizes();
        this.redraw();
    }

    _onMouseMove(event) {
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
        console.log("FOV=", this.fov, "Z=", this.zNear, this.zFar);

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

        if(this.drawFunc) {
            try {
                this.drawFunc();
            }
            catch(ex) {
                console.error("Error redrawing", ex);
            }
        }

        //now render again to our depth buffer.
        //gl.disable(gl.BLEND);
        //gl.clearColor(1.0, 1.0, 1.0, 1.0);
        //gl.bindFramebuffer(gl.FRAMEBUFFER, this._frameBuffer);
        //gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
        //gl.uniform1i(this.gx.programInfo.uniforms.useId, 1);
        //this.renderer.renderPickBuffer();
    }
}
