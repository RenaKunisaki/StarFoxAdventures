import { assertType, hex, int } from "../Util.js";
import { E } from "../lib/Element.js";
import Game from "./Game.js";
import App from "../app/App.js";

export default class GameBit {
    /** A "Bit" (as the game calls them) in the game's GameBit tables.
     */
    constructor(game, eBit=null) {
        /** Construct a GameBit from a 'bit' element from gamebits.xml.
         */
        //we don't use fromXml() because we want to instantiate directly
        //using app._getXml()
        //XXX _getXml() passes App, not Game
        if(game instanceof App) game = game.game;
        this.game    = assertType(game, Game);
        this.app     = game.app;
        this.id      = eBit ? int(eBit.getAttribute('id')) : null;
        this.table   = eBit ? int(eBit.getAttribute('table')) : null;
        this.hintId  = eBit ? int(eBit.getAttribute('hintid')) : null;
        this.size    = eBit ? int(eBit.getAttribute('size')) : null;
        this.offset  = eBit ? int(eBit.getAttribute('offset')) : null; //in bits
        this.name    = eBit ? eBit.getAttribute('name') : null;
        this.hint    = null;
        this.notes   = [];
        this.objRefs = [];
        this.description = null;

        if(eBit) {
            let desc = eBit.getElementsByTagName('description').item(0);
            if(desc != null) this.description = desc.textContent;

            for(let note of eBit.getElementsByTagName('note')) {
                this.notes.push(note.textContent);
            }

            for(let eRef of eBit.getElementsByTagName('objref')) {
                this.objRefs.push({
                    map:   eRef.getAttribute('map'),
                    obj:   eRef.getAttribute('obj'),
                    objId: int(eRef.getAttribute('objid')),
                    param: eRef.getAttribute('param'),
                });
            }
        }

        if(this.hintId != undefined && this.game.texts) {
            try {
                this.hint = this.game.texts[this.hintId]
                    .phrases[0].toString(false);
            }
            catch(ex) {
                this.hint = ex.toString();
            }
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

        for(let objref of this.objRefs) {
            elem.append(E.objref(null, {
                map:   objref.map,
                obj:   objref.obj,
                objid: `0x${hex(objref.objId,8)}`,
                param: objref.param,
            }));
        }

        return elem;
    }
}
