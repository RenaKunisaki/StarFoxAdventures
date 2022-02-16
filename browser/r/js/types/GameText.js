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
    constructor(app, data) {
        this.app     = app;
        this.data    = data;
        this._offset = 0; //data read offset
        this.readCharStructs();
        this.readTexts();
        this.readStringTable();
        this.readUnknown();
        this.readCharTextures();

        //read the actual strings
        for(let text of this.texts) {
            const base = text.phrases;
            text.phrases = [];
            for(let i=0; i<text.nPhrases; i++) {
                if(isNaN(this.strTab[base+i])) debugger;
                this._offset = this.strTab[base+i] + this.strDataOffs;
                let phrase = {
                    fileOffs: this._offset,
                    str: this.readString(),
                };
                phrase.byteLength = this._offset - phrase.fileOffs;
                this._offset += phrase.byteLength;
                text.phrases.push(phrase);
            }
        }
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
        const numTexts  = this.data.getUint16(this._offset);
        this.strDataLen = this.data.getUint16(this._offset+2);
        this._offset += 4;
        this.texts = [];
        for(let i=0; i<numTexts; i++) {
            const text = this._readStruct(GameTextStruct);
            //copy the struct to a normal Object so we can modify it later.
            this.texts.push({
                id:       text.id,
                nPhrases: text.nPhrases,
                window:   text.window,
                alignH:   text.alignH,
                alignV:   text.alignV,
                language: text.language,
                phrases:  text.phrases,
            });
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
                        result.push(['str',
                            this._readUtf8String(start, this._offset)]);
                    }
                    result.push((c == 0xEE) ?
                        this._readEEcode() : this._readEFcode());
                    start = this._offset;
                    break;
                }
            }
        }
        if(this._offset > start) {
            result.push(['str',
                this._readUtf8String(start, this._offset)]);
        }
        return result;
    }

    readStringTable() {
        const numStrs = this.data.getUint32(this._offset);
        this._offset += 4;

        //read the offset table
        this.strTab = [];
        for(let i=0; i<numStrs; i++) {
            let offs = this.data.getUint32(this._offset);
            this._offset += 4;
            this.strTab.push(offs);
        }

        this.strDataOffs = this._offset;

        //skip past the actual strings
        this._offset += this.strDataLen;
    }

    readUnknown() {
        const numBytes = this.data.getUint32(this._offset);
        //this data is always all 0xEE in every file and isn't used.
        //just verify that in case it's different in some version...
        //XXX remove this.
        for(let i=0; i<numBytes; i++) {
            let b = this.data.getUint8(this._offset + i + 4);
            if(b != 0xEE) {
                console.log(`Suspicious data in gametext file! 0x${hex(b)} @ 0x${hex(this._offset + i + 4)}`);
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
        //console.log(`Read string from 0x${hex(start)}`);
        const decoder = new TextDecoder('utf-8');
        const view    = new DataView(this.data.buffer,
            start+this.data.byteOffset, (end-start)-1);
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

            default: return ['unkEE', hex(param,4)];
        }
    }

    _readEFcode() {
        let param = this.data.getUint8(this._offset++);
        console.log(`PARAM ${hex(param)}`);
        //if(param != 0xA3) return ['unkEF', hex(param,2)];
        param = this.data.getUint8(this._offset++);
        switch(param) {
            case 0xB2: {
                const res = ['unkEFB2',
                    this.data.getUint16(this._offset),
                    this.data.getUint16(this._offset+2),
                ];
                this._offset += 4;
                return res;
            }
            case 0xB3: return ['unkEFB3'];
            case 0xB4: {
                param = this.data.getUint16(this._offset);
                this._offset += 2;
                return ['scale', param];
            }
            case 0xB5: {
                const res = ['unkEFB5', this.data.getUint16(this._offset)];
                this._offset += 2;
                return res;
            }
            case 0xB6: {
                const res = ['unkEFB5', this.data.getUint16(this._offset)];
                this._offset += 2;
                return res;
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
            case 0xBC: {
                const res = ['unkEFBC', this.data.getUint16(this._offset)];
                this._offset += 2;
                return res;
            }
            case 0xBD: {
                const res = ['unkEFBD', this.data.getUint16(this._offset)];
                this._offset += 2;
                return res;
            }
            case 0xBE: {
                const res = ['unkEFBE', this.data.getUint16(this._offset)];
                this._offset += 2;
                return res;
            }
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

            default: return ['unkEF', hex(param,2)];
        }
    }
}
