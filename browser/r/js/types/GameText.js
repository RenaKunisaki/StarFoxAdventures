import { hex } from "../Util.js";
import Struct from '../lib/Struct.js';

const CharacterStruct = Struct(
    ['I', 'character'],
    ['H', 'xpos'],
    ['H', 'ypos'],
    ['b', 'left'],
    ['b', 'right'],
    ['b', 'top'],
    ['b', 'bottom'],
    ['B', 'width'],
    ['B', 'height'],
    ['B', 'font'],
    ['B', 'texture'],
);
const GameTextStruct = Struct(
    ['H', 'id'],
    ['H', 'nPhrases'],
    ['B', 'window'],
    ['B', 'alignH'],
    ['B', 'alignV'],
    ['B', 'language'],
    ['I', 'phrases'],
);
const TextureStruct = Struct(
    ['H', 'texFmt'],
    ['H', 'pixFmt'],
    ['H', 'width'],
    ['H', 'height'],
);

export default class GameTextFile {
    constructor(app, file) {
        this.app     = app;
        this.file    = file;
        this.data    = file.getData();
        this._offset = 0; //file read offset
        this.readCharStructs();
        this.readTexts();
        this.readStrings();
        this.readUnknown();
        this.readCharTextures();
    }

    //XXX move to some kind of file wrapper
    _readStruct(typ) {
        const res = new typ(this.data, this._offset);
        this._offset += typ._size;
        return res;
    }

    readCharStructs() {
        const numCharStructs = this.data.getUint32(this._offset);
        this._offset += 4;
        this.charStructs = [];
        for(let i=0; i<numCharStructs; i++) {
            this.charStructs.push(this._readStruct(CharacterStruct));
        }
    }

    readTexts() {
        const numTexts   = this.data.getUint16(this._offset);
        const strDataLen = this.data.getUint16(this._offset+2);
        this._offset += 4;
        this.texts = [];
        for(let i=0; i<numTexts; i++) {
            this.texts.push(this._readStruct(GameTextStruct));
        }
    }

    readString() {
        let result = [];
        let done   = false;
        let start  = this._offset;
        while(this._offset < this.data.byteLength && !done) {
            let c = this.data.getUint8(this._offset++);
            switch(c) {
                case 0x00: done = true; break;
                case 0xEE: case 0xEF: {
                    if(this._offset > start) {
                        result.push(this._readUtf8String(start, this._offset));
                    }
                    result.push((c == 0xEE) ?
                        this._readEEcode() : this._readEFcode());
                    start = this._offset;
                    break;
                }
            }
        }
        if(this._offset > start) {
            result.push(this._readUtf8String(start, this._offset));
        }
        return result;
    }

    readStrings() {
        const numStrs = this.data.getUint32(this._offset);
        this._offset += 4;
        this.strings = [];
        for(let i=0; i<numStrs; i++) {
            let offs = this.data.getUint32(this._offset);
            this._offset += 4;
            this.strings.push({offset:offs});
        }
        const base = this._offset;
        for(let i=0; i<numStrs; i++) {
            this._offset = this.strings[i].offset + base;
            this.strings[i].fileOffs = this._offset;
            this.strings[i].str = this.readString();
        }
        this._offset = base + this.strDataLen;
    }

    readUnknown() {
        const numBytes = this.data.getUint32(this._offset);
        //this data is always all 0xEE in every file and isn't used.
        //just verify that in case it's different in some version...
        //XXX remove this.
        for(let i=0; i<numBytes; i += 4) {
            if(this.data.getUint32(this._offset + i + 4) != 0xEEEEEEEE) {
                console.log("Suspicious data in gametext file!",
                    this._offset + i + 4);
                break;
            }
        }
        this._offset += numBytes + 4;
    }

    readCharTextures() {
        this.charTextures = [];
        this.textureOffset = this._offset;
        while(true) {
            let start = this._offset;
            let texS  = this._readStruct(TextureStruct);
            if(texS.width == 0 && texS.height == 0) break;
            let size = texS.width * texS.height;
            if(texS.pixFmt == 4) { size >>= 1; }

            let tex = {
                offset: start,
                texFmt: texS.texFmt,
                pixFmt: texS.pixFmt,
                width:  texS.width,
                height: texS.height,
                data:   this.data.buffer.slice(start, start+size),
                length: ((texS.width * texS.height * texS.pixFmt) >> 4) * 2,
            };
            this._offset = start + tex.length + 4;
            this.charTextures.push(tex);
        }
    }

    _readUtf8String(start, end) {
        const decoder = new TextDecoder('utf-8');
        const view    = new DataView(this.data.buffer, start, end-start);
        return decoder.decode(view);
    }

    _readEEcode() {
        let param = this.data.getUint16(this._offset);
        this._offset += 2;
        switch(param) {
            case 0x8080: {
                param = this.data.getUint16(this._offset);
                this._offset += 2;
                return ['seq', param];
            }
            case 0x8098: {
                let a = this.data.getUint16(this._offset);
                let b = this.data.getUint16(this._offset+2);
                let c = this.data.getUint16(this._offset+4);
                this._offset += 6;
                return ['time', a, b, c];
            }

            case 0x80A0: {
                param = this.data.getUint16(this._offset);
                this._offset += 2;
                return ['hint', param];
            }

            default: return ['unkEE', param];
        }
    }

    _readEFcode() {
        let param = this.data.getUint8(this._offset++);
        if(param != 0xA3) return ['unkEF', param];

        param = this.data.getUint8(this._offset++);
        switch(param) {
            case 0xB4: {
                param = this.data.getUint16(this._offset);
                this._offset += 2;
                return ['scale', param];
            }
            case 0xB7: {
                param = this.data.getUint16(this._offset);
                this._offset += 2;
                return ['font', param];
            }
            case 0xB8: return ['justify', 'left'];
            case 0xB9: return ['justify', 'right'];
            case 0xBA: return ['justify', 'center'];
            case 0xBB: return ['justify', 'full'];

            case 0xBF: {
                const tr = this.data.getUint8(this._offset++); //text
                const tg = this.data.getUint8(this._offset++);
                const tb = this.data.getUint8(this._offset++);
                const ta = this.data.getUint8(this._offset++);
                const br = this.data.getUint8(this._offset++); //background
                const bg = this.data.getUint8(this._offset++);
                const bb = this.data.getUint8(this._offset++);
                const ba = this.data.getUint8(this._offset++);
                return ['color', [tr, tg, tb, ta], [br, bg, bb, ba]];
            }

            default: return ['unkEFA3', param];
        }
    }
}
