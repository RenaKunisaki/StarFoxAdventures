import Struct from '../../lib/Struct.js';
import IsoFile from './isofile.js';

const NUM_TEXT_SECTIONS = 7;
const NUM_DATA_SECTIONS = 11;

export default class DOL extends IsoFile {
    constructor(path, offset=0, size=0, buffer=null, isSystem=false) {
        super(path, false, offset, size, buffer, 0, null, isSystem);
        this.textSections = [];
        this.dataSections = [];
        this.bssAddr      = 0;
        this.bssSize      = 0;
        this.entryPoint   = 0;
        if(buffer != null) this.readBuffer(buffer, offset);
    }

    readBuffer(buffer, offset) {
        if(buffer.buffer) {
            //if given a typed array, get the underlying buffer.
            offset += buffer.byteOffset;
            buffer  = buffer.buffer;
        }
        const view = new DataView(buffer);
        for(let i=0; i<NUM_TEXT_SECTIONS; i++) {
            this.textSections.push({offset:view.getUint32(offset)});
            offset += 4;
        }
        for(let i=0; i<NUM_DATA_SECTIONS; i++) {
            this.dataSections.push({offset:view.getUint32(offset)});
            offset += 4;
        }
        for(let i=0; i<NUM_TEXT_SECTIONS; i++) {
            this.textSections[i].addr = view.getUint32(offset);
            offset += 4;
        }
        for(let i=0; i<NUM_DATA_SECTIONS; i++) {
            this.dataSections[i].addr = view.getUint32(offset);
            offset += 4;
        }
        for(let i=0; i<NUM_TEXT_SECTIONS; i++) {
            this.textSections[i].size = view.getUint32(offset);
            offset += 4;
        }
        for(let i=0; i<NUM_DATA_SECTIONS; i++) {
            this.dataSections[i].size = view.getUint32(offset);
            offset += 4;
        }
        this.bssAddr    = view.getUint32(offset);
        this.bssSize    = view.getUint32(offset+4);
        this.entryPoint = view.getUint32(offset+8);
        offset += 12;

        this.size = Math.max(...(this.textSections.concat(this.dataSections))
            .map(it => it.size));
    }
}
