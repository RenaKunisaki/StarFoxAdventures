import {E} from './Element.js';
import Table from './Table.js';
import {get, getBin, getXml, getDescriptionAndNotes, hex, int, makeList, makeCollapsibleList, makeDescriptionAndNotes} from './Util.js';
import CurveView from './CurveView.js';
import SeqTextLut from './SeqTextLut.js';

const MainCmdNames = {
    0x00: 'SetCurvePos',
    0x01: 'CameraAnim',
    0x02: 'PlayAnim',
    0x03: 'ToggleAttach',
    0x04: 'ModelCmd04',
    0x06: 'PlaySound',
    0x07: 'ToggleCmd07',
    0x09: 'SetLoopPoint',
    0x0B: 'QueueActions',
    0x0D: 'SubCmd0D',
    0x0E: 'ShowDialogue',
    0x0F: 'StopSound',
    0x7F: 'ExtParams',
    0xFF: 'SetCurveLen',
};
const SubCmds02 = {
    0x00: 'EnqueueObjCmd',
    0x01: 'SetField60',
    0x02: 'NOP',
    0x03: 'SetGlobal1',
    0x04: 'SetGlobal2',
    0x05: 'SetBool',
    0x06: 'SetGameBit',
};
const SubCmds06 = {
    0x00: 'End',
    0x02: 'FindCurve',
    0x07: 'Earthquake',
    0x09: 'SetFlag7F_1',
    0x0A: 'StartCountupTimer',
    0x0B: 'StartCountdownTimer',
    0x0C: 'SetTimerToCountUp',
    0x0D: 'SoundCmd0D',
    0x0E: 'FadeToBlack',
    0x0F: 'FadeFromBlack',
    0x10: 'SetField7D',
    0x12: 'ToggleLetterbox',
    0x13: 'EnableLetterbox',
    0x14: 'CameraModeTestStrength',
    0x15: 'CamModeStatic', // XXX verify
    0x17: 'SwitchModel',
    0x18: 'PullOutStaff',
    0x19: 'PutAwayStaff',
    0x1A: 'CameraModeNormal',
    0x1B: 'SetObjGroupStatusTo1',
    0x1C: 'SetObjGroupStatusTo0',
    0x1D: 'SetMapAct',
    0x1E: 'DisableLetterbox',
    0x1F: 'ClearRestartPoint',
    0x20: 'GotoRestartPoint',
    0x21: 'SetFlag6E_0400_andField136',
    0x22: 'ClearFlag6E_0400_andField136',
    0x23: 'SetIsSavePoint',
    0x24: 'SavePoint',
    0x25: 'StopAndDisableTimer',
    0x26: 'PlayerLock',
    0x27: 'PlayVoice',
    0x28: 'PlayStream',
    0x2C: 'EnableMotionBlur',
    0x2D: 'DisableMotionBlur',
    0x2E: 'EnableMonochromeFilter',
    0x2F: 'DisableMonochromeFilter',
    0x30: 'EnvFxCmd30',
    0x31: 'EnvFxCmd31',
    0x32: 'EnvFxCmd32',
    0x33: 'SetCamSpeed',
};
const SubCmds0B = {
    0x01: 'SetCurvePos',
    0x02: 'ObjCmd',
    0x03: 'AddParamToField60',
    0x04: 'While',
    0x05: 'End',
    0x06: 'SubCmd06',
    0x07: 'SendObjMsg',
    0x08: 'SetEventLabel',
    0x09: 'Label',
    0x0A: 'GotoIf',
};
const SubCmds0D = {
    0x00: 'PlaySong',
    0x02: 'GetEnvFxAct',
    0x03: 'ParticleFx',
    0x04: 'NOP_04',
    0x05: 'LoadDll',
    0x06: 'WarpToMap',
    0x07: 'NOP_07',
    0x08: 'SetEyeAndEyelid',
    0x09: 'ScreenFade',
    0x0B: 'SetGameBit',
    0x0C: 'ClearGameBit',
    0x0D: 'SetInputOverride',
    0x0E: 'SetEyeState',
    0x0F: 'SetEyelidState',
};
const Conditions = {
    0x00: 'True',
    0x01: 'Field60 < 1',
    0x02: 'Field60 > 0',
    0x03: 'IsDaytime',
    0x04: 'IsNight',
    0x05: 'Bool == 0',
    0x06: 'Bool == 1',
    0x07: 'Var == 0',
    0x08: 'Var != 0',
    0x09: 'Global1 < 1',
    0x0A: 'Global1 > 0',
    0x0B: 'Global2 < 1',
    0x0C: 'Global2 > 0',
    0x0D: 'TimerDisabled',
    0x0E: 'TimerEnabled',
    //0x0F: undefined,
    0x10: 'Global3 != 0',
    0x11: 'Global3 == 0',
};
const Events = {
    0x12: "A Pressed",
    0x13: "B Pressed",
    0x1A: "Not Talking to NPC",
};
const FadeCmds0D09 = {
    0x06: {duration:3, out:true,  start:0},
    0x07: {duration:3, out:false, start:0},
    0x08: {duration:2, out:true,  start:0},
    0x09: {duration:2, out:false, start:0},
    0x0B: {duration:4, out:true,  start:0},
    0x0C: {duration:4, out:false, start:0.2},
};
const ObjMsgs0B07 = [
    {target:'one',  msg:0x00050001},
    {target:'one',  msg:0x00050002},
    {target:'one',  msg:0x00050003},
    {target:'one',  msg:0x00060001},
    {target:'one',  msg:0x00060002},
    {target:'one',  msg:0x000A0001},
    {target:'one',  msg:0x000A0002},
    {target:'one',  msg:0x000A0003},
    {target:'one',  msg:0x00000008},
    {target:'one',  msg:0x00000009},
    {target:'near', msg:0x00030002},
    {target:'near', msg:0x00030003},
    {target:'one',  msg:0x000A0004},
    {target:'one',  msg:0x000A0005},
    {target:'one',  msg:0x000A0006},
    {target:'any',  msg:0x000F000B},
    {target:'any',  msg:0x000F000C},
    {target:'any',  msg:0x000F000D},
    {target:'any',  msg:0x000F000E},
    {target:'any',  msg:0x000F000F},
    {target:'any',  msg:0x000F0010},
    {target:'one',  msg:0x00130001},
    {target:'one',  msg:0x00130002},
];
const CurveColors = {
    rot9E_Z:    '#08F',
    rot9E_X:    '#800',
    rot9E_Y:    '#080',
    Opacity:    '#444',
    TimeOfDay:  '#222',
    Scale:      '#880',
    RotZ:       '#048',
    RotX:       '#400',
    RotY:       '#040',
    AnimTimer:  '#808',
    pointSound: '#088',
    PosZ:       '#08C',
    PosY:       '#0C0',
    PosX:       '#C00',
    Unk0E:      '#C0C',
    EyeX:       '#CC0',
    EyeY:       '#0CC',
    Unk11_X:    '#AAA',
    Unk12:      '#88F',
};

