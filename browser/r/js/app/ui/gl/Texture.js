import {isPowerOf2} from '/r/js/Util.js';

export default class Texture {
    /** A texture that can be bound to GL.
     */
    constructor(context) {
        this.context = context;
        this.gl      = context.gl;
        const gl     = context.gl;
        this.texture = gl.createTexture();
        this.bind();
    }

    bind(target=null) {
        const gl = this.gl;
        if(target == null) target = gl.TEXTURE_2D;
        gl.bindTexture(target, this.texture);
        return this;
    }

    _buildMipMaps() {
        const gl = this.gl;
        // WebGL1 has different requirements for power of 2 images
        // vs non power of 2 images so check if the image is a
        // power of 2 in both dimensions.
        if (isPowerOf2(this.width) && isPowerOf2(this.height)) {
            // Yes, it's a power of 2. Generate mips.
            gl.generateMipmap(gl.TEXTURE_2D);
        } else {
            // No, it's not a power of 2. Turn off mips and set
            // wrapping to clamp to edge
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
        }
    }

    makeSolidColor(r, g, b, a) {
        /** Make a 1x1 solid-color texture.
         */
        const gl            = this.gl;
        this.level          = 0;
        this.internalFormat = gl.RGBA;
        this.width          = 1;
        this.height         = 1;
        this.border         = 0;
        this.srcFormat      = gl.RGBA;
        this.srcType        = gl.UNSIGNED_BYTE;
        const pixel         = new Uint8Array([r, g, b, a]);
        this.bind();
        gl.texImage2D(gl.TEXTURE_2D, this.level, this.internalFormat,
            this.width, this.height, this.border, this.srcFormat, this.srcType,
            pixel);
    }

    loadFromImage(url) {
        /** Load texture from image file.
         */
        const gl   = this.gl;
        this.image = new Image();

        // Because images have to be download over the internet
        // they might take a moment until they are ready.
        // Until then put a single pixel in the texture so we can
        // use it immediately. When the image has finished downloading
        // we'll update the texture with the contents of the image.
        this.level = 0;
        this.internalFormat = gl.RGBA;
        this.width     = 1;
        this.height    = 1;
        this.border    = 0;
        this.srcFormat = gl.RGBA;
        this.srcType   = gl.UNSIGNED_BYTE;
        const pixel     = new Uint8Array([255, 0, 255, 255]); // opaque magenta
        gl.texImage2D(gl.TEXTURE_2D, this.level, this.internalFormat,
            this.width, this.height, this.border, this.srcFormat, this.srcType,
            pixel);

        //we do this instead of using an async method because we need to wait
        //for the onload callback which isn't async
        console.log("Download texture:", url);
        return new Promise((resolve, reject) => {
            this.image.onload = e => {
                this.width  = this.image.width;
                this.height = this.image.height;
                this.bind();
                gl.texImage2D(gl.TEXTURE_2D, this.level, this.internalFormat,
                    this.srcFormat, this.srcType, this.image);
                this._buildMipMaps();
                resolve(this);
            };
            this.image.src = url;
        });
    }

    loadDXT1(data, width, height) {
        /** Construct from raw DDS DXT1/BC1 data.
         *  @param {ArrayBuffer} data image data.
         *  @param {integer} width image width.
         *  @param {integer} height image height.
         *  @returns {Texture} this.
         */
        const gl    = this.gl;
        const fmt   = this.context._gl_extensions.compressed_texture_s3tc;
        this.width  = width;
        this.height = height;
        this.bind();
        //data = new DataView(data.buffer, 0, 65536);
        console.log("Load DXT1 size %dx%d, %d bytes", width, height,
            data.byteLength, data);
        gl.compressedTexImage2D(gl.TEXTURE_2D, 0,
            fmt.COMPRESSED_RGBA_S3TC_DXT1_EXT, width, height, 0, data);
        return this;
    }

    loadGameTexture(tex) {
        /** Construct from game's texture asset.
         *  @param {SfaTexture} tex Texture asset to load.
         *  @returns {Texture} this.
         */
        const gl    = this.gl;
        this.width  = tex.width;
        this.height = tex.height;
        this.internalFormat = gl.RGBA;
        this.border    = 0;
        this.srcFormat = gl.RGBA;
        this.srcType   = gl.UNSIGNED_BYTE;
        this.bind();
        //console.log("load game tex", tex.image.data.data);
        gl.texImage2D(gl.TEXTURE_2D, this.level, this.internalFormat,
            this.width, this.height, this.border, this.srcFormat, this.srcType,
            tex.image.data);
        this._buildMipMaps();
        return this;
    }
}
