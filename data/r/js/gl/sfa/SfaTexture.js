import Texture from '/r/js/gl/Texture.js';

export default class SfaTexture extends Texture {
    /** A Texture loaded from SFA.
     *  Functions the same as a normal Texture but records
     *  additional information from the game.
     */
    constructor(context) {
        super(context);
        this.format  = null;
        this.nFrames = null;
        this.id      = null;
        this.offset  = null;
        this.group   = null; //0 (TEX0) or 1 (TEX1)
    }
}