export default class SeqInfo {
    /** A page showing details of a sequence.
     */
    constructor(app) {
        this.app     = app;
        this.element = E.div('seqinfo');
        this.id      = int(this.app.params.get('id'));
        this.dir     = this.app.params.get('dir');
        this.objId   = int(this.app.params.get('obj'));
        this.dll     = null;
        this.labels  = {};
    }

    getTitle() { return `Sequence Info` }

    async run() {
        if(this.id == null || this.id == undefined) {
            this.element.append(await this.makeSeqList());
        }
        else {
            const curve = await this.app.game.getSequence(this.dir, this.id);
            if(!curve) {
                alert("Sequence not found");
                return;
            }
            this.animCurve = curve;

            if(this.objId != null && this.objId != undefined) {
                const obj = this.app.game.getObjectById(this.objId);
                if(obj) this.dll = this.app.game.dlls[obj.dllId];
                console.log("Obj", obj, "DLL", this.dll);
                if(this.dll) console.log("ObjCmds", this.dll.objCmds);
            }

            this.element.append(await this.makeCurveInfo());
            this.element.append(await this.makeCurveView());
            this.element.append(await this.makeCurveActionsTable());
            this.element.append(await this.makeCurvePointsTable());
            this.element.append(await this.makeTextTable());
        }
    }

