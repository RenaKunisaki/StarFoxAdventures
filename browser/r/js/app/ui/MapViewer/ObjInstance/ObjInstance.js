import RenderBatch from "../../gl/gx/RenderBatch.js";
import Box from "../../gl/Model/Box.js";
import { hsv2rgb } from "../../../../Util.js";

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
            [x-s/2, y-s/2, z-s/2],
            [x+s/2, y+s/2, z+s/2],
        )).setId(id).batch);
        return batch;
    }
};

class Curve extends ObjInstance {
    _makeColor(entry) {
        const [r,g,b] = hsv2rgb(
            //XXX confirm max type
            (entry.params.type.value.value / 0x3F) * 360, 1, 1);
        return [r*255, g*255, b*255, 0xC0];
    }
    _renderPoint(id, obj, batch) {
        batch.addFunction(Box.fromLine(this.gx,
            [this.entry.position.x, this.entry.position.y, this.entry.position.z],
            [obj.position.x, obj.position.y, obj.position.z],
            [0.25, 0.25, 0.25])
            .setId(id).setColors([this._makeColor(obj)]).batch);
    }

    render(id) {
        const batch  = new RenderBatch(this.gx);
        const entry  = this.entry;

        //render lines between the points
        const idNext = entry.params.idNext.value.value; //ugh
        const idPrev = entry.params.idPrev.value.value;
        const id24   = entry.params._24.value.value;
        const id28   = entry.params._28.value.value;
        if(idNext > 0) {
            const next = this.map.romList.objsByUniqueId[idNext];
            if(!next) {
                console.warn("Curve's next point doesn't exist", entry);
            }
            else this._renderPoint(id, next, batch);
        }

        const others = [idPrev, id24, id28];
        for(let idOther of others) {
            if(idOther > 0) {
                const that = this.map.romList.objsByUniqueId[idOther];
                if(that && that.params.idNext.value.value != entry.id) {
                    this._renderPoint(id, that, batch);
                }
            }
        }

        //render the point
        const x = this.entry.position.x;
        const y = this.entry.position.y;
        const z = this.entry.position.z;
        const s = 5;
        batch.addFunction((new Box(this.gx,
            [x-s/2, y-s/2, z-s/2],
            [x+s/2, y+s/2, z+s/2],
        )).setId(id).setColors([this._makeColor(entry)]).batch);

        return batch;
    }
};

const Types = { //ObjDefEnum => class
    [0x6E]: Curve,
};

export default function createObjInstance(gx, game, map, entry) {
    let cls = Types[entry.defNo];
    if(!cls) cls = ObjInstance;
    return new cls(gx, game, map, entry);
};
