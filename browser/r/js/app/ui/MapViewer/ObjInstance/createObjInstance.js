import { ObjInstance } from "./ObjInstance.js";
import Curve from "./Curve.js";
import { TrigPnt, TrigPln, TrigArea, TrigTime, TrigButt, TriggSetp, TrigBits, TrigCrve, TrigCyl } from "./Trigger.js";

const Types = { //ObjDefEnum => class
    [0x004B]: TrigPnt,
    [0x004C]: TrigPln,
    [0x004D]: TrigArea,
    [0x004E]: TrigTime,
    [0x004F]: TrigButt,
    [0x0050]: TriggSetp,
    [0x0054]: TrigBits,
    [0x006E]: Curve,
    [0x00F4]: TrigCrve,
    [0x0230]: TrigCyl,
};

export default function createObjInstance(gx, game, map, entry) {
    let cls = Types[entry.defNo];
    if(!cls) cls = ObjInstance;
    return new cls(gx, game, map, entry);
};
