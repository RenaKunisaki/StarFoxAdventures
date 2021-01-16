import {int, getDescriptionAndNotes} from './Util.js';

export default class ObjCategory {
    /** A category for a GameObject.
     */
    constructor(game, eCat) {
        /** Construct an ObjCategory from a 'cat' element from objcats.xml.
         */
        this.game = game;
        this.id   = int(eCat.getAttribute('id'));
        this.name = eCat.getAttribute('name');
        getDescriptionAndNotes(this, eCat);
    }
}
