import { NotImplementedError } from "../../app/errors.js";

export function PackRGBA(r, g, b, a=255) {
    return (r<<24) | (g<<16) | (b<<8) | a;
}
export function UnpackRGBA(color) {
    return [
         color >> 24,
        (color >> 16) & 0xFF,
        (color >>  8) & 0xFF,
         color        & 0xFF,
    ];
}

export default class Image {
    //Attempts to mimic the Python Image class enough for this code.
    constructor(width, height, canvas=null) {
        if(canvas == null) canvas = document.createElement('canvas');
        console.assert(width > 0 && height > 0);
        this.width   = width;
        this.height  = height;
        this._canvas = canvas;
        this._ctx    = this._canvas.getContext('2d');
        this._data   = this._ctx.createImageData(this.width, this.height);
        this._minTouchedX = this.width; //debug, can remove
        this._maxTouchedX = -1;
        this._minTouchedY = this.height;
        this._maxTouchedY = -1;

        for(let y=0; y<this.height; y++) {
            for(let x=0; x<this.width; x++) {
                //this.setPixel(x, y, 0xFF00FFFF);
                const n = ((y*this.width)+x) * 4;
                this._data.data[n  ] = 0xFF;
                this._data.data[n+1] = 0x00;
                this._data.data[n+2] = 0xFF;
                this._data.data[n+3] = 0xFF;
            }
        }
    }

    get data() {
        return this._data;
    }

    get canvas() {
        console.log("put image", this);
        this._ctx.putImageData(this._data, 0, 0); //XXX move this somewhere
        return this._canvas;
    }

    getPixel(x, y) {
        const n = ((y*this.width)+x) * 4;
        return (
            //who the fuck designed this shit!?
            //why does ImageData have its own .data!?
            //and guess what happens if you access data[x]
            //instead of data.data[x]... that's right, fuck all!
            (this._data.data[n  ] << 24) |
            (this._data.data[n+1] << 16) |
            (this._data.data[n+2] <<  8) |
             this._data.data[n+3]); //RGBA
    }

    setPixel(x, y, color) {
        //console.assert(color != undefined && color != null);
        console.assert(typeof(color) == 'number' || typeof(color) == 'object');
        if(x < 0 || x >= this.width || y < 0 || y >= this.height) return;
        this._minTouchedX = Math.min(this._minTouchedX, x);
        this._minTouchedY = Math.min(this._minTouchedY, y);
        this._maxTouchedX = Math.max(this._maxTouchedX, x);
        this._maxTouchedY = Math.max(this._maxTouchedY, y);
        const n = ((y*this.width)+x) * 4;
        if(color.length == undefined) {
            //color = 0x0000FFFF;
            this._data.data[n  ] =  color >> 24;
            this._data.data[n+1] = (color >> 16) & 0xFF;
            this._data.data[n+2] = (color >>  8) & 0xFF;
            this._data.data[n+3] =  color        & 0xFF;
        }
        else {
            console.assert(color.length == 3 || color.length == 4);
            let [r,g,b,a] = color;
            if(a == undefined) a = 255; //XXX remove?
            //for(let i=0; i<color.length; i++) this._data.data[i+n] = color[i];
            this._data.data[n  ] = r;
            this._data.data[n+1] = g;
            this._data.data[n+2] = b;
            this._data.data[n+3] = a;
        }
    }

    resize(width, height, mode) {
        throw new NotImplementedError();
    }
};