    async makeSeqList() {
        /** Display a list of all sequences in this directory.
         */
        const v = this.app.game.version;
        const d = this.dir;
        const seqsXml     = await getXml(`${v}/objseqs.xml`);
        const objSeq2C    = new DataView(await getBin(`${v}/disc/${d}/OBJSEQ2C.tab`));
        const animCurvTab = new DataView(await getBin(`${v}/disc/${d}/ANIMCURV.tab`));
        const tbl         = new Table({title:"Sequence List", columns:[
            {
                displayName: "Id",
                name: 'id',
                type: 'hex',
                length: 4,
                format: (id, col, row) => E.a('seq-link', hex(id,4), {
                    href: `?v=${v}&p=seq&dir=${d}&id=0x${hex(row.id,4)}`,
                }),
            }, {
                displayName: "Idx",
                name: 'idx',
                type: 'hex',
                length: 4,
            }, {
                displayName: "Name",
                name: 'name',
                type: 'string',
            }, {
                displayName: "Description",
                name: 'description',
                type: 'string',
                format: (desc, col, row) => makeDescriptionAndNotes(row),
            },
        ]});

        for(let i=0; i<objSeq2C.byteLength/2; i++) {
            const idx = objSeq2C.getUint16(i*2);
            if(idx == 0xFFFF) break;
            const offs = animCurvTab.getUint32(idx*4);
            if(offs & 0x80000000) {
                let eSeq = seqsXml.getElementById(`0x${hex(i,4)}`);
                let row = {
                    id: i,
                    idx: idx,
                    name: eSeq ? eSeq.getAttribute('name') : '',
                    description: '',
                };
                if(eSeq) getDescriptionAndNotes(row, eSeq);
                tbl.add(row);
            }
        }
        return E.div('box seq-list',
            E.h2(null, "Sequence List"), tbl.element);
    }

    async makeCurveInfo() {
        const C = this.animCurve;
        const res = E.div('box curve-info',
            E.h2(null, `Sequence 0x${hex(C.id,4)}`),
                E.div('name str', C.name ? `Name: ${C.name}` : "No name"),
                E.div('offset hex', `Offset 0x${hex(C.offset,6)} length 0x${hex(C.length,6)}`),
                E.div('idx hex', `Idx 0x${hex(C.idx,4)}, nAct=${C.header.nActions}`),
                E.div('description', makeDescriptionAndNotes(C)),
            );
        return res;
    }

    async makeTextTable() {
        const C = this.animCurve;
        const textLut = SeqTextLut[C.id];
        const res = E.div('box curve-text', E.h2(null, "GameText"));
        if(textLut) {
            res.append(
                E.div('note', `seq=0x${hex(textLut.seq,4)} text=0x${hex(textLut.text,4)}`));
            let text;
            if(textLut.text == 0x29) { //game does this, no idea why
                 text = this.app.game.texts[textLut.seq];
            }
            else text = this.app.game.texts[textLut.text];
            if(text) {
                const ul = E.ul('gametext');
                for(let phrase of text.phrases) {
                    ul.append(E.li(null, phrase));
                }
                res.append(ul);
            }
            else res.append(E.div('error', "[text not found]"));
        }
        else res.append(E.div('note', "no text"));
        return res;
    }

    async makeCurveView() {
        const view = new CurveView(this.app);
        let anyCurves = false;
        for(const [attr, curve] of Object.entries(this.animCurve.curves)) {
            let color = CurveColors[attr];
            if(color == undefined) color = '#FFF';
            let points = this.animCurve.interpolateCurve(curve);
            let yMin=99999999, yMax=-99999999;
            for(let p of points) {
                //if(attr == 'PosY') console.log(p.x, Math.round(p.y));
                yMin = Math.min(yMin, p.y);
                yMax = Math.max(yMax, p.y);
            }
            if(yMax > yMin) {
                console.log("Range", attr, yMin, yMax);
                view.addCurve({
                    points: points,
                    name:   attr,
                    color:  color,
                });
            }
            anyCurves = true;
        }
        return E.div('box curve-view',
            E.h2(null, "Curve Data"),
            anyCurves ? view.element : E.div('notice', "No curves"));
    }

