export default class Seekable {
    /** Base class for classes that support seeking.
     */
    constructor() {
        this.offset = 0;
        this.length = 0;
    }

    seek(offset, whence=0) {
        switch(whence) {
            case 0: //SEEK_SET
            case 'set':
                this.offset = offset;
                break;
            case 1: //SEEK_CUR
            case 'cur':
                this.offset += offset;
                break;
            case 2: //SEEK_END
            case 'end':
                this.offset = this.length - offset;
                break;
            default: throw new Error("Invalid `whence` parameter: "+String(whence));
        }
        if(this.offset < 0) this.offset = 0;
        if(this.offset > this.length) this.offset = this.length;
        return this.offset;
    }

    get isEof() {
        return this.offset >= this.length;
    }
}
