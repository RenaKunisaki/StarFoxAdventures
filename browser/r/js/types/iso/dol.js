import { hex } from '../../Util.js';
import IsoFile from './isofile.js';

const NUM_TEXT_SECTIONS = 7;
const NUM_DATA_SECTIONS = 11;

export default class DOL extends IsoFile {
    constructor(path, offset=0, size=0, buffer=null, isSystem=false) {
        super(path, false, offset, size, buffer, offset, null, isSystem);
        this.textSections = [];
        this.dataSections = [];
        this.bssAddr      = 0;
        this.bssSize      = 0;
        this.entryPoint   = 0;
        if(buffer != null) this.readBuffer(buffer, offset);
    }

    addrToSection(addr) {
        for(let section of this.textSections.concat(this.dataSections)) {
            if(addr >= section.addr && addr < section.addr+section.size) {
                return section;
            }
        }
        return null;
    }

    addrToOffset(addr) {
        const section = this.addrToSection(addr);
        if(!section) return null;
        return (addr - section.addr) + section.offset;
    }

    readBuffer(buffer, offset) {
        if(buffer.buffer) {
            //if given a typed array, get the underlying buffer.
            offset += buffer.byteOffset;
            buffer  = buffer.buffer;
        }
        const view = new DataView(buffer, offset);
        offset = 0;
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
            .map(it => it.offset + it.size));
        console.log(`DOL size=0x${hex(this.size)}`);
        console.log(`data: ${hex(this.getRawData().getUint32(0x100))}`);

        this._dumpSections();
    }

    _dumpSections() {
        let secs = {};
        for(let i=0; i<NUM_TEXT_SECTIONS; i++) {
            const sec = this.textSections[i];
            secs[`text${i}`] = {
                addr:    hex(sec.addr),
                offset:  hex(sec.offset),
                size:    hex(sec.size),
                end:     hex(sec.addr+sec.size),
                endOffs: hex(sec.offset+sec.size),
            };
        }
        for(let i=0; i<NUM_DATA_SECTIONS; i++) {
            const sec = this.dataSections[i];
            secs[`data${i}`] = {
                addr:    hex(sec.addr),
                offset:  hex(sec.offset),
                size:    hex(sec.size),
                end:     hex(sec.addr+sec.size),
                endOffs: hex(sec.offset+sec.size),
            };
        }
        secs['bss'] = {
            addr:    hex(this.bssAddr),
            offset:  null,
            size:    hex(this.bssSize),
            end:     hex(this.bssAddr+this.bssSize),
            endOffs: null,
        };
        console.table(secs);
    }
}