    async makeCurvePointsTable() {
        let tbl = E.table('curve-points',
            E.tr(
                E.th(null, "Attr"),
                E.th(null, "#"),
                E.th(null, "X"),
                E.th(null, "Y"),
                E.th(null, "T", {title:"Type"}),
                E.th(null, "S", {title:"Scale"}),
                E.th(null, "F", {title:"Field"}),
                E.th(null, "H", {title:"FieldHi"}),
            ),
        );
        let nPoints=0;
        for(const [attr, curve] of Object.entries(this.animCurve.curves)) {
            //tbl.append(E.tr(E.th(null, attr, {colspan:4})));
            for(let i=0; i<curve.length; i++) {
                const point = curve[i];
                tbl.append(E.tr(
                    E.td('str',    attr),
                    E.td('number', String(i)),
                    E.td('number', String(point.x)),
                    E.td('number', point.y.toFixed(2)),
                    E.td('number', String(point.type)),
                    E.td('number', point.scale.toFixed(2)),
                    E.td('number', hex(point.field,2)),
                    E.td('number', hex(point.fieldHi,2)),
                ));
                nPoints++;
            }
        }
        if(!nPoints) tbl = E.div('notice', "No curves");
        return E.div('box seq-curve-points',
            E.h2(null, `Curve Points @0x${hex(this.animCurve.curveOffs,6)}`),
            tbl);
    }

    async makeCurveActionsTable() {
        const v = this.app.game.version;
        const tbl = E.table('curve-actions',
            E.tr(
                E.th(null, "Idx"),
                E.th(null, "Offs"),
                E.th(null, "Time"),
                E.th(null, "Secs"),
                E.th(null, "Len"),
                E.th(null, "RawParam"),
                E.th(null, "Label"),
                E.th(null, "Cmd", {colspan:2}),
                E.th(null, "Params"),
            ),
        );
        let iAction = 0;
        let curTime = 0;
        while(iAction < this.animCurve.actions.length) {
            const action = this.animCurve.actions[iAction];
            let row;
            let name = MainCmdNames[action.cmd];
            if(name == undefined) name = `Cmd_${hex(action.cmd,2)}`;
            const P = action.param;
            //console.log("Action %d", iAction, name, action.cmd, action.param);
            switch(action.cmd) {
                //AnimCurve already took care of skipping past the param data
                //for longer commands, so we can just read as-is
                case 0x02: //play animation
                    row = {name:name,
                        param:`0x${hex(P&0x0FFF,3)}, duration 0x${hex(P>>12)}`};
                    break;
                case 0x06: //play sound
                    row = {name:name,
                        param:`0x${hex(P&0x0FFF,3)}, 0x${hex(P>>12)}`};
                    break;
                case 0x0B: row = this._parseCmd0B(iAction); break;
                case 0x0D: row = this._parseCmd0D(iAction); break;
                case 0x0E: {
                    let text = this.app.game.texts[action.param];
                    if(text) {
                        if(text.phrases.length == 1) {
                            text = `0x${hex(action.param,4)} ${text.phrases[0]}`;
                        }
                        else text = E.span(null,
                            `0x${hex(action.param,4)}`,
                            makeList(text.phrases),
                        );
                    }
                    else text = `0x${hex(action.param,4)} [NOT FOUND]`;
                    row = {name: name, param: text};
                    break;
                }
                default: row = { name:name,
                    param:`${action.param} (0x${hex(action.param,4)})`,
                };
            }
            let rawParam = action.param;
            if(rawParam instanceof DataView) {
                const h = [];
                for(let i=0; i<rawParam.byteLength; i+=2) {
                    h.push(hex(rawParam.getUint16(i),4));
                }
                rawParam = h.join(' ');
            }
            else rawParam = hex(rawParam, 4);
            let label = this.labels[action.offset];
            if(label == undefined) label = '';
            tbl.append(E.tr(
                E.td('number', String(action.idx)),
                E.td('hex',    hex(action.offset, 4)),
                E.td('number', String(curTime)),
                E.td('number', (curTime/60).toFixed(2)),
                E.td('number', `${action.time} 0x${hex(action.time,2)}`),
                E.td('hex',    rawParam),
                E.td('str',    label),
                E.td('hex',    hex(action.cmd,2)),
                E.td('str',    row.name),
                E.td('hex',    row.param),
            ));
            iAction++;
            if(action.cmd != 0x0F) curTime += action.time;
            if(row.length) iAction += row.length;
        }
        return E.div('box seq-curves',
            E.h2(null, `Curve Actions @0x${hex(this.animCurve.offset,6)}`),
            tbl);
    }

