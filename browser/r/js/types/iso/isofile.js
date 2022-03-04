import Struct from '../../lib/Struct.js';

export default class IsoFile {
    //A file in an ISO.

    constructor(path, isDir=false, offset=0, size=0, buffer=null,
    bufferOffs=0, parent=null, isSystem=false) {
        /** @description Create file in ISO.
         *  @param path       file path within ISO.
         *  @param isDir      whether this represents a directory (true) or file (false).
         *  @param offset     file data offset within ISO.
         *  @param size       file data size in bytes.
         *  @param buffer     buffer to read from.
         *  @param bufferOffs offset to read data from within buffer.
         *  @param parent     parent IsoFile.
         *  @param isSystem   whether this is a system file (extracted to /sys
         *      instead of /files).
         */
        this.path       = path;
        this.isDir      = isDir;
        this.offset     = offset;
        this.size       = size;
        this.buffer     = buffer;
        this.bufferOffs = bufferOffs;
        this.parent     = parent;
        this.isSystem   = isSystem;
        this.name       = path.split('/');
        this.name       = this.name[this.name.length - 1];
        this._data      = {}; //cache
    }

    isDescendantOf(file) {
        //Check if this file is a child or descendant of the given file.
        return ((self.parent == file) ||
            (self.parent != null && self.parent.isDescendantOf(file)));
    }

    _readHeader(offset) {
        /** Read and decode an archive header at the given offset. */
        //XXX move this
        if((this.size - offset) < 4) return null;
        const view  = this.getRawData(offset);
        const magic = view.getUint32(0, false);
        switch(magic) {
            case 0x00000000: {
                let length = 4;
                for(let i=4; (i+offset)<this.size && view.getUint32(i)==0; i+=4){
                    length += 4;
                }
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
                version:      view.getUint32(0x04, false), //always 1
                unpackedSize: view.getUint32(0x08, false), //not checked, should be == packedSize
                packedSize:   view.getUint32(0x0C, false),
                offset:       0x10,
            };

            case 0x5A4C4200: return { //'ZLB\0'
                fmt:         'ZLB',
                version:      view.getUint32(0x04, false), //always 1
                unpackedSize: view.getUint32(0x08, false),
                packedSize:   view.getUint32(0x0C, false),
                offset:       0x10,
            };

            case 0xE0E0E0E0: {
                const result = {
                    fmt:        'E0E0E0E0',
                    packedSize: view.getUint32(0x04, false),
                    offset:     view.getUint32(0x08, false) + 0x18,
                    extraData:  [],
                };
                result.unpackedSize = result.packedSize;
                for(let i=0x0C; i < result.offset; i += 4) {
                    //XXX what is this?
                    result.extraData.push(view.getUint32(i));
                }
                return result;
            }

            case 0xF0F0F0F0: {
                const result = {
                    fmt:          'F0F0F0F0',
                    unpackedSize: view.getUint32(0x04, false),
                    offset:       view.getUint32(0x08, false) + 0x28,
                    packedSize:   view.getUint32(0x0C, false),
                    extraData:    [],
                };
                for(let i=0x10; i < result.offset; i += 4) {
                    //XXX what is this?
                    result.extraData.push(view.getUint32(i));
                }
                return result;
            }

            case 0xFACEFEED: {
                const offs   = view.getUint32(0x08, false);
                const result = {
                    fmt: 'FACEFEED',
                    unpackedSize: view.getUint32(0x04, false),
                    offset:       (offs - 3) * 4, //offset from magic
                    packedSize:   view.getUint32(0x0C, false),
                    extraData:    [],
                };
                for(let i=0; i<offs; i++) {
                    result.extraData.push(view.getUint32(0x10+(i*4), false));
                }
                return result;
            }

            default: return null;
        }
    }

    getFormat(offset=0) {
        let view = new DataView(this.buffer, this.bufferOffs+offset, 4);
        const magic = view.getUint32(0, false);
        switch(magic) {
            case 0x5A4C4200: return "ZLB"; //'ZLB\0'
            default: return "raw";
        }
    }

    getRawData(offset=0, size=0) {
        if(size <= 0) size += this.size;

        let key = `R${offset},${size}`;
        if(!this._data[key]) {
            this._data[key] = new DataView(this.buffer,
                this.bufferOffs+offset, size);
        }
        return this._data[key];
    }

    getData(decompress=true, offset=0, size=0) {
        if(size <= 0) size = this.size;

        let key = `${decompress?'D':'R'}${offset},${size}`;
        if(this._data[key]) return this._data[key];

        let view = this.getRawData(offset, size);
        if(decompress && this.size >= 4) {
            const magic = view.getUint32(0, false);
            switch(magic) {
                case 0x5A4C4200: //'ZLB\0'
                    //BUG: if there are multiple ZLB archives in one file,
                    //we can only see the first one...
                    //console.log("Decompressing ZLB");
                    const decomp = pako.inflate(new Uint8Array(
                        this.buffer, this.bufferOffs+offset+0x10,
                        this.size-0x10));
                    view = new DataView(decomp.buffer);
                    break;

                //XXX FEEDFACE, LZO, etc

                default: break;
            }
        }

        this._data[key] = view;
        return view;
    }

    getContents() {
        /** If file is an archive, return list of its contents. */
        const result = [];
        let offset   = 0;
        let done     = false;
        while(offset < this.size && !done) {
            //const view = this.getRawData(offset);
            let item = this._readHeader(offset);
            if(item == null) {
                item = {
                    fmt:          'raw',
                    packedSize:   this.size - offset,
                    unpackedSize: this.size - offset,
                    offset:       offset,
                    headerOffs:   offset,
                };
                done = true;
            }
            else {
                item.headerOffs = offset;
                offset += item.offset + item.packedSize;
                //round up
                if(offset & 3) offset += (3 - (offset & 3));
            }
            result.push(item);
        }
        return result;
    }
};
