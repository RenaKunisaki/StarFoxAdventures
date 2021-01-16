import {E, createElement} from './Element.js';
const SVG = "http://www.w3.org/2000/svg";

export default class CurveView {
    /** A widget for displaying curve data.
     */
    constructor(app) {
        this.app     = app;
        this.svg     = createElement([SVG, 'svg']);
        this.element = E.div('curve-view-main', this.svg);
        this._minX   = 0;
        this._minY   = 0;
        this._maxX   = 0;
        this._maxY   = 0;
        this._axisLabels = [];

        //create legend
        this.legend = E.ul('legend');
        this.element.append(this.legend);
    }

    addCurve(curve) {
        /** Add a curve to the display.
         *  @param curve.name Displayed name.
         *  @param curve.color Displayed color.
         *  @param curve.points List of points (objects with fields `x`, `y`).
         */
        const pathData = [];

        for(let iPoint=0; iPoint<curve.points.length; iPoint++) {
            const point = curve.points[iPoint];
            if(iPoint == 0) {
                pathData.push(`M 0 ${point.y}`);
            }
            if(point.cx1 != undefined) { //XXX not used
                if(point.cx2 != undefined) {
                    pathData.push(`C ${point.cx1} ${-point.cy1} ${point.cx2} ${-point.cy2} ${point.x} ${-point.y}`);
                }
                else pathData.push(`S ${point.cx1} ${-point.cy1} ${point.x} ${-point.y}`);
            }
            else pathData.push(`L ${point.x} ${-point.y}`);
            this._minX = Math.min(this._minX,  point.x);
            this._minY = Math.min(this._minY, -point.y);
            this._maxX = Math.max(this._maxX,  point.x);
            this._maxY = Math.max(this._maxY, -point.y);
        }
        this._addPath({
            d: pathData.join(' '),
            color: curve.color,
            width: 0.5,
        });

        this.legend.append(E.li(null,
            E.div('color', {style:`background-color: ${curve.color}`}),
            E.span('label', ' ' + curve.name),
        ));
        this._updateAxes();
    }

    _updateAxes() {
        const w = this._maxX - this._minX;
        const h = this._maxY - this._minY;
        //add 20 here to zoom out a little
        //this.svg.setAttributeNS(null, 'viewBox', `${this._minX} ${this._minY} ${w} ${h+20}`);
        this.svg.setAttributeNS(null, 'viewBox', `0 -240 256 256`);
        //this.svg.setAttribute('width', w);
        //this.svg.setAttribute('height', h);

        for(let label of this._axisLabels) label.remove();

        const next10 = n => {
            const s = (n<0) ? -1 : 1;
            n = Math.abs(n);
            return (Math.ceil(n/10)*10) * s;
        }
        //cap at 10000 to avoid insane loops when drawing axes
        const yMin=Math.max(-10000, Math.min(-1000, next10(this._minY)));
        const yMax=Math.min( 10000, Math.max( 1000, next10(this._maxY)));
        const xMin=Math.max(-10000, Math.min(-1000, next10(this._minX)));
        const xMax=Math.min( 10000, Math.max( 1000, next10(this._maxX)));
        console.log("X range", xMin, xMax, "Y range", yMin, yMax);

        //horizontal axis lines
        for(let y=yMin; y<yMax; y += 10) {
            this._axisLabels.push(this._addPath({
                d: `M ${xMin} ${y} H ${xMax-xMin}`,
                color: y == 0 ? '#FFF' : '#777',
                width: 0.25,
                dashArray: 1,
            }));
            this._axisLabels.push(this._addText({
                cls: 'axis-label', x:0, y:-y, text:String(y),
            }));
        }

        //vertical axis lines
        for(let x=xMin; x<xMax; x += 10) {
            this._axisLabels.push(this._addPath({
                d: `M ${x} ${yMin} V ${yMax-yMin}`,
                color: x == 0 ? '#FFF' : '#777',
                width: 0.25,
                dashArray: 1,
            }));
            this._axisLabels.push(this._addText({
                cls: 'axis-label', x:x-4, y:0, text:String(x),
            }));
        }
    }

    _addPath(path) {
        const ePath = createElement([SVG, 'path']);
        let fill  = 'transparent';
        let color = '#FFF';
        let width = 1;
        if(path.fill  != undefined) fill  = path.fill;
        if(path.color != undefined) color = path.color;
        if(path.width != undefined) width = path.width;
        ePath.setAttributeNS(null, 'd',            path.d);
        ePath.setAttributeNS(null, 'fill',         fill);
        ePath.setAttributeNS(null, 'stroke',       color);
        ePath.setAttributeNS(null, 'stroke-width', width);
        if(path.dashArray != undefined)
            ePath.setAttributeNS(null, 'stroke-dasharray', path.dashArray);
        this.svg.append(ePath);
        return ePath;
    }

    _addText(text) {
        const eText = createElement([SVG, 'text']);
        eText.setAttribute('class', text.cls);
        eText.textContent = text.text;
        eText.setAttributeNS(null, 'x', text.x);
        eText.setAttributeNS(null, 'y', text.y);
        this.svg.append(eText);
        return eText;
    }
}
