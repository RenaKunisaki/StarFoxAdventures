import { int } from "../Util.js";

export default class GameBit {
    /** A "Bit" (as the game calls them) in the game's GameBit tables.
     */
    constructor(app, eBit) {
        /** Construct a GameBit from a 'bit' element from gamebits.xml.
         */
        this.app    = app;
        this.id     = int(eBit.getAttribute('id'));
        this.table  = int(eBit.getAttribute('table'));
        this.hintId = int(eBit.getAttribute('hintid'));
        this.size   = int(eBit.getAttribute('size'));
        this.offset = int(eBit.getAttribute('offset')); //in bits
        this.name   = eBit.getAttribute('name');
        this.hint   = null;
        //getDescriptionAndNotes(this, eBit);

        let hint = eBit.getElementsByTagName('hint').item(0);
        if(hint != null) {
            this.hint = [];
            for(let phrase of hint.getElementsByTagName('phrase')) {
                this.hint.push(phrase.textContent);
            }
        }

        //this.objRefs = [];
        //for(let eRef of eBit.getElementsByTagName('objref')) {
        //    this.objRefs.push(new ObjRef(eRef));
        //}
    }
}
