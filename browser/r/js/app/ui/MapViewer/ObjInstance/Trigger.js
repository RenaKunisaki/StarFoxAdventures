import { ObjInstance } from "./ObjInstance.js";
import RenderBatch from "../../gl/gx/RenderBatch.js";
import Box from "../../gl/Model/Box.js";
import Sphere from "../../gl/Model/Sphere.js";
import Cylinder from "../../gl/Model/Cylinder.js";
import { E } from "../../../../lib/Element.js";
import { hex } from "../../../../Util.js";

//all of these params need verification by comparing
//how the game actually handles them. just reading the
//code isn't perfectly clear.

//struct types
let ObjSeqCmdEnum;

function rot2rad(n) {
    /** Convert rotation value (0-65535) to radians.
     *  @param {integer} n Rotation value.
     *  @returns {float} Rotation in radians.
     */
    return (n/65535) * (Math.PI * 2);
}

export class Trigger extends ObjInstance {
    decodeParams() {
        /** Turn romlist parameters into human-readable HTML. */
        ObjSeqCmdEnum = this.game.app.types.getType('sfa.seq.ObjSeqCmdEnum');
        let result = super.decodeParams();
        const seq = [];
        for(let iCmd=0; iCmd<8; iCmd++) {
            const cmd = this.entry.params.seq.value.value[iCmd];
            if(cmd.cmd == 0) continue;

            let conditions = [];
            const flags = cmd.flags;
            if(flags & 0x01) { //trigger when inside
                //04: reactivate when we leave
                if(flags & 0x10) conditions.push("WhileIn");
                else if(flags & 0x04) conditions.push("OnEnter");
                else conditions.push("On1stEnter");
            }
            if(flags & 0x02) { //trigger when outside
                //08: reactivate when we enter
                if(flags & 0x10) conditions.push("WhileOut");
                else if(flags & 0x08) conditions.push("OnLeave");
                else conditions.push("On1stLeave");
            }
            //if neither enter/leave are set it triggers every frame
            if(flags & 0x1C == 0x10) conditions.push("Always");
            if(!conditions.length) {
                if(flags & 0x04) conditions.push("?Enter");
                else if(flags & 0x08) conditions.push("?Leave");
                else conditions.push("Never");
            }
            if(flags & 0xE0) {
                conditions.push(`Unk${hex(cmd.flags,2)}`);
            }
            conditions = conditions.join(', ');

            let name = ObjSeqCmdEnum.valueToString(cmd.cmd);
            if(name == undefined) name = `unk${hex(cmd.cmd,2)}`;
            let params;
            switch(cmd.cmd) {
                case 0x01: { //player subcmd
                    name = "Player";
                    switch(cmd.param1) {
                        case 0x08: params = "Respawn"; break;
                        default: params = `Unk${hex(cmd.param1,2)}${hex(cmd.param2,2)}`;
                    }
                    break;
                }
                case 0x08: { //set env params
                    params = cmd.param2 ? 'On' : 'Off';
                    switch(cmd.param1) {
                        case 0x00: name = "CloudsAndLights"; break;
                        case 0x01: name = "NoAntiAlias"; break;
                        case 0x02: name = "Lights"; break;
                        case 0x03: name = "Nothing"; break; //deleted cloudaction func
                        case 0x04: name = "Unk04"; break; //DLL_0D.func05
                        case 0x05: {
                            if(cmd.param2) name = "NOP";
                            else name = "ClearFootprints";
                            params = '';
                            break;
                        }
                        case 0x06: name = "HideMoon"; break;
                        case 0x07: name = "Unk07"; break;
                        case 0x08: name = "HeatFx"; break;
                        case 0x09: case 0x0A: case 0x0B: {
                            name = 'SkyUnk_';
                            switch(cmd.param1) {
                                case 0x09: name += 'Toggle'; break;
                                case 0x0A: name += 'Off'; break;
                                case 0x0B: name += 'On'; break;
                            }
                            params = `0x${hex(param2,2)}`;
                            break;
                        }
                        default: name = `Unk${hex(cmd.param1,2)}`;
                    }
                    break;
                }
                case 0x0B: { //subcmd, relates tp seq
                    switch(cmd.param1) {
                        case 0: {
                            name = "ActivateTrigger";
                            params = `0x${hex(cmd.param2,2)}`;
                            break;
                        }
                        case 1: case 2: {
                            name = "ObjSeqBool_";
                            if(cmd.param1 == 1) name += 'Set';
                            else name += 'Clear;'
                            break;
                        }
                    }
                    break;
                }
                case 0x10: { //switch character model
                    if(cmd.param1 < this.game.charNames.length) {
                        params = this.game.charNames[cmd.param1];
                    }
                    else params = `0x${hex(cmd.param1,2)}`;
                    break;
                }
                case 0x12: { //change gamebit
                    const bitNo = ((cmd.param1 << 8) | cmd.param2) & 0x3FFF;
                    const op  = (cmd.param1 >> 6);
                    switch(op) {
                        case 0:  name = 'GameBit_Clear';  break;
                        case 1:  name = 'GameBit_Set';    break;
                        case 2:  name = 'GameBit_Invert'; break;
                        default: name = 'GameBit_Nop';    break;
                    }
                    params = `0x${hex(bitNo,4)}`;
                    const bit = this.game.bits[bitNo];
                    if(bit && bit.name) params += ' '+bit.name;
                    break;
                }
                case 0x13: case 0x14: { //show obj group, hide obj group
                    params = `${cmd.param2}`;
                    break;
                }
                case 0x1A: case 0x1B: { //show/hide in other map
                    params = `${this.game.getMapDirName(cmd.param2)} ${cmd.param1}`;
                    break;
                }
                case 0x1C: { //env cmd
                    params = cmd.param2 ? "On" : "Off";
                    switch(cmd.param1) {
                        case 0: name = "EnvBit 3AB"; break;
                        case 1: name = "EnvBit 3AC"; break;
                        case 2: name = "EnvBit 3AF"; break;
                        case 3: {
                            params = '';
                            switch(cmd.param2) {
                                case 0: name = "Env Outdoor"; break;
                                case 1: name = "Env Indoor"; break;
                                case 2: name = "Env Outdoor2"; break;
                                default: name = `Env Unk03${hex(cmd.param2,2)}`;
                            }
                            break;
                        }
                        default:
                            name   = `Env Unk${hex(cmd.param1,2)}`;
                            params = hex(cmd.param2,2);
                    }
                    break;
                }
                case 0x1D: { //enable/disable the dinosaur horn, fireflies, feeding Tricky
                    params = cmd.param1 ? 'Enabled' : 'Disabled';
                    break;
                }
                case 0x1E: { //set map act for other map
                    params = `${this.game.getMapDirName(cmd.param2)} ${cmd.param1}`;
                    break;
                }
                case 0x1F: { //save/restore player position
                    params = (cmd.param2 & 1) ? "Restore" : "Save";
                    params += `; 0x${hex(cmd.param2 & 0xFE,2)}`;
                    break;
                }
                case 0x20: { //change map layer
                    switch(cmd.param1) {
                        case 0: params = '+1'; break;
                        case 1: params = '-1'; break;
                        default: params = `? ${cmd.param1}`;
                    }
                    break;
                }
                case 0x21: { //toggle bit in GameBit
                    const bitNo = ((cmd.param1 << 8) | cmd.param2) & 0x1FFF;
                    const idx   = cmd.param1 >> 5;
                    params = `0x${hex(bitNo,4)}`;
                    const bit = this.game.bits[bitNo];
                    if(bit && bit.name) params += ' '+bit.name;
                    params += ` bit${idx}`;
                    break;
                }
                case 0x23: { //subcmd
                    switch(cmd.param1) {
                        case 0: name = "RespawnPos_Set"; break;
                        case 1: name = "RespawnPos_Clear"; break;
                        case 2: name = "RespawnPos_Goto"; break;
                        case 3: name = "RespawnPos_SetForCRF"; break;
                    }
                    params = '';
                    break;
                }
                case 0x26: { //subcmd
                    params = '';
                    switch(cmd.param1) {
                        case 0x01: name = "Tricky_Delete"; break;
                        case 0x03: name = "Tricky_DisableBall"; break;
                        case 0x04: name = "Tricky_EnableBall"; break;
                        default:
                            name = `Tricky_Unk${hex(cmd.param1,2)}`;
                            params = hex(cmd.param2,2);
                            break;
                    }
                    break;
                }
                case 0x27: case 0x28: { //load/free map assets
                    const id = (cmd.param1 << 8) | cmd.param2;
                    params = this.game.getMapDirName(id);
                    break;
                }
                case 0x2A: case 0x2B: { //lock/unlock bucket
                    params = `${this.game.getMapDirName(cmd.param1)} ${hex(cmd.param2,2)}`;
                    break;
                }
                case 0x2D: { //show dialogue
                    const id = (cmd.param1 << 8) | cmd.param2;
                    const text = this.game.texts[id];
                    params = `[${hex(id,4)}] `;
                    if(text) {
                        params += text.phrases.join('\n').substr(0,64);
                    }
                    else params += '[unknown]';
                    break;
                }
                default:
                    params = `${hex(cmd.param1,2)}${hex(cmd.param2,2)}`;
            }
            seq.push(E.li('seqcmd', `[${conditions}] ${name} ${params}`));
        }

        result.seq = E.ul(...seq);
        return result;
    }
}

export class TrigPnt extends Trigger {
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
export class TrigCyl extends Trigger {
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
export class TrigPln extends Trigger {
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
            [-0.5, -0.5, -0.1],
            [ 0.5,  0.5,  0.1],
        )).setScale(s,s,1).setRot(0,rx,ry).setPos(x,y,z).setId(id).setColors(
            [0x40, 0xFF, 0x40, 0x80]).batch);
        return batch;
    }
}
export class TrigArea extends Trigger {
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
export class TrigTime extends Trigger {
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
export class TrigButt extends Trigger {
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
export class TriggSetp extends Trigger { //this one has extra g
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
export class TrigBits extends Trigger {
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
export class TrigCrve extends Trigger {
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
