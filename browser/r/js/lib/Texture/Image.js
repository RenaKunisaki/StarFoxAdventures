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
    }

    get data() {
        return this._data;
    }

    get canvas() {
        return this._canvas;
    }

    getPixel(x, y) {
        const n = ((y*this.width)+x) * 4;
        const data = this._data.slice(n, n+4);
        return (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3]; //RGBA
    }

    setPixel(x, y, color) {
        console.assert(color != undefined && color != null);
        const n = ((y*this.width)+x) * 4;
        if(color.length == undefined) {
            this._data[n  ] =  color >> 24;
            this._data[n+1] = (color >> 16) & 0xFF;
            this._data[n+2] = (color >>  8) & 0xFF;
            this._data[n+3] =  color        & 0xFF;
        }
        else {
            for(let i=0; i<color.length; i++) this._data[i+n] = color[i];
        }
    }

    resize(width, height, mode) {
        throw new Error("Not implemented");
    }
};
