import BinaryFile from "../lib/BinaryFile.js";
import IsoFile from "../types/iso/isofile.js";
import { hex } from "../Util.js";

export default class GameFile extends BinaryFile {
    /** Wrapper around BinaryFile that adds game-specific features. */

    constructor(buffer, byteOffset=0, byteLength=null) {
        if(buffer instanceof IsoFile) {
            byteOffset = 0;
            byteLength = buffer.size;
            buffer     = buffer.getRawData();
        }
        super(buffer, byteOffset, byteLength, '>'); //always big endian
        this._contents = [null, null]; //cache
    }

    decompress(offset=0, _depth=0) {
        /** Decompress data at given offset and return it.
         *  @param offset Offset to read from.
         *  @returns {ArrayBuffer} Decompressed data.
         */
        console.assert(_depth < 10);
        const header = this._readArchiveHeader(offset);
        if(header == null) return null;
        this.seek(header.fileOffset);
        console.log("Decoding", header);
        switch(header.fmt) {
            case 'raw': case 'padding': {
                return this.readBytes(header.packedSize);
            }
            case 'DIR': { //non-compressed ZLB data
                this.seek(header.offset, 'SEEK_CUR');
                return this.readBytes(header.packedSize);
            }
            case 'ZLB': {
                this.seek(header.offset, 'SEEK_CUR');
                const data   = this.readBytes(header.packedSize);
                console.assert(data.byteLength == header.packedSize);
                const decomp = pako.inflate(data);
                console.assert(decomp);
                return decomp.buffer;
            }
            case 'FACEFEED': case 'E0E0E0E0': case 'F0F0F0F0': {
                //XXX do something with extra data?
                if(header.offset == 0) return null; //avoid infinite loop
                return this.decompress(offset + header.offset, _depth+1);
            }
            default: throw new Error(`Unimplemented format: '${header.fmt}'`);
        }
    }

    getItem(idx, includePadding=false) {
        const contents = this.getContents(includePadding);
        return this.decompress(contents[idx].fileOffset);
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
                let length = 4;
                while(!this.isEof() && !this.readU8()) length += 1;
                return {
                    fmt:          'padding',
                    packedSize:   length,
                    unpackedSize: length,
                    offset:       0,
                    fileOffset:   offset,
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
                fileOffset:   offset,
            };
            case 0x5A4C4200: return { //'ZLB\0'
                fmt:         'ZLB',
                version:      this.readU32(), //always 1
                unpackedSize: this.readU32(),
                packedSize:   this.readU32(),
                offset:       0x10,
                fileOffset:   offset,
            };
            case 0xE0E0E0E0: {
                const result = {
                    fmt:        'E0E0E0E0',
                    packedSize: this.readU32(),
                    offset:     this.readU32() + 0x18,
                    fileOffset:   offset,
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
                    fileOffset:   offset,
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
                    fileOffset:   offset,
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
                fileOffset:   offset,
            };
        }
    }
}
