import BinaryFile from "../lib/BinaryFile.js";
import IsoFile from "../types/iso/isofile.js";

export default class GameFile extends BinaryFile {
    /** Wrapper around BinaryFile that adds game-specific features. */

    constructor(buffer, byteOffset=0, byteLength=null) {
        if(buffer instanceof IsoFile) {
            byteOffset = 0;
            byteLength = buffer.size;
            buffer = buffer.getRawData();
        }
        super(buffer, byteOffset, byteLength, '>'); //always big endian
        this._contents = [null, null]; //cache
    }

    getContents(includePadding=false) {
        /** Return a list of the file's contents.
         *  For files that are archives, this will be one or more items.
         *  For other files, it will be zero or one entries (depending whether
         *  the file is empty).
         */
        const cacheKey = includePadding ? 0 : 1;
        if(this._contents[cacheKey] != null) return this._contents[cacheKey];
        const result = [];
        let offset   = 0;
        while(offset < this.byteLength) {
            let item = this._readArchiveHeader(offset);
            if(item == null) break;

            item.headerOffs = offset;
            offset += item.offset + item.packedSize;
            if(offset & 3) offset += (3 - (offset & 3)); //round up
            if(includePadding || item.fmt != 'padding') result.push(item);
        }
        this._contents[cacheKey] = result;
        return result;
    }

    _readArchiveHeader(offset) {
        /** Read and decode an archive header at the given offset. */
        //XXX move this
        if((this.byteLength - offset) < 4) return null;
        this.seek(offset);
        const magic = this.readU32();
        switch(magic) {
            case 0x00000000: {
                let length = 1;
                while(!this.isEof() && !this.readU8()) length += 1;
                return {
                    fmt:          'padding',
                    packedSize:   length,
                    unpackedSize: length,
                    offset:       0,
                };
            }
            case 0x44495200: //'DIR\0'
            case 0x44495230: //'DIR0' (XXX used?)
            case 0x44495231: //'DIR1' (XXX used?)
            case 0x4449526E: //'DIRn' (XXX used?)
            return {
                fmt:         'DIR',
                version:      this.readU32(), //always 1
                unpackedSize: this.readU32(), //not checked, should be == packedSize
                packedSize:   this.readU32(),
                offset:       0x10,
            };
            case 0x5A4C4200: return { //'ZLB\0'
                fmt:         'ZLB',
                version:      this.readU32(), //always 1
                unpackedSize: this.readU32(),
                packedSize:   this.readU32(),
                offset:       0x10,
            };
            case 0xE0E0E0E0: {
                const result = {
                    fmt:        'E0E0E0E0',
                    packedSize: this.readU32(),
                    offset:     this.readU32() + 0x18,
                };
                result.unpackedSize = result.packedSize;
                //XXX what is this?
                result.extraData = this.readU32Array((result.offset - 0x0C)/4);
                return result;
            }
            case 0xF0F0F0F0: {
                const result = {
                    fmt:          'F0F0F0F0',
                    unpackedSize: this.readU32(),
                    offset:       this.readU32() + 0x28,
                    packedSize:   this.readU32(),
                };
                //XXX what is this?
                result.extraData = this.readU32Array((result.offset - 0x10)/4);
                return result;
            }
            case 0xFACEFEED: {
                const result = {
                    fmt:         'FACEFEED',
                    unpackedSize: this.readU32(),
                    offset:       this.readU32(),
                    packedSize:   this.readU32(),
                };
                result.extraData = this.readU32Array(result.offset);
                result.offset = (result.offset - 3) * 4; //offset from magic
                return result;
            }
            default: return {
                fmt: 'raw',
                packedSize:   this.byteLength - offset,
                unpackedSize: this.byteLength - offset,
                offset:       0,
            };
        }
    }
}
