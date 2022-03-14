import Game from "../../../game/Game.js";
import { E, clearElement } from "../../../lib/Element.js";
import { assertType, hex } from "../../../Util.js";
import SfaTexture from "../../../game/SfaTexture.js";

export default class ImageViewer {
    constructor(game, dataView) {
        this.game    = assertType(game, Game);
        this.app     = game.app;
        this.view    = dataView;
        this.scale   = 4;
        this.eCanvas = E.canvas('imageView');
        this.element = E.div('imageviewer',
            //this._makeToolbar(),
            this.eCanvas);
        this.ctx = this.eCanvas.getContext('2d');
        this.texture = SfaTexture.fromData(dataView);
        if(!this.texture) throw new Error("Not an image file");
        console.log("decoded texture", this.texture);
        this.eCanvas.setAttribute('width',  this.texture.width*this.scale);
        this.eCanvas.setAttribute('height', this.texture.height*this.scale);
        this.refresh();
    }

    refresh() {
        this.ctx.drawImage(this.texture.image.canvas,
            0, 0, this.texture.width, this.texture.height,
            0, 0, this.texture.width*this.scale, this.texture.height*this.scale);
    }
}
