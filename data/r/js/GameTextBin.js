import {E} from './Element.js';
import Table from './Table.js';
import {get, hex, makeList, makeCollapsibleList, makeDescriptionAndNotes} from './Util.js';
import {Texture} from './Texture.js';

//XXX move these

/** The number of parameters each control code has.
 *  @note Each parameter is two bytes, and either or both of those bytes can be zero.
 */
const controlCharLength = {
    0xF8F2:2, 0xF8F3:0, 0xF8F4:1, 0xF8F5:1, 0xF8F6:1, 0xF8F7:1, 0xF8F8:0, 0xF8F9:0,
    0xF8FA:0, 0xF8FB:0, 0xF8FC:0, 0xF8FD:0, 0xF8FE:0, 0xF8FF:4, 0xE000:1, 0xE018:3,
    0xE020:1,
};

/** Identifies the location of one character in the font textures.
 */
class CharacterStruct {
    constructor(app, data, offset) {
        this.character = data.getUint32(offset+0x00); //unicode number
        this.xpos      = data.getUint16(offset+0x04); //coordinate of leftmost texel in font bitmap
        this.ypos      = data.getUint16(offset+0x06); //coordinate of topmost texel in font bitmap
        this.left      = data.getInt8  (offset+0x08); //spacing
        this.right     = data.getInt8  (offset+0x09); //spacing
        this.top       = data.getInt8  (offset+0x0A); //spacing
        this.bottom    = data.getInt8  (offset+0x0B); //spacing
        this.width     = data.getUint8 (offset+0x0C); //size
        this.height    = data.getUint8 (offset+0x0D); //size
        this.fontNo    = data.getUint8 (offset+0x0E); //Index into GameTextFonts[] in GameTextData.h
        this.textureNo = data.getUint8 (offset+0x0F); //Index into Textures[] in the binary file
    }
}

/** Defines a dialogue message.
 */
class GameTextStruct {
    constructor(app, data, offset) {
        this.identifier = data.getUint16(offset+0x00); // which game text is this
        this.numPhrases = data.getUint16(offset+0x02); // how many phrases this text has
        this.window     = data.getInt8  (offset+0x04); // preferred window
        this.alignH     = data.getInt8  (offset+0x05); // horizontal alignment
        this.alignV     = data.getInt8  (offset+0x06); // vertical alignment
        this.language   = data.getInt8  (offset+0x07); // LANGUAGE_xxx from GameTextData.h
        this.phrases    = data.getUint32(offset+0x08); // pointer to <numPhrases> pointers to localised strings
    }
}

/** Defines a font texture image.
 */
class FontTexture {
    constructor(app, data, offset) {
        this.texFmt = data.getUint16(offset+0x00);
        this.pixFmt = data.getUint16(offset+0x02);
        this.width  = data.getUint16(offset+0x04);
        this.height = data.getUint16(offset+0x06);
        offset += 8;
        if(this.width != 0 || this.height != 0) {
            //convert length for format
            let texFmt = this.texFmt;
            if(texFmt == 2) texFmt = 0; //game does this for some reason
            else if(texFmt == 1) texFmt = 5;
            let length = Texture.calcSize(this.width, this.height, texFmt, false, 0);
            this.bytes = data.buffer.slice(offset, offset + length);
        }
        else this.bytes = null;
    }
}

function readStr(data, offset) {
    let str = '';
    while(true) {
        let b = data.getUint8(offset); offset++;
        if(!b) break;

        if(b >= 0x80) { b = (b << 8) | data.getUint8(offset); offset++; }
        if(controlCharLength[b] != undefined) { //XXX what's this table used for?
            str = `<${hex(b, 4)}`;
            for(let iCtrl=0; iCtrl<controlCharLength[b]; iCtrl++) {
                b  = (data.getUint8(offset) << 8); offset++;
                b |=  data.getUint8(offset);       offset++;
                str += ` ${hex(b, 4)}`;
            }
            str += '>';
        }
        else if(b >= 0x80) {
            switch(b) {
                case 0xEE80: { //sequence commands
                    let b2 = data.getUint8(offset); offset++;
                    switch(b2) {
                        case 0x80: { //sequence ID
                            let b3 = data.getUint16(offset); offset += 2;
                            str += `<SEQ #0x${hex(b3)}>`;
                            break;
                        }
                        case 0x98: { //sequence timing
                            let b3 = data.getUint16(offset); offset += 2;
                            let b4 = data.getUint16(offset); offset += 2;
                            let b5 = data.getUint16(offset); offset += 2;
                            str += `<SEQ TIME 0x${hex(b3)} 0x${hex(b4)} 0x${hex(b5)}>`;
                            break;
                        }
                        default:
                            str += `<SEQ UNK${hex(b2, 2)}`;
                    } //switch(b2)
                    break;
                } //case 0xEE80
                case 0xEFA3: { //render commands
                    let b2 = data.getUint8(offset); offset++;
                    switch(b2) {
                        case 0xB4: { //set scale
                            let scale = data.getUint16(offset); offset += 2;
                            str += `<SCALE ${scale / 2.56}%>`;
                            break;
                        }
                        case 0xB7: { //set font
                            let font = data.getUint16(offset); offset += 2;
                            str += `<FONT ${font}>`;
                            break;
                        }
                        case 0xB8: str += `<JUSTIFY LEFT>`; break;
                        case 0xB9: str += `<JUSTIFY RIGHT>`; break;
                        case 0xBA: str += `<JUSTIFY CENTER>`; break;
                        case 0xBB: str += `<JUSTIFY FULL>`; break;
                        case 0xBF: { //set color
                            let tr = data.getUint8(offset); offset++;
                            let tg = data.getUint8(offset); offset++;
                            let tb = data.getUint8(offset); offset++;
                            let ta = data.getUint8(offset); offset++;
                            let sr = data.getUint8(offset); offset++;
                            let sg = data.getUint8(offset); offset++;
                            let sb = data.getUint8(offset); offset++;
                            let sa = data.getUint8(offset); offset++;
                            str += `<COLOR #${hex(tr,2)}${hex(tg,2)}${hex(tb,2)}${hex(ta,2)}, `+
                                `#${hex(sr,2)}${hex(sg,2)}${hex(sb,2)}${hex(sa,2)}>`
                            break;
                        } //case 0xBF
                        default: str += `<UNK EF A3 ${hex(b2,2)}>`;
                    } //switch(b2)
                    break;
                } //case 0xEFA3
                //default: str += `<UNK ${hex(b,4)}>`;
                default: str += String.fromCharCode(b);
            }
            //str += `\\u${hex(b,4)}`;
        } //else if(b > 0x80)
        else str += String.fromCharCode(b);
    }
    return [str, offset];
}

