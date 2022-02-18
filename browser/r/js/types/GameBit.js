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
        this.hint   = [];
        this.notes  = [];
        this.description = null;
        //getDescriptionAndNotes(this, eBit);

        let desc = eBit.getElementsByTagName('description').item(0);
        if(desc != null) this.description = desc.textContent;

        let hint = eBit.getElementsByTagName('hint').item(0);
        if(hint != null) {
            for(let phrase of hint.getElementsByTagName('phrase')) {
                this.hint.push(phrase.textContent);
            }
        }

        for(let note of eBit.getElementsByTagName('note')) {
            this.notes.push(note.textContent);
        }

        //this.objRefs = [];
        //for(let eRef of eBit.getElementsByTagName('objref')) {
        //    this.objRefs.push(new ObjRef(eRef));
        //}
    }
}