    _parseCmd0B(iAction) {
        const data = this.animCurve.actions[iAction].param;
        const cmds = [];
        for(let offs=0; offs<data.byteLength; offs += 4) {
            const cmd = data.getUint16(offs+2) & 0x3F;
            const idx = data.getUint16(offs+2) >> 6;
            const param = (cmd == 2 || cmd == 3) ?
                data.getInt16(offs) : data.getUint16(offs);
            let name = SubCmds0B[cmd];
            if(name == undefined) name = `cmd_0x${hex(cmd,2)}`;
            switch(cmd) {
                case 0x02: {
                    if(idx == 0) { //object-specific command.
                        //target object is the one that was passed to
                        //ObjSeq_start.
                        name = null;
                        if(this.dll) {
                            const objCmd = this.dll.objCmds[param];
                            if(objCmd && objCmd.name) name = objCmd.name;
                        }
                        if(name == undefined || name == null) {
                            name = `ObjCmd_${hex(param,2)}`;
                        }
                    }
                    else {
                        name = SubCmds02[idx];
                        if(name == undefined) name = `SubCmd02_${hex(idx,2)}`;
                        name += ` ${param}`;
                    }
                    break;
                }
                case 0x04: {
                    let cond = Conditions[idx];
                    if(cond == undefined) cond = `cond_0x${hex(idx,2)}`;
                    name = `while(${cond}) [${hex(param,4)}]`;
                    break;
                }
                case 0x06: { //idx=op param=param byte
                    name = SubCmds06[idx];
                    if(name == undefined) name = `SubCmd06_${hex(idx,2)}`;
                    name += ` 0x${hex(param,2)}`;
                    break;
                }
                case 0x07: {
                    let msg = ObjMsgs0B07[idx];
                    if(msg == undefined) msg = `InvalidMsg_${hex(idx,2)}`;
                    else msg = `0x${hex(msg.msg,8)} to ${msg.target}`;
                    name += ` ${msg}`;
                    break;
                }
                case 0x08: {
                    let evt = Events[idx];
                    if(evt == undefined) evt = `Event 0x${hex(idx,2)}`;
                    name = `on(${evt}) goto L_${hex(param,4)}`;
                    break;
                }
                case 0x09: {
                    const o = this.animCurve.actions[iAction].offset;
                    this.labels[o] = `L_${hex(param,4)}`;
                    name  = '';
                    break;
                }
                case 0x0A: {
                    let cond = Conditions[idx];
                    if(cond == undefined) cond = `cond_0x${hex(idx,2)}`;
                    name = `if(${cond}) goto L_${hex(param,4)}`;
                    break;
                }
                default: {
                    name += ` ${param}`;
                }
            }
            cmds.push(name);
        }
        return {
            name:   MainCmdNames[0x0B],
            param:  makeList(cmds),
        };
    }

    _parseCmd0D(iAction) {
        const data  = this.animCurve.actions[iAction].param;
        const cmd   = data >> 12;
        let   param = data & 0x0FFF;
        let   len   = 0;
        if(cmd == 0x06) { //warp
            const warp = this.app.game.warps[param];
            param = `0x${hex(param,2)} `;
            if(warp) {
                if(warp.name) param += warp.name;
                else param += `${warp.x}, ${warp.y}, ${warp.z}, layer ${warp.layer}`;
            }
            else param += '(nowhere)';
        }
        else if(cmd == 0x09) { //screen transition
            //0x2F is correct here, perhaps a typo in the original game code?
            const subCmd = FadeCmds0D09[param & 0x2F]; //sic
            const effect = (param & 0x0FC0) >> 4;
            if(subCmd) {
                const dir = subCmd.out ? 'out' : 'in';
                param = `Effect 0x${hex(effect,2)}, fade ${dir}, duration ${subCmd.duration}, start ${subCmd.start}`;
            }
            else param = `Effect 0x${hex(effect,2)}, invalid param 0x${hex(param&0x2F,2)}`;
        }
        else if(cmd == 0x0B || cmd == 0x0C) {
            //I think the game discards the original param,
            //but this lets us see if it's ever set
            param = (param << 16) | this.animCurve.actions[iAction+1].param;
            len = 1;
            const bit = this.app.game.bits[param];
            let name = bit ? bit.name : undefined;
            if(name == undefined) name = '?';
            param = `0x${hex(param,4)} ${name}`;
        }
        let name = SubCmds0D[cmd];
        if(name == undefined) name = `SubCmd0D_${hex(cmd,2)}`;
        return {
            name:   name,
            param:  param,
            length: len,
        };
    }
}