/** Represents the contents of a .bin file under gametext directory.
 */
export default class GameTextBin {
    constructor(app, path) {
        this.app      = app;
        this.path     = path;
        this.game     = this.app.game;
        this.chars    = []; //character positions within font textures
        this.texts    = []; //text definitions
        this.strings  = {}; //actual text strings (offset => str)
        this.unkData  = []; //just full of 0xEE?
        this.textures = []; //texture images
    }

    async load() {
        this.rawData = (await get({
            path:         `${this.game.version}/disc/gametext/SwapHol/English.bin`,
            mimeType:     'application/octet-stream',
            responseType: 'arraybuffer',
        })).response;
        this.dataView = new DataView(this.rawData);

        //read character data
        const numCharStructs = this.dataView.getUint32(0);
        let offset = 4;
        for(let i=0; i<numCharStructs; i++) {
            this.chars.push(new CharacterStruct(this.app, this.dataView, offset));
            offset += 0x10;
        }

        //read texts
        const numTexts   = this.dataView.getUint16(offset); offset += 2;
        const strDataLen = this.dataView.getUint16(offset); offset += 2;
        console.log(`numTexts=${numTexts}, strDataLen=${strDataLen}, offset=${hex(offset)}`);
        for(let i=0; i<numTexts; i++) {
            this.texts.push(new GameTextStruct(this.app, this.dataView, offset));
            offset += 0xC;
        }

        //read string offsets
        const numStrings = this.dataView.getUint32(offset); offset += 4;
        const strOffsets = [];
        console.log(`numStrings=${numStrings}, offset=${hex(offset)}`);
        for(let i=0; i<numStrings; i++) {
            strOffsets.push(this.dataView.getUint32(offset)); offset += 4;
        }

        //read string data
        console.log(`Str data offset=${hex(offset)}`);
        const strOffs = offset;
        for(let i=0; i<numStrings; i++) {
            let str = '';
            let startOffs = strOffs + strOffsets[i];
            offset = startOffs;
            let sOffs = offset - strOffs;
            [str, offset] = readStr(this.dataView, offset);
            console.log(`Str @ ${hex(sOffs)}:`, str);
            this.strings[sOffs] = str;
        }
        offset = strOffs + strDataLen; //account for padding, etc

        //read unknown data
        const unkDataLen = this.dataView.getUint32(offset); offset += 4;
        console.log(`unDataLen=${unkDataLen}, offset=${hex(offset)}`);
        this.unkData = this.rawData.slice(offset, offset + unkDataLen); offset += unkDataLen;

        //read texture images
        while(true) {
            console.log(`Texture ${this.textures.length} offset=${hex(offset)}`);
            let tex = new FontTexture(this.app, this.dataView, offset);
            if(tex.bytes == null) break;
            this.textures.push(tex);
            offset += tex.bytes.byteLength + 8; //8 for header
        }
        console.log(`EOF at ${hex(offset)}`);

        //get phrase data
        for(let text of this.texts) {
            let idx = text.phrases;
            text.phrases = [];
            for(let i=0; i<text.numPhrases; i++) {
                let offs = strOffsets[idx+i];
                console.log(`Phrase ${i} idx = ${idx} offs = ${offs}: ${this.strings[offs]}`);
                text.phrases.push(this.strings[offs]);
            }
        }
    }
}
