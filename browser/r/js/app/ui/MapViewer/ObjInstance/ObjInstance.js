import RenderBatch from "../../gl/gx/RenderBatch.js";
import Box from "../../gl/Model/Box.js";
import { E } from "../../../../lib/Element.js";

export class ObjInstance {
    /** An instance of an object in a map. */
    constructor(gx, game, map, romListEntry) {
        /** Construct ObjInstance.
         *  @param {GX} gx The GX instance to use for rendering.
         *  @param {Game} game The game instance this belongs to.
         *  @param {Map} map The game map this belongs to.
         *  @param {RomListEntry} romListEntry The romlist entry that
         *   defines this object instance.
         */
        this.game  = game;
        this.map   = map;
        this.entry = romListEntry;
        this.gx    = gx;
        this.gl    = gx.gl;
    }

    render(id) {
        /** Render the object.
         *  @param {integer} id The picker ID to set.
         *  @returns {RenderBatch} A batch that renders this object at
         *   its set position.
         */
        const x = this.entry.position.x;
        const y = this.entry.position.y;
        const z = this.entry.position.z;
        const s = Math.max(this.entry.object.scale, 10);

        const batch = new RenderBatch(this.gx);
        batch.addFunction((new Box(this.gx,
            [-0.5, -0.5, -0.5],
            [ 0.5,  0.5,  0.5],
        )).setScale(s).setPos(x,y,z).setId(id).batch);
        return batch;
    }

    decodeParams() {
        /** Turn romlist parameters into human-readable HTML.
         *  @returns {object} Dict of name => element.
         */
        const result = {};
        if(!this.entry.params) return result;
        for(const [name, param] of Object.entries(this.entry.params)) {
            let disp = param.value.display;
            //what the fuck
            const tp = param.value.param ? param.value.param.type : '';
            let val = param.value.value;

            //XXX this is gross and doesn't handle arrays or structs
            if(tp == 'ObjUniqueId') {
                const idTarget = val;
                if(idTarget > 0) {
                    const target = this.map.romList.
                        objsByUniqueId[idTarget];
                    if(target) {
                        disp = E.a('objlink', disp);
                        //XXX connect event handler to show that object
                    }
                    else disp += ' (not found)';
                }
            }
            else if(tp == 'GameBit' || tp == 'GameBit16' || tp == 'GameBit32') {
                const bit = this.game.bits[val];
                if(bit && bit.name) disp += ' '+bit.name;
            }
            else if(Array.isArray(val)) {
                disp = val.join(', ');
            }
            result[name] = E.span(null, {title:tp}, disp);
        }
        return result;
    }
};

