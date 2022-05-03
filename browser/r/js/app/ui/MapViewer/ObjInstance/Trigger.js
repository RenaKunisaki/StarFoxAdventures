import { ObjInstance } from "./ObjInstance.js";
import RenderBatch from "../../gl/gx/RenderBatch.js";
import Box from "../../gl/Model/Box.js";
import Sphere from "../../gl/Model/Sphere.js";
import Cylinder from "../../gl/Model/Cylinder.js";

//all of these params need verification by comparing
//how the game actually handles them. just reading the
//code isn't perfectly clear.

function rot2rad(n) {
    /** Convert rotation value (0-65535) to radians.
     *  @param {integer} n Rotation value.
     *  @returns {float} Rotation in radians.
     */
    return (n/65535) * (Math.PI * 2);
}


export class TrigPnt extends ObjInstance {
    render(id) {
        const batch  = new RenderBatch(this.gx);
        const entry  = this.entry;

        //render the point
        //size1 is the size of the point (divided by 2)
        //setting the sphere's scale sets diameter rather than
        //radius so no need to multiply it
        const x = this.entry.position.x;
        const y = this.entry.position.y;
        const z = this.entry.position.z;
        const s = entry.params.size.value.value[0];
        batch.addFunction(
            (new Sphere(this.gx, [x,y,z])).setScale(s).setId(id).setColor(
                [0xFF, 0x40, 0x40, 0x80]).batch);
        return batch;
    }
}
export class TrigCyl extends ObjInstance {
    render(id) {
        const batch  = new RenderBatch(this.gx);
        const entry  = this.entry;

        //size1 = radius (or diameter?) divided by 2
        //size2 = unused? set, but not read
        //size3 = height
        const x = this.entry.position.x;
        const y = this.entry.position.y;
        const z = this.entry.position.z;
        const r = entry.params.size.value.value[0];
        //const r = entry.params.size.value.value[1];
        const h = entry.params.size.value.value[2];
        batch.addFunction((new Cylinder(this.gx, [x,y,z],
        )).setScale(r,h/2,r).setId(id).setColor(
            [0x40, 0x40, 0xFF, 0x80]).setRot(
            rot2rad(entry.params.rot.value.value[0] << 8),
            rot2rad(entry.params.rot.value.value[1] << 8), 0).batch);
        return batch;
    }
}
export class TrigPln extends ObjInstance {
    render(id) {
        const batch  = new RenderBatch(this.gx);
        const entry  = this.entry;

        //probably still wrong...
        //scale looks about right?
        const x  = this.entry.position.x;
        const y  = this.entry.position.y;
        const z  = this.entry.position.z;
        const s  = entry.params.size.value.value[0] * 6.25 * 2 * entry.object.scale;
        const rx = rot2rad((entry.params.rot.value.value[0] & 0x3F) << 10);
        const ry = rot2rad(entry.params.rot.value.value[1] << 8);
        batch.addFunction((new Box(this.gx,
            [-0.5, -0.5, -0.5],
            [ 0.5,  0.5,  0.5],
        )).setScale(s,s,1).setRot(0,rx,ry).setPos(x,y,z).setId(id).setColors(
            [0x40, 0xFF, 0x40, 0x80]).batch);
        return batch;
    }
}
export class TrigArea extends ObjInstance {
    render(id) {
        const batch  = new RenderBatch(this.gx);
        const entry  = this.entry;

        //size1, size2, size3 define the area size
        //rotX, rotY define its orientation?
        const x = this.entry.position.x;
        const y = this.entry.position.y;
        const z = this.entry.position.z;
        const sx = entry.params.size.value.value[0] * 2;
        const sy = entry.params.size.value.value[1] * 2;
        const sz = entry.params.size.value.value[2] * 2;
        batch.addFunction((new Box(this.gx,
            [-0.5, -0.5, -0.5],
            [ 0.5,  0.5,  0.5],
        )).setScale(sx,sy,sz).setPos(x,y,z).setId(id).setColors(
            [0x40, 0x40, 0xFF, 0x80]).setRot(
            rot2rad(entry.params.rot.value.value[0] << 8),
            rot2rad(entry.params.rot.value.value[1] << 8), 0).batch);
        return batch;
    }
}
export class TrigTime extends ObjInstance {
    render(id) {
        const batch  = new RenderBatch(this.gx);
        const x = this.entry.position.x;
        const y = this.entry.position.y;
        const z = this.entry.position.z;
        const s = 10;
        batch.addFunction((new Box(this.gx,
            [-0.5, -0.5, -0.5],
            [ 0.5,  0.5,  0.5],
        )).setScale(s).setPos(x,y,z).setId(id).setColors(
            [0x40, 0xFF, 0xFF, 0x80]).batch);
        return batch;
    }
}
export class TrigButt extends ObjInstance {
    render(id) {
        const batch  = new RenderBatch(this.gx);
        const x = this.entry.position.x;
        const y = this.entry.position.y;
        const z = this.entry.position.z;
        const s = 10;
        batch.addFunction((new Box(this.gx,
            [-0.5, -0.5, -0.5],
            [ 0.5,  0.5,  0.5],
        )).setScale(s).setPos(x,y,z).setId(id).setColors(
            [0xFF, 0x40, 0xFF, 0x80]).batch);
        return batch;
    }
}
export class TriggSetp extends ObjInstance { //this one has extra g
    render(id) {
        const batch  = new RenderBatch(this.gx);
        const x = this.entry.position.x;
        const y = this.entry.position.y;
        const z = this.entry.position.z;
        const s = 10;
        batch.addFunction((new Box(this.gx,
            [-0.5, -0.5, -0.5],
            [ 0.5,  0.5,  0.5],
        )).setScale(s).setPos(x,y,z).setId(id).setColors(
            [0xFF, 0xFF, 0x40, 0x80]).batch);
        return batch;
    }
}
export class TrigBits extends ObjInstance {
    render(id) {
        const batch  = new RenderBatch(this.gx);
        const x = this.entry.position.x;
        const y = this.entry.position.y;
        const z = this.entry.position.z;
        const s = 10;
        batch.addFunction((new Box(this.gx,
            [-0.5, -0.5, -0.5],
            [ 0.5,  0.5,  0.5],
        )).setScale(s).setPos(x,y,z).setId(id).setColors(
            [0xFF, 0x80, 0x40, 0x80]).batch);
        return batch;
    }
}
export class TrigCrve extends ObjInstance {
    render(id) {
        const batch  = new RenderBatch(this.gx);
        const x = this.entry.position.x;
        const y = this.entry.position.y;
        const z = this.entry.position.z;
        const s = 10;
        batch.addFunction((new Box(this.gx,
            [-0.5, -0.5, -0.5],
            [ 0.5,  0.5,  0.5],
        )).setScale(s).setPos(x,y,z).setId(id).setColors(
            [0x40, 0x80, 0xFF, 0x80]).batch);
        return batch;
    }
}
