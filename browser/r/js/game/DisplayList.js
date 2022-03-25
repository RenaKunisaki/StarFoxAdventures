import { assertType } from "../Util.js";
import Game from "./Game.js";

let DisplayListPtr = null;

export default class DisplayList {
    /** Display list pointed to by DisplayListPtr */
    constructor(game, view, offset) {
        /** Construct DisplayList.
         *  @param {Game} game The game this belongs to.
         *  @param {DataView} view The view to read from.
         *  @param {int} offset Offset to read the DisplayListPtr from.
         */
        assertType(game, Game);
        assertType(view, DataView);
        this.game = game;
        if(!DisplayListPtr) DisplayListPtr =
            game.app.types.getType('sfa.maps.DisplayListPtr');

        const ptr = DisplayListPtr.fromBytes(view, offset);
        this.bbox = ptr.bbox;
        this.shaderId = ptr.shaderId;
        this.specialBitAddr = ptr.specialBitAddr;
        this.unk12 = ptr._12;
        this.unk16 = ptr._16;
        this.unk18 = ptr._18;
        this.data  = view.buffer.slice(view.byteOffset+ptr.list,
            view.byteOffset+ptr.list+ptr.size);
    }
}
