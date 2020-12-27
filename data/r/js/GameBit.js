import {int, getDescriptionAndNotes} from './Util.js';

class ObjRef {
    /** An object reference to a GameBit.
     */
    constructor(eRef) {
        this.map   = eRef.getAttribute('map');
        this.obj   = eRef.getAttribute('obj');
        this.objId = int(eRef.getAttribute('objId'));
        this.param = eRef.getAttribute('param');
    }
}

export default class GameBit {
    /** A "Bit" (as the game calls them) in the game's GameBit tables.
     */
    constructor(game, eBit) {
        /** Construct a GameBit from a 'bit' element from gamebits.xml.
         */
        this.game      = game;
        this.id        = int(eBit.getAttribute('id'));
        this.table     = int(eBit.getAttribute('table'));
        this.hintId    = int(eBit.getAttribute('hint'));
        this.hasHint   = eBit.getAttribute('hasHint') == '1';
        this.max       = int(eBit.getAttribute('max'));
        this.name      = eBit.getAttribute('name');
        getDescriptionAndNotes(this, eBit);

        this.objRefs = [];
        for(let eRef of eBit.getElementsByTagName('objref')) {
            this.objRefs.push(new ObjRef(eRef));
        }
    }
}
