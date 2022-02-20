import { hex, int } from "../Util.js";
import { E } from "../lib/Element.js";

export default class GameBit {
    /** A "Bit" (as the game calls them) in the game's GameBit tables.
     */
    constructor(app, eBit) {
        /** Construct a GameBit from a 'bit' element from gamebits.xml.
         */
        this.app     = app;
        this.id      = int(eBit.getAttribute('id'));
        this.table   = int(eBit.getAttribute('table'));
        this.hintId  = int(eBit.getAttribute('hintid'));
        this.size    = int(eBit.getAttribute('size'));
        this.offset  = int(eBit.getAttribute('offset')); //in bits
        this.name    = eBit.getAttribute('name');
        this.hint    = [];
        this.notes   = [];
        this.objRefs = [];
        this.description = null;

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

        for(let eRef of eBit.getElementsByTagName('objref')) {
            this.objRefs.push({
                map:   eRef.getAttribute('map'),
                obj:   eRef.getAttribute('obj'),
                objId: int(eRef.getAttribute('objId')),
                param: eRef.getAttribute('param'),
            });
        }
    }

    toXml() {
        /** Construct XML element representing this Bit.
         *  @returns {Element} XML element.
         */
        const elem = E.bit({
            id:     `0x${hex(this.id,4)}`,
            table:  this.table,
            size:   this.size,
            offset: `0x${hex(this.offset,4)}`,
        });
        if(this.name != null) elem.setAttribute('name', this.name);
        if(this.hintId != null) elem.setAttribute('hintid', this.hintId);
        if(this.description) elem.append(E.description(null, this.description));

        for(let note of this.notes) {
            elem.append(E.note(null, note));
        }

        for(let eRef of this.objRefs) {
            elem.append(E.objref(null, {
                map:   eRef.map,
                obj:   eRef.obj,
                objId: `0x${hex(eRef.objId,8)}`,
                param: eRef.param,
            }));
        }

        return elem;
    }
}
