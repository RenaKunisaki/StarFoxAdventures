const RADIANS = Math.PI / 180;

function CHECK_ERROR(gl) {
    const err = gl.getError();
    console.assert(!err);
}

export default class Context {
    /** GL context wrapper.
     */
    constructor(canvas, drawFunc=null) {
        /** Construct GL context.
         *  @param {HTMLCanvasElement} canvas The canvas to render to.
         *  @param {function} drawFunc Function to call to render the scene.
         */
        this.fov   = 60; //degrees
        this.zNear = 1;
        this.zFar  = 50000;
        this.clearColor = [0.0, 0.5, 0.5, 1.0];
        this.clearDepth = 1.0; //game uses lower = nearer
        this.drawFunc   = drawFunc; //callback to draw the scene
        //XXX add getters/setters for these
        this._gl_extensions = {};
        this.enableTextures = true;
        this.useWireframe = false; //render in wireframe
        this.enableBackfaceCulling = true;
        this.frontFaceCW = true; //game seems to use this order
        this.useSRT = true; //use SRT transform order, not TSR
            //SRT = the rotation is around the camera position (ideal
            //for map viewing)
            //TSR = the rotation is around the origin point (ideal
            //for character model viewing)
        this.showPickBuffer = false; //debug: render the picker buffer

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
        this.gl = canvas.getContext("webgl2", {preserveDrawingBuffer: true});

        if(this.gl === null) {
            alert("Unable to initialize WebGL");
            throw new Error("GL init failed (maybe not supported on this browser?)");
        }
        console.log("GL init OK");

        canvas.addEventListener('resize', e => this._onResize(e));
        this._setupViewport();

        if(canvas.getAttribute('tabindex') == null) {
            //allow the canvas to receive focus for key input
            canvas.setAttribute('tabindex', '1');
        }
        CHECK_ERROR(this.gl);
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

        //allow u32 index buffers
        this._gl_extensions.OES_element_index_uint = gl.getExtension(
            "OES_element_index_uint");
    }

    _setupState() {
        const gl = this.gl;
        gl.clearColor(this.clearColor[0], this.clearColor[1],
            this.clearColor[2], this.clearColor[3]);
        gl.clearDepth(this.clearDepth);
        gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
        gl.enable(gl.DEPTH_TEST); // Enable depth testing
        gl.depthFunc(gl.GEQUAL);  // lower depth value = nearer to camera
        gl.enable(gl.BLEND);
        gl.blendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA);
        this._initMatrices();
        CHECK_ERROR(this.gl);
        this._setupViewport();
        CHECK_ERROR(this.gl);
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
        CHECK_ERROR(this.gl);

        // create a depth renderbuffer
        this._depthBuffer = gl.createRenderbuffer();
        gl.bindRenderbuffer(gl.RENDERBUFFER, this._depthBuffer);
        CHECK_ERROR(this.gl);

        this._setFramebufferAttachmentSizes();

        // Create and bind the framebuffer
        this._frameBuffer = gl.createFramebuffer();
        gl.bindFramebuffer(gl.FRAMEBUFFER, this._frameBuffer);
        CHECK_ERROR(this.gl);

        // attach the texture as the first color attachment
        const attachmentPoint = gl.COLOR_ATTACHMENT0;
        const level = 0;
        gl.framebufferTexture2D(gl.FRAMEBUFFER, attachmentPoint, gl.TEXTURE_2D,
            this._targetTexture, level);
        CHECK_ERROR(this.gl);

