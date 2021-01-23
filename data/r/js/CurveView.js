import {E, createElement} from './Element.js';
//const SVG = "http://www.w3.org/2000/svg";

const next10 = n => {
    const s = (n<0) ? -1 : 1;
    n = Math.abs(n);
    return (Math.ceil(n/10)*10) * s;
}

export default class CurveView {
    /** A widget for displaying curve data.
     */
    constructor(app) {
        this.app           = app;
        this.canvas        = E.canvas(null);
        this.ctx           = this.canvas.getContext('2d')
        this.element       = E.div('curve-view-main', E.div('curve-view-css-is-a-butt', this.canvas));
        this._minX         = 0;
        this._minY         = 0;
        this._maxX         = 0;
        this._maxY         = 0;
        this.offsetX       = 0;
        this.offsetY       = 0;
        this.scaleX        = 1;
        this.scaleY        = 1;
        this._axisLabels   = [];
        this._curves       = [];
        this._curvesByName = {};
        this._highlight    = null;
        this._cursorX      = null;
        this._cursorY      = null;

        //create legend
        this.legend = E.ul('legend');
        this.element.append(this.legend);
        this.eCoords = E.div('coords');
        this.element.append(this.eCoords);
        this.eDebug = E.div('debug');
        this.element.append(this.eDebug);

        this._doneDrawing = false;
        let _startXM=null, _startYM=null; //mouse start
        let _startXo=null, _startYo=null; //offset start
        let _startXs=null, _startYs=null; //scale start
        this.canvas.addEventListener('mousemove', e => {
            if(e.buttons & 1) {
                if(_startXM == null) {
                    _startXM = e.layerX;
                    _startYM = e.layerY;
                    _startXo = this.offsetX;
                    _startYo = this.offsetY;
                    _startXs = this.scaleX;
                    _startYs = this.scaleY;
                }
                if(e.shiftKey) { //scale the display
                    this.scaleX = Math.max(0.01, (((_startXM - e.layerX) / 256) + _startXs));
                    this.scaleY = Math.max(0.01, (((_startYM - e.layerY) / 256) + _startYs));
                }
                else { //pan the display
                    this.offsetX = ((e.layerX - _startXM) * this.scaleX) + _startXo;
                    this.offsetY = ((e.layerY - _startYM) * this.scaleY) + _startYo;
                }
                //console.log("offs", this.offsetX, this.offsetY,
                //    "scale", this.scaleX, this.scaleY);
                this.queueDraw();
            }
            else { //update the cursor position
                _startXM = null;
                this._cursorX = e.layerX;
                this._cursorY = e.layerY;
                this.queueDraw();
            }
        });
        this.canvas.addEventListener('mouseout', e => {
            if(this._cursorX != null) { //hide the cursor
                this._cursorX = null;
                this._cursorY = null;
                this.queueDraw();
            }
        })

        this._creationTime = performance.now();
        if(window.ResizeObserver) {
            new ResizeObserver(() => this.queueDraw()).observe(this.canvas.parentElement);
        }
        else {
            let rect = this.canvas.parentElement.getBoundingClientRect();
            let prevW = rect.width, prevH = rect.height;
            setInterval(() => {
                rect = this.canvas.parentElement.getBoundingClientRect();
                if(rect.width != prevW || rect.height != prevH) {
                    //XXX ResizeObserver should do this stuff
                    const doScale = (performance.now() - this._creationTime < 1000);
                    this.queueDraw();
                    rect = this.canvas.parentElement.getBoundingClientRect();
                    console.log(`size changed from ${prevW}x${prevH} to ${rect.width}x${rect.height}`);
                    prevW = rect.width;
                    prevH = rect.height;
                    if(doScale) this._scaleToFit();
                }
            }, 500);
        }
    }

    addCurve(curve) {
        /** Add a curve to the display.
         *  @param curve.name Displayed name.
         *  @param curve.color Displayed color.
         *  @param curve.points List of points (objects with fields `x`, `y`).
         */
        this._curves.push(curve);
        this._curvesByName[curve.name] = curve;

        //make legend item
        const li = E.li(null,
            E.div('color', {style:`background-color: ${curve.color}`}),
            E.span('label', ' ' + curve.name),
        );
        li.addEventListener('mouseover', e => {
            this.highlightCurve(curve.name);
        });
        li.addEventListener('mouseout', e => {
            this.highlightCurve(null);
        });
        this.legend.append(li);

        //find range
        for(let iPoint=0; iPoint<curve.points.length; iPoint++) {
            const point = curve.points[iPoint];
            let x = point.x;
            let y = point.y;
            this._minX = Math.min(this._minX, point.x);
            this._minY = Math.min(this._minY, point.y);
            this._maxX = Math.max(this._maxX, point.x);
            this._maxY = Math.max(this._maxY, point.y);
        }

        this._scaleToFit();
    }

    _scaleToFit() {
        const w = (this._maxX - this._minX);
        const h = (this._maxY - this._minY);
        this.scaleX = Math.min(10, Math.max(0.01, w / this.canvas.width));
        this.scaleY = Math.min(10, Math.max(0.01, h / this.canvas.height));
        this.offsetX = (this.canvas.width  * this.scaleX)*0.15;
        this.offsetY = (this.canvas.height * this.scaleY)*0.90;
        this.queueDraw();
    }

    queueDraw() {
        if(!this._doneDrawing) return;
        this._doneDrawing = false;
        window.requestAnimationFrame(t => this.redraw());
    }

