import {get, isPowerOf2} from '/js/Util.js';

export default class Texture {
    /** A texture that can be bound to GL.
     */
    constructor(gl) {
        this.gl = gl;
        this.texture = gl.createTexture();
        gl.bindTexture(gl.TEXTURE_2D, this.texture);

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
    }

    bind(target=null) {
        const gl = this.gl;
        if(target == null) target = gl.TEXTURE_2D;
        gl.bindTexture(target, this.texture);
        return this;
    }

    loadFromImage(url) {
        /** Load texture from image file.
         */
        const gl   = this.gl;
        this.image = new Image();

        //we do this instead of using an async method because we need to wait
        //for the onload callback which isn't async
        return new Promise((resolve, reject) => {
            this.image.onload = e => {
                this.width  = this.image.width;
                this.height = this.image.height;
                gl.bindTexture(gl.TEXTURE_2D, this.texture);
                gl.texImage2D(gl.TEXTURE_2D, this.level, this.internalFormat,
                    this.srcFormat, this.srcType, this.image);

                // WebGL1 has different requirements for power of 2 images
                // vs non power of 2 images so check if the image is a
                // power of 2 in both dimensions.
                if (isPowerOf2(this.image.width) && isPowerOf2(this.image.height)) {
                    // Yes, it's a power of 2. Generate mips.
                    gl.generateMipmap(gl.TEXTURE_2D);
                } else {
                    // No, it's not a power of 2. Turn off mips and set
                    // wrapping to clamp to edge
                    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
                    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
                    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
                }
                resolve(this);
            };
            this.image.src = url;
        });
    }
}