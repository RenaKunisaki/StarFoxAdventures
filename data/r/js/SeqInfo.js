import {E} from './Element.js';
import Table from './Table.js';
import {get, getBin, getXml, getDescriptionAndNotes, hex, int, makeList, makeCollapsibleList, makeDescriptionAndNotes, hsv2rgb} from './Util.js';
import CurveView from './CurveView.js';
import SeqTextLut from './SeqTextLut.js';

const MainCmdNames = {
    0x00: 'WaitUntil',
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
const EyeStates = ["Normal", "Half-Closed", "Closed", "3/4 Closed",
    "Wide Open", "Confused", "Annoyed"];
const MakeCurveColor = (i) => {
    const [r,g,b] = hsv2rgb((i/19)*360, 0.5, ((i%4)*0.15)+0.5);
    return `#${hex(Math.floor(r*16))}${hex(Math.floor(g*16))}${hex(Math.floor(b*16))}`;
};
const CurveColors = {
    PosX:       MakeCurveColor( 0), //'#C00',
    RotX:       MakeCurveColor( 1), //'#400',
    EyeX:       MakeCurveColor( 2), //'#CC0',
    HeadRotX:   MakeCurveColor( 3), //'#800',
    MouthOpen:  MakeCurveColor( 4), //'#AAA',
    PosY:       MakeCurveColor( 5), //'#0C0',
    RotY:       MakeCurveColor( 6), //'#040',
    EyeY:       MakeCurveColor( 7), //'#F0F',
    HeadRotY:   MakeCurveColor( 8), //'#080',
    PosZ:       MakeCurveColor( 9), //'#08C',
    RotZ:       MakeCurveColor(10), //'#048',
    HeadRotZ:   MakeCurveColor(11), //'#08F',
    Opacity:    MakeCurveColor(12), //'#444',
    TimeOfDay:  MakeCurveColor(13), //'#222',
    Scale:      MakeCurveColor(14), //'#880',
    AnimTimer:  MakeCurveColor(15), //'#808',
    pointSound: MakeCurveColor(16), //'#084',
    FOV:        MakeCurveColor(17), //'#C0C',
    Unk12:      MakeCurveColor(18), //'#88F',
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
        this.seqIdx  = int(this.app.params.get('idx'));
        this.dll     = null;
        this.labels  = {};
        this.obj     = (this.objId == null || this.objId == undefined) ? null : this.app.game.getObjectById(this.objId);
        this.map = this.app.game.mapsByDir[this.dir];
        console.log("Object", this.obj);
    }

    getTitle() { return `Sequence Info` }

    async run() {
        if(this.id == null || this.id == undefined) {
            this.element.append(await this.makeSeqList());
            return;
        }

        let curve;
        try {
            await this.map.getSeqTables();

            //get the sequence ID from the object's sequence list.
            //unnecessary because we already have it from the URL param.
            //also seqIdx here is index into the object list for this
            //sequence, not the object's sequence list.
            //const seqId = this.obj.sequences[this.seqIdx];
            //console.log("seqId", hex(seqId));

            //use that as index into OBJSEQ2C
            let id = this.map.lookupSequence((this.id << 4) | 0x8000 | this.seqIdx);
            console.log("=>", hex(id));
            curve = await this.app.game.getSequence(this.map, id);
        }
        catch(ex) {
            console.error(ex);
            alert("Error loading sequence");
            return;
        }
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

        const eCurveInfo    = await this.makeCurveInfo();
        const eCurvePoints  = await this.makeCurvePointsTable();
        const eCurveView    = await this.makeCurveView();
        const eCurveActions = await this.makeCurveActionsTable();
        const eText         = await this.makeTextTable();
        const eObjList      = await this.makeObjList();

        this.element.append(
            E.div('leftcol', eCurveInfo, eObjList, eCurvePoints),
            E.div('rightcol', eCurveView,
                E.div('bottom', eCurveActions, eText)
            ),
        );
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

    async makeObjList() {
        //XXX this is mostly copied from ObjInfo.js, should merge them
        const tbl = E.table(E.tr(
            //E.th(null, "ObjID"),
            E.th(null, "Flags"),
            E.th(null, "Seq", {colspan:2}),
            E.th(null, "Object", {colspan:2}),
        ));
        const game = this.app.game;
        const v    = game.version;
        const dir  = this.map.dirName;
        const data = game.sequences[this.id];
        for(let i=0; i<data.length; i++) {
            const item = data[i];
            let sobj = item.objId ? null : game.getObjectById(item.defNo);
            let objName = '';
            if(!item.objId) {
                if(item.defNo == 0xFFFF) objName = "[Override]";
                else if(item.defNo == 0xFFFE) objName = "[AnimCamera]";
                else if(sobj) objName = E.a('objlink', sobj.name,
                    {href:`?v=${v}&p=obj&id=${sobj.id}`});
            }
            else {
                sobj = await game.getObjectByUniqueId(item.objId);
                if(sobj) {
                    objName = E.span(null,
                        E.a('objlink', sobj.name,
                        {href:`?v=${v}&p=obj&id=${sobj.id}`}),
                        " in ",
                        E.a('maplink', sobj.romlist,
                        {href:`?v=${v}&p=map&romlist=${sobj.romlist}`}),
                    );
                }
                else objName = '[not found]';
            }

            let newId  = ((this.id & 0x07FF) << 4) | 0x8000 | ((i+1) & 0xF);
            let realId = this.map.lookupSequence(newId);
            let newSeq = game.sequences[realId];
            let defNo  = item.defNo;
            if(defNo == 0xFFFF) defNo = 0x06; //Override
            if(defNo == 0xFFFE) defNo = 0x1E; //AnimCamera

            tbl.append(E.tr(i == this.seqIdx ? 'current' : '',
                //E.td('hex', hex(item.objId, 8)),
                E.td('hex', hex(item.flags, 4)),
                E.td('hex', E.a('seqlink', hex(realId, 4), {
                    href: `?v=${v}&p=seq&dir=${dir}&id=0x${hex(this.id,4)}&obj=0x${hex(defNo)}&idx=${i}`
                })),
                E.td('str', newSeq ? newSeq.name : '-'),
                E.td('hex', hex(item.defNo, 4)),
                E.td('str', objName),
            ));
        }
        return E.div('box seq-obj-list',
            E.h2(null, `Obj List 0x${hex(this.id,4)}`),
            E.ul(
                E.li('note dir', `File: ${dir}/OBJSEQ.bin`),
                E.li('note hex', `Offset: 0x${hex(data[0].offset,6)}`),
            ),
            tbl,
        );
    }

    async makeTextTable() {
        //get the sequence list entry for this object
        const textLut = SeqTextLut[this.id];
        if(textLut) {
            let text, src;
            if(textLut.text == 0x29) { //game does this, no idea why
                 text = this.app.game.texts[textLut.seq];
                 src = `Sequences/${textLut.seq}`;
            }
            else {
                text = this.app.game.texts[textLut.text];
                src  = `ID 0x${hex(textLut.text,4)}`;
            }
            const res = E.div('box curve-text',
                E.h2(null, `GameText: ${src}`),
            );
            if(text) {
                const ul = E.ul('gametext');
                for(let phrase of text.phrases) {
                    //wrap in a span here so we can add a background to only
                    //the text itself, not the entire row.
                    //this lets us see trailing whitespace.
                    ul.append(E.li(null, E.span(null, phrase)));
                }
                res.append(ul);
            }
            else res.append(E.div('error', "[text not found]"));
            return res;
        }
        else {
            return E.div('box curve-text',
                E.h2(null, "GameText"),
                E.div('note', "no text"),
            );
        }
    }

    async makeCurveView() {
        const view = new CurveView(this.app);
        this.curveView = view;
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
        view.redraw();
        return E.div('box curve-view',
            E.h2(null, "Curve Data"),
            anyCurves ? view.element : E.div('notice', "No curves"));
    }

    async makeCurvePointsTable() {
        let tbl = E.table('curve-points',
            E.tr(
                E.th(null, "Offset"),
                E.th(null, "Attr"),
                E.th(null, "#"),
                E.th(null, "X"),
                E.th(null, "Y"),
                E.th(null, "T", {title:"Type"}),
                E.th(null, "S", {title:"Scale"}),
                //E.th(null, "F", {title:"Field"}),
                E.th(null, "H", {title:"FieldHi"}),
            ),
        );
        let nPoints=0;
        let offs = this.animCurve.curveOffs;
        for(const [attr, curve] of Object.entries(this.animCurve.curves)) {
            //tbl.append(E.tr(E.th(null, attr, {colspan:4})));
            for(let i=0; i<curve.length; i++) {
                const point = curve[i];
                tbl.append(this._makeRowForPoint(attr, i, point, offs));
                nPoints++;
                offs += 8;
            }
        }
        if(!nPoints) tbl = E.div('notice', "No curves");
        return E.div('box seq-curve-points',
            E.h2(null, `Curve Points @0x${hex(this.animCurve.curveOffs,6)}`),
            tbl);
    }

    _makeRowForPoint(attr, i, point, offs) {
        const tr = E.tr(
            E.td('hex',    hex(offs,6)),
            E.td('str',    attr),
            E.td('number', String(i)),
            E.td('number', String(point.x)),
            E.td('number', point.y.toFixed(2)),
            E.td('number', String(point.type)),
            E.td('number', point.scale.toFixed(2)),
            //E.td('number', hex(point.field,2)),
            E.td('number', hex(point.fieldHi,2)),
        );
        tr.addEventListener('mouseover', e => {
            this.curveView.setCursorPos(point.x, point.y);
        });
        return tr;
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
            tbl.append(this._makeRowForAction(action, curTime, rawParam, row));
            iAction++;
            if(action.cmd == 0x00) curTime =  action.param;
            else if(action.cmd != 0x0F) curTime += action.time;
            if(row.length) iAction += row.length;
        }
        return E.div('box seq-curves',
            E.h2(null, `Curve Actions @0x${hex(this.animCurve.offset,6)}`),
            tbl);
    }

    _makeRowForAction(action, curTime, rawParam, row) {
        let label = this.labels[action.offset];
        if(label == undefined) label = '';
        const tr = E.tr(
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
        );
        tr.addEventListener('mouseover', e => {
            this.curveView.highlightTime(curTime, action.time);
        });
        tr.addEventListener('mouseout', e => {
            this.curveView.highlightTime(null);
        });
        return tr;
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
        else if(cmd == 0x08 || cmd == 0x0E || cmd == 0x0F) { //eye state
            let eye = EyeStates[param];
            if(!eye) eye = `0x${hex(param,2)}`;
            param = eye;
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