        // make a depth buffer and the same size as the targetTexture
        gl.framebufferRenderbuffer(gl.FRAMEBUFFER, gl.DEPTH_ATTACHMENT,
            gl.RENDERBUFFER, this._depthBuffer);
        CHECK_ERROR(this.gl);
        console.log("Created depth texture", this._depthBuffer);
    }

    _setFramebufferAttachmentSizes() {
        /** Set up framebuffer after window is resized.
         *  Required for depth buffer picker.
         */
        const gl = this.gl;
        const width=gl.drawingBufferWidth, height=gl.drawingBufferHeight;
        if(width < 1 || height < 1) return;

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
        CHECK_ERROR(this.gl);
        console.log("Setup pick buffer, size", width, height);
    }

    async readPickBuffer(x, y) {
        /** Read pick buffer.
         *  @param {integer} x X coordinate to read.
         *  @param {integer} y Y coordinate to read.
         *  @returns {integer} Value from pick buffer.
         */
        const gl     = this.gl;
        const data   = new Uint8Array(4);
        const rect   = gl.canvas.getBoundingClientRect();
        const mouseX = Math.round(x - rect.left);
        const mouseY = Math.round(y - rect.top);
        const pixelX = Math.trunc(mouseX * (gl.canvas.width / gl.canvas.clientWidth));
        const pixelY = Math.trunc((gl.canvas.height - mouseY) * (gl.canvas.height / gl.canvas.clientHeight));
        await this._drawPicker();
        gl.readBuffer(gl.COLOR_ATTACHMENT0);
        gl.readPixels(
            pixelX, pixelY, 1, 1, // x, y, width, height
            gl.RGBA,              // format
            gl.UNSIGNED_BYTE,     // type
            data);                // typed array to hold result
        console.log("Read picker", x, y, "=>", pixelX, pixelY, "data", data,
            "rect", rect, "client", gl.canvas.clientWidth, gl.canvas.clientHeight,
            "wh", gl.canvas.width, gl.canvas.height);
        return data[3] | (data[2] << 8) | (data[1] << 16) | (data[0] << 24);
        //return data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
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
            rotation: {x:20,  y:120, z:0}, //degrees
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
        gl.canvas.setAttribute('width', window.innerWidth   - (rect.left +  2));
        gl.canvas.setAttribute('height', window.innerHeight - (rect.top  + 80));

        const tStart = performance.now();
        const lol = () => {
            //wait for the size to actually change, ie the canvas to actually
            //be added to the document and the layout recalculated.
            //XXX find a less stupid way to do this.
            //const width=gl.canvas.clientWidth, height=gl.canvas.clientHeight;
            //MDN example just uses these
            //const width=gl.canvas.width, height=gl.canvas.height;
            //but apparently it's better to use these in case the GPU has
            //a small size limit
            const width=gl.drawingBufferWidth, height=gl.drawingBufferHeight;
            if(width < 1 || height < 1) {
                if(performance.now() - tStart > 15000) {
                    //just in case something even more dumb happens,
                    //don't loop forever wasting CPU power.
                    console.error("GL canvas not responding to resize");
                }
                else window.requestAnimationFrame(lol);
            }
            else {
                console.log("GL: setting viewport size:", width, height);
                gl.viewport(0, 0, width, height);
                CHECK_ERROR(this.gl);
                this._setupDepthTexture();
                CHECK_ERROR(this.gl);
            }
        };
        window.requestAnimationFrame(lol);
    }

    _onResize(event) {
        console.log("resize event");
        this._setupViewport();
        this._setFramebufferAttachmentSizes();
        this.redraw();
    }

    resetStats() {
        this.stats = {
            nOps:           0, //total operations
            nVtxs:          0, //total vertices drawn
            nPolys:         0, //total polygons drawn
            nBufferUploads: 0, //total buffer data ops
            nBufferBytes:   0, //total buffer data size uploaded
            nBufferSwaps:   0, //# times buffer binding changed
            timeBind:       0, //msec spent binding buffers
            timeUpload:     0, //msec spent uploading buffers
            timeDraw:       0, //msec spent doing draw ops
            geomBounds: {
                xMin: 999999999, xMax: -999999999,
                yMin: 999999999, yMax: -999999999,
                zMin: 999999999, zMax: -999999999,
            },
            renderStartTime: performance.now(),
        };
    }

    redraw() {
        /** Redraw the scene.
         */
        if(this._pendingDraw) return;
        this._pendingDraw = true;
        window.requestAnimationFrame(() => {
            this._pendingDraw = false;
            this._redraw();
        });
    }

    async _redraw() {
        this.resetStats();
        await this._drawScreen();
        this.stats.renderTime = performance.now() - this.stats.renderStartTime;
    }

    async _drawScreen() {
        /** Draw the visible scene. */
        const gl = this.gl;
        this._setupFrame();
        gl.enable(gl.BLEND);
        gl.clearColor(this.clearColor[0], this.clearColor[1],
            this.clearColor[2], this.clearColor[3]);
        gl.clearDepth(this.clearDepth);
        gl.bindFramebuffer(gl.FRAMEBUFFER, null);
        gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

        if(this.drawFunc) {
            try {
                await this.drawFunc(false);
            }
            catch(ex) {
                console.error("Error redrawing", ex);
            }
        }
        gl.flush();
    }

    async _drawPicker() {
        /** Draw the picker buffer. */
        const gl = this.gl;
        this._setupFrame();
        gl.disable(gl.BLEND);
        gl.clearColor(1.0, 1.0, 1.0, 1.0);
        if(!this.showPickBuffer) {
            gl.bindFramebuffer(gl.FRAMEBUFFER, this._frameBuffer);
        }
        gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
        if(this.drawFunc) {
            try {
                await this.drawFunc(true);
            }
            catch(ex) {
                console.error("Error drawing pick buffer", ex);
            }
        }
        gl.flush();
    }

    _setupFrame() {
        /** Prepare to render a frame. */
        const gl = this.gl;
        const P  = this.view.pos;
        const S  = this.view.scale;
        const R  = this.view.rotation;
        const aspect = gl.canvas.clientWidth / gl.canvas.clientHeight;

        //ensure canvas size matches displayed size
        const width  = gl.canvas.clientWidth;
        const height = gl.canvas.clientHeight;
        if(gl.canvas.width != width || gl.canvas.height != height) {
            gl.canvas.width = width;
            gl.canvas.height = height;
        }
        gl.viewport(0, 0, gl.drawingBufferWidth, gl.drawingBufferHeight);

        //set up projection matrix
        mat4.perspective(this.matProjection, //destination matrix
            this.fov * RADIANS, //to radians
            aspect, this.zNear, this.zFar);
        //console.log("FOV=", this.fov, "Z=", this.zNear, this.zFar);

        //set up modelview matrix
        //it can help to think of this as moving the "drawing position"
        //rather than thew camera.
        if(this.useSRT) {
            mat4.scale(this.matModelView, mat4.create(),
                [S.x, S.y, S.z]);
            mat4.rotateX(this.matModelView, this.matModelView, R.x * RADIANS);
            mat4.rotateY(this.matModelView, this.matModelView, R.y * RADIANS);
            mat4.rotateZ(this.matModelView, this.matModelView, R.z * RADIANS);
            mat4.translate(this.matModelView, // destination matrix
                this.matModelView,    // matrix to translate
                [-P.x, -P.y, -P.z]);  // amount to translate
        }
        else {
            mat4.translate(this.matModelView, // destination matrix
                mat4.create(),       // matrix to translate
                [-P.x, -P.y, -P.z]); // amount to translate
            mat4.scale(this.matModelView, this.matModelView,
                [S.x, S.y, S.z]);
            mat4.rotateX(this.matModelView, this.matModelView, R.x * RADIANS);
            mat4.rotateY(this.matModelView, this.matModelView, R.y * RADIANS);
            mat4.rotateZ(this.matModelView, this.matModelView, R.z * RADIANS);

        }

        //set up normal matrix, for lighting.
        this.matNormal = mat4.create();
        mat4.invert(this.matNormal, this.matModelView);
        mat4.transpose(this.matNormal, this.matNormal);

        gl.frontFace(this.frontFaceCW ? gl.CW : gl.CCW);
        //our code overrides this, so this is a waste
        //if(this.enableBackfaceCulling) gl.enable(gl.CULL_FACE);
        //else gl.disable(gl.CULL_FACE);
    }
}
