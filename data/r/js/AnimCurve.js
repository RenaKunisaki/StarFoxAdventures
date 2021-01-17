import {E} from './Element.js';
import {int, hex} from './Util.js';
import Struct from './Struct.js';

const AnimCurvHeader = Struct({
    'signature': ['4c', 0x00], //SEQA or SEQB
    'byteSize':  ['H',  0x04],
    'nActions':  ['H',  0x06],
});
const Action = Struct({
    'cmd':   ['B', 0x00],
    'time':  ['B', 0x01],
    'param': ['H', 0x02],
});
const CurvePoint = Struct({
    'y':            ['f', 0x00],
    'typeAndScale': ['B', 0x04],
    'field':        ['B', 0x05],
    'x':            ['h', 0x06],
});
const CurveAttrs = [
    'rot9E_Z',
    'rot9E_X',
    'rot9E_Y',
    'Opacity',
    'TimeOfDay',
    'Scale',
    'RotZ',
    'RotX',
    'RotY',
    'AnimTimer',
    'pointSound',
    'PosZ',
    'PosY',
    'PosX',
    'Unk0E',
    'EyeX',
    'EyeY',
    'Unk11_X',
    'Unk12', //sound related
];

export default class AnimCurve {
    /** An animation curve script in the game.
     */
    constructor(game, data, offset, length) {
        /** Construct a curve from a region of ANIMCURV.bin.
         */
        this.game    = game;
        this.offset  = offset;
        this.length  = length;
        this.actions = [];
        this.curves  = {};

        this.header  = new AnimCurvHeader(data, offset);
        let sig = this.header.signature.join('');
        if(sig != 'SEQA' && sig != 'SEQB') {
            console.error("Unrecognized sequence header", this.header._toString());
            return;
        }

        //read actions
        offset += 8;
        const startOffs = offset;
        for(let iAction=0; iAction<this.header.nActions; iAction++) {
            const action = new Action(data, offset);
            action.offset = offset - startOffs;
            action.idx    = iAction;
            if(action.cmd == 0x0B) { //param: number of BG commands to follow
                const count = action.param;
                action.param = new DataView(data.buffer.slice(
                    offset+4, offset+4+(action.param*4)));
                offset += count * 4;
                iAction += count;
            }
            this.actions.push(action);
            offset += 4;
        }

        //find curve length - same logic the game uses
        this.xMax = 1000;
        for(let i=0; i<2; i++) {
            if(this.actions[i].cmd == 0xFF) {
                this.xMax = this.actions[i].param;
                break;
            }
        }
        //console.log("xMax=", this.xMax);

        //read curves
        this.curveOffs = offset;
        //let curve = [];
        //let prevAttr = null;

        //mostly copied game code
        const curvePoints = [];
        for(let iPoint=0; (offset-startOffs)+8 < length; iPoint++) {
            //console.log("AnimCurv read point", iPoint);
            let point = new CurvePoint(data, offset);
            curvePoints.push({
                y:            point.y,
                typeAndScale: point.typeAndScale,
                field:        point.field & 0x1F,
                fieldHi:      point.field >> 5,
                x:            point.x,
                type:         point.typeAndScale & 3,
                scale:        (point.typeAndScale >> 2) / 16,
            });
            offset += 8;
        }

        let nPoints = curvePoints.length;
        let iField = 0;
        let iPoint = 0;
        let iAttr  = 0;
        let attrNumPoints  = {};
        let attrFirstPoint = {};
        let butt = 0;
        while (iPoint < nPoints) {
            let nPointsThisField = 0;
            while((iPoint + nPointsThisField < nPoints
            && (iField == curvePoints[iPoint + nPointsThisField].field))) {
                nPointsThisField = nPointsThisField + 1;
                butt++;
                if(butt >= 1000) break;
            }
            attrNumPoints [iAttr] = nPointsThisField;
            attrFirstPoint[iAttr] = iPoint;
            iAttr++;
            iField = iField + 1;
            iPoint = iPoint + nPointsThisField;
            butt++;
            if(butt >= 1000) break;
        }
        if(butt >= 1000) console.error("shit's broke");

        //build the curves
        let nAttrs = iAttr;
        for(iAttr=0; iAttr<nAttrs; iAttr++) {
            let attrName = CurveAttrs[iAttr];
            if(attrName == undefined) attrName = `attr${hex(iAttr,2)}`;
            let curve = [];
            for(iPoint=0; iPoint<attrNumPoints[iAttr]; iPoint++) {
                curve.push(curvePoints[iPoint + attrFirstPoint[iAttr]]);
            }
            if(curve.length > 0) this.curves[attrName] = curve;
            console.log(`Curve ${attrName} len ${curve.length}`);
        }
    }

    interpolateCurve(curve) {
        /** Given a curve, compute all points from X=0 to X=this.xMax.
         */
        const result = [];
        let iPrev=1, iPoint=0;
        let r=0, a=0, b=0, c=0, d=0, t0=0, t1=0, t2=0, t3=0;
        for(let t=0; t<this.xMax; t++) {
            //find the point nearest this X position
            let found=false;
            while(iPrev < curve.length) {
                if(curve[iPrev].x == t) {
                    //if we have a point at exactly this X coord, use it.
                    found = true;
                    r = curve[iPrev].y;
                    result.push({x:t, y:r});
                    break;
                }
                else if(curve[iPrev].x > t) {
                    //if this point.x < t and next point.x > t, then
                    //interpolate between the two points.
                    iPoint = iPrev - 1;
                    break;
                }
                iPrev++;
            }
            if(found) continue;
            if(iPoint == 0) { //for first point, just use it directly
                r = curve[0].y;
                result.push({x:t, y:r});
                continue;
            }
            if(iPrev >= curve.length) break; //sanity check

            //this is mostly decompiled, so it's ugly.
            //XXX is this just some common algorithm?
            const Pprev = curve[iPoint-1];
            const Pcur  = curve[iPoint  ];
            const Pnext = curve[iPoint+1];
            //if(!(Pcur && Pprev && Pnext)) debugger;
            const Ptype = Pcur.typeAndScale & 3;
            const scale = (Pcur.typeAndScale >> 2) / 16;
            if(t == Pcur.x) { //XXX redundant? this is copied from game code...
                r = Pcur.y;
                if(Ptype > 1 && (iPoint < t + -1)) r = Pnext.y;
            }
            else {
                a  = Pcur.x - (Pprev ? Pprev.x : 0);
                t0 = Pcur.y;
                if(Ptype == 0) {
                    r  = Pnext.y - t0;
                    b  =  Math.abs(iPoint==0 ? r : (t0 - Pprev.y));
                    t2 = (Math.abs(r) + b) * scale;
                }
                if(iPoint+1 < curve.length) {
                    t1 = Pcur.y;
                    if(Ptype == 0) {
                        b  = Math.abs((iPoint+2 == curve.length) ? r : (Pnext.y - t1));
                        t3 = (r+b) * scale;
                    }
                }
                if(a < 0) r = t1;
                else {
                    r = (t - Pprev.x) / a;
                    if(Ptype == 0) {
                        c =  t3 + t2 +  2 * t0 + -2 * t1;
                        d = (-2 * t2 + -3 * t0 +  3 * t1) - t3;
                        r = r * (r * (c * r + d) + t2) + t0;
                    }
                    else if(Ptype == 1) r = r * (t1 - t0) + t0;
                    else r = t1;
                }
            }
            result.push({x:t, y:r});
        }
        //console.log("Interpolated curve:", result);
        return result;
    }
}
