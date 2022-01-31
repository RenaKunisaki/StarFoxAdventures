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
        this.game   = game;
        this.id     = int(eBit.getAttribute('id'));
        this.table  = int(eBit.getAttribute('table'));
        this.hintId = int(eBit.getAttribute('hintid'));
        this.size   = int(eBit.getAttribute('size'));
        this.offset = int(eBit.getAttribute('offset'));
        this.name   = eBit.getAttribute('name');
        this.hint   = null;
        getDescriptionAndNotes(this, eBit);

        let hint = eBit.getElementsByTagName('hint').item(0);
        if(hint != null) {
            this.hint = [];
            for(let phrase of hint.getElementsByTagName('phrase')) {
                this.hint.push(phrase.textContent);
            }
        }

        this.objRefs = [];
        for(let eRef of eBit.getElementsByTagName('objref')) {
            this.objRefs.push(new ObjRef(eRef));
        }
    }
}
