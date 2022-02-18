import { hex } from "../Util.js";
import Struct from '../lib/Struct.js';
import BinaryFile from "../lib/BinaryFile.js";
import IsoFile from "./iso/isofile.js";

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

const ControlCodes = { //code: [param names...]
    [0xE000]: ['seq', 'id'],
    [0xE018]: ['time', 'unk1', 'time', 'unk3'],
    [0xE020]: ['hint', 'id'],
    [0xF8F2]: ['unkF8F2', 'unk1', 'unk2'], //unused
    [0xF8F3]: ['unkF8F3', 'unk1', 'unk2'], //unused
    [0xF8F4]: ['scale', 'scale'],
    [0xF8F5]: ['unkF8F5', 'unk1'], //unused
    [0xF8F6]: ['unkF8F6', 'unk1'], //unused
    [0xF8F7]: ['font', 'id'],
    [0xF8F8]: ['justifyLeft'],
    [0xF8F9]: ['justifyRight'],
    [0xF8FA]: ['justifyCenter'],
    [0xF8FB]: ['justifyFull'],
    [0xF8FC]: ['unkF8FC'], //unused
    [0xF8FD]: ['unkF8FD'], //unused
    [0xF8FE]: ['unkF8FE'], //unused
    [0xF8FF]: ['color', 'r', 'g', 'b', 'a'],
};

export default class GameTextFile {
    constructor(app, data) {
        if(data instanceof IsoFile) data = data.getData(); //lolno
        this.app     = app;
        this.data    = data;
        this._file   = new BinaryFile(this.data);
        this.readCharStructs();
        this.readTexts();
        this.readStringTable();
        this.readUnknown();
        this.readCharTextures();

        //XXX Sequences files have some extra info before the
        //actual strings, such as the sequence ID. Investigate this.

        //read the actual strings
        //console.log("strTab", this.strTab);
        for(let text of this.texts) {
            const base = text.phrases;
            text.phrases = [];
            for(let i=0; i<text.nPhrases; i++) {
                //console.log(`text 0x${hex(text.id,4)} phrase ${i} base=0x${hex(base)}`);
                if(isNaN(this.strTab[base+i])) debugger;
                this._file.seek(this.strTab[base+i] + this.strDataOffs);
                let phrase = {
                    fileOffs: this._file.tell(),
                    str: this.readString(),
                };
                phrase.byteLength = this._file.tell() - phrase.fileOffs;
                this._file.seek(phrase.byteLength, 'SEEK_CUR');
                text.phrases.push(phrase);
            }
        }
    }

    readCharStructs() {
        /** Read the CharacterStructs.
         *  These tell where each character is in the texture,
         *  which texture it's in, and how to align it.
         */
        const numCharStructs = this._file.readU32();
        //console.log(`numCharStructs = 0x${hex(numCharStructs)}`);
        //sanity check to avoid hanging
        if(numCharStructs > 10000) {
            throw new Error("Not a GameText file");
        }
        this.charStructs = this._file.read(CharacterStruct, numCharStructs);
        if(numCharStructs == 1) this.charStructs = [this.charStructs];
    }

    readTexts() {
        /** Read the GameText structs.
         *  These tell which strings belong to each text,
         *  how to display it, and which language it is.
         */
        const numTexts  = this._file.readU16();
        this.strDataLen = this._file.readU16();
        //console.log(`numTexts=0x${hex(numTexts)} strDataLen=0x${hex(this.strDataLen)}`);
        this.texts = [];
        for(let i=0; i<numTexts; i++) {
            const text = this._file.read(GameTextStruct);
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
        const result = [];
        while(this._file.tell() < this.data.byteLength) {
            const c = this._file.readUtf8Char();
            if(c == 0) break;
            else if(ControlCodes[c]) {
                //control codes are followed by zero or more parameters,
                //which can contain null bytes.
                let control = {cmd:ControlCodes[c][0]};
                for(let i=1; i < ControlCodes[c].length; i++) {
                    control[ControlCodes[c][i]] = this._file.readS16();
                }
                result.push(control);
            }
            //else if(c >= 0x80) result.push(`\\u${hex(c,4)}`);
            else result.push(String.fromCodePoint(c));
        }
        return result;
    }

    readStringTable() {
        /** Read the string offsets.
         *  These are offsets into the actual string data.
         *  The `phrases` field of GameTextStruct is an index
         *  into this list of offsets.
         */
        const numStrs = this._file.readU32();

        //read the offset table
        this.strTab = this._file.readU32(numStrs);
        if(numStrs == 1) this.strTab = [this.strTab];
        this.strDataOffs = this._file.tell();
        //console.log(`numStrs=0x${hex(numStrs)} strDataOffs=0x${hex(this.strDataOffs)}`);

        //skip past the actual strings
        this._file.seek(this.strDataLen, 'SEEK_CUR');
    }

    readUnknown() {
        /** Read some padding data.
         *  This data is always all 0xEE in every file and isn't used.
         */
        const numBytes = this._file.readU32();
        //console.log(`unkDataLen=0x${hex(numBytes)}`);
        //just verify in case it's different in some version...
        //XXX remove this.
        const offs = this._file.tell();
        for(let i=0; i<numBytes; i++) {
            let b = this._file.readU8();
            if(b != 0xEE) {
                console.log(`Suspicious data in gametext file! 0x${hex(b)} @ 0x${hex(this._offset + i + 4)}`);
                break;
            }
        }
        //console.log(`unkData end=0x${hex(offs+numBytes)}`);
        this._file.seek(offs+numBytes);
    }

    readCharTextures() {
        /** Read the texture graphics.
         */
        this.charTextures = [];
        this.textureOffset = this._file.tell();
        //game will only recognize 2 textures
        while(this.charTextures.length < 2) {
            let start = this._file.tell();
            //console.log(`charTextures[${this.charTextures.length}] start=0x${hex(start)}`);
            let texS  = this._file.read(TextureStruct);
            if(texS.width == 0 && texS.height == 0) break;
            let size = texS.width * texS.height;
            if(texS.pixFmt == 4) { size >>= 1; }

            //console.log(`charTextures[${this.charTextures.length}] size=${texS.width},${texS.height}`);
            this.charTextures.push({
                offset: start,
                texFmt: texS.texFmt,
                pixFmt: texS.pixFmt,
                width:  texS.width,
                height: texS.height,
                data:   this.data.buffer.slice(start, start+size),
                length: ((texS.width * texS.height * texS.pixFmt) >> 4) * 2,
            });
        }
    }
}
