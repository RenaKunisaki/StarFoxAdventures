import {int, hex, getDescriptionAndNotes} from './Util.js';

export default class Model {
    /** A 3D model in the game.
     */
    constructor(game, eModel) {
        /** Construct a Model from a 'model' element from models.xml.
         */
        this.game = game;
        this.id   = int(eModel.getAttribute('id'));
        this.type = eModel.getAttribute('type');
        getDescriptionAndNotes(this, eModel);
    }
}
