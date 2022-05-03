import RenderBatch from "../../gl/gx/RenderBatch.js";
import Box from "../../gl/Model/Box.js";

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
};