    redraw() {
        const rect = this.canvas.parentElement.getBoundingClientRect();
        if(rect.width < 1 || rect.height < 1) {
            this._doneDrawing = true;
            return;
        }
        this.canvas.setAttribute('width',  rect.width);
        this.canvas.setAttribute('height', rect.height);
        this._doneDrawing = false;
        this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
        this._drawAxes();
        for(let curve of this._curves) this._drawCurve(curve);
        this._doneDrawing = true;
    }

    highlightCurve(name) {
        this._highlight = name ? this._curvesByName[name] : null;
        this.queueDraw();
    }

    highlightTime(time, length=1) {
        this._highlightTime   = time;
        this._highlightLength = length;
        this.queueDraw();
    }

    setCursorPos(x, y) {
        this._cursorX =  ((x+this.offsetX)/this.scaleX);
        this._cursorY = -((y-this.offsetY)/this.scaleY);
        this.queueDraw();
    }

    _drawCurve(curve) {
        this.ctx.beginPath();
        this.ctx.strokeStyle=curve.color;
        if(this._highlight == null || this._highlight == curve) {
            this.ctx.lineWidth = 2;
        }
        else this.ctx.lineWidth = 0.5;
        for(let iPoint=0; iPoint<curve.points.length; iPoint++) {
            const point = curve.points[iPoint];
            let x = ( point.x + this.offsetX) / this.scaleX;
            let y = (-point.y + this.offsetY) / this.scaleY;
            if(iPoint==0) this.ctx.moveTo(x, y);
            else this.ctx.lineTo(x, y);
        }
        this.ctx.stroke();
    }

    _drawAxes() {
        this.ctx.lineWidth = 1;

        const CW  = this.canvas.width;
        const CH  = this.canvas.height;
        const SX  = this.scaleX;
        const SY  = this.scaleY;
        const OSX = this.offsetX / SX;
        const OSY = this.offsetY / SY;
        const xp  = Math.max(0, Math.log10(1/SX)); //X coord decimal places
        const yp  = Math.max(0, Math.log10(1/SY)); //Y coord decimal places

        //draw vertical lines
        //every 10 units, skipping those that are too close together
        let xd = 50 / SX; //scaled distance between lines
        while(xd > 80) xd /= 2;
        while(xd < 50) xd *= 2;
        let xStart = OSX % xd;
        this.ctx.strokeStyle='#888';
        this.ctx.beginPath();
        for(let x=0; x<=CW; x += xd) {
            //x is in scaled units
            this.ctx.moveTo(x+xStart, 0);
            this.ctx.lineTo(x+xStart, CH);
        }
        this.ctx.stroke();

        //draw horizontal lines
        let yd = 20 / SY;
        while(yd > 50) yd /= 2;
        while(yd < 20) yd *= 2;
        let yStart = OSY % yd;
        this.ctx.beginPath();
        for(let y=0; y<=CH; y += yd) {
            this.ctx.moveTo(0,  y+yStart);
            this.ctx.lineTo(CW, y+yStart);
        }
        this.ctx.stroke();

        //draw zeros
        this.ctx.strokeStyle='white';
        this.ctx.beginPath();
        this.ctx.moveTo(  0, OSY);
        this.ctx.lineTo( CW, OSY);
        this.ctx.moveTo(OSX,   0);
        this.ctx.lineTo(OSX,  CH);
        this.ctx.stroke();

        //draw cursor
        if(this._cursorX != null) {
            this.ctx.beginPath();
            this.ctx.moveTo(  0, this._cursorY);
            this.ctx.lineTo( CW, this._cursorY);
            this.ctx.moveTo(this._cursorX,  0);
            this.ctx.lineTo(this._cursorX, CH);
            this.ctx.stroke();
            const cx =  (this._cursorX-OSX)*SX;
            const cy = -(this._cursorY-OSY)*SY;
            this.eCoords.textContent = `${cx.toFixed(xp)}, ${cy.toFixed(yp)}`;
        }

        //draw highlight time
        if(this._highlightTime != null) {
            const t1 = this._highlightTime;
            const t2 = t1 + this._highlightLength;
            let x1 = (t1 + this.offsetX) / this.scaleX;
            let x2 = (t2 + this.offsetX) / this.scaleX;
            if(Math.floor(x2) <= Math.floor(x1)) x2 = x1+1;
            this.ctx.fillStyle = 'rgba(255, 0, 0, 0.5)';
            this.ctx.fillRect(x1, 0, x2-x1, CH);
        }

        this.ctx.font = '10px serif';
        this.ctx.fillStyle='white';

        //draw vertical labels
        for(let x=0; x<=CW; x += xd) {
            const str = (((x+xStart)-OSX)*SX).toFixed(xp);
            const textMetrics = this.ctx.measureText(str);
            this.ctx.fillText(str, (x+xStart)-(textMetrics.width/2), CH-10);
            //lol of course textMetrics doesn't contain height,
            //that would just be stupid.
        }

        //draw horizontal labels
        for(let y=0; y<=CH; y += yd) {
            const str = (-(((y+yStart)-OSY)*SY)).toFixed(yp);
            this.ctx.fillText(str, 5, y+yStart+5);
        }

        //draw axis labels
        const labelX = "Time";
        let textMetrics = this.ctx.measureText(labelX);
        this.ctx.fillText(labelX, (CW/2)-(textMetrics.width/2), CH-3);

        //this.eDebug.textContent =
        //    `CW/H=${CW}/${CH} SX/Y=${SX.toFixed(4)}/${SY.toFixed(4)} OX/Y=${this.offsetX.toFixed(2)}/${this.offsetY.toFixed(2)} d=${xd.toFixed(2)}/${yd.toFixed(2)}`;
    }
}
