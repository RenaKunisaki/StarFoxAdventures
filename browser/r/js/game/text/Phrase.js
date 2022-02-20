import { CommandBase, Command, CommandById } from "./Command.js";
import { createElement, E } from "../../lib/Element.js";
import { Language } from "./Language.js";

//build lookup table for fast access
const cmdByChr = {};
for(let [name, cls] of Object.entries(Command)) {
    cmdByChr[cls.chr] = {name:name, cls:cls};
}

export default class Phrase {
    /** One "phrase" in a GameText file.
     *  In the files, this is simply a string, but containing control codes
     *  that can have embedded null bytes.
     *  It's represented here as an array of strings and commands.
     */
    constructor(str=null, lang=Language.English) {
        this._lang = lang;
        this._str  = [];

        //we can accept a string, but it can't contain control codes,
        //since the presence of null bytes means we need to parse those
        //as they're being read from the file.
        if(typeof(str) == 'string') this._str.push(str);
        else if(str != null) this._str = str;
    }

    static fromFile(file, lang=Language.English) {
        /** Read Phrase from binary file.
         *  @param {BinaryFile} file file to read from.
         *  @param {Language} lang which language this phrase is in.
         *  @returns {Phrase} the Phrase object.
         *  @note Expects the file to be seeked to the beginning of
         *     this phrase's string data.
         */
        const str = [];
        let   buf = '';
        while(true) {
            let c = file.readUtf8Char();
            if(c == 0) break;
            else if(CommandById[c]) {
                if(buf.length > 0) str.push(buf);
                buf = '';
                const cls = CommandById[c];
                const data = file.readU16Array(cls.params.length);
                //console.log("cmd", c, "cls", cls, "data", data);
                str.push(new cls(...data));
            }
            else buf += String.fromCodePoint(c);
        }
        if(buf.length > 0) str.push(buf);
        return new Phrase(str, lang);
    }

    static fromXml(elem, lang=Language.English) {
        /** Read Phrase from XML <phrase> element. */
        const str = [];
        for(let ch of elem.childNodes) {
            if(ch.tagName == 'str') str.push(ch.textContent);
            else if(Command[ch.tagName]) {
                const cls = Command[ch.tagName];
                const params = [];
                for(let param of cls.params) {
                    params.push(ch.getAttribute(param));
                }
                str.push(new cls(...params));
            }
            else if(ch.tagName == undefined) continue; //text node (whitespace)
            else throw new TypeError(
                `Unexpected XML tag '${ch.tagName}' in <phrase>`);
        }
        return new Phrase(str, lang);
    }

    toString() {
        /** Convert to plain text.
         *  @returns {string} String representation.
         */
        const res = [];
        for(let str of this._str) {
            if(typeof(str) == 'string') res.push(str);
            else if(str instanceof CommandBase) res.push(str.toString());
            else throw new TypeError(
                `Unexpected object in GameText Phrase: ${str}`);
        }
        return res.join('');
    }

    toXml() {
        /** Convert to XML.
         *  @returns {Element} XML element.
         */
        const ePhrase = E.phrase();
        for(let str of this._str) {
            if(typeof(str) == 'string') {
                ePhrase.append(E.str(null, str));
            }
            else if(!(str instanceof CommandBase)) {
                throw new TypeError(
                    `Unexpected object in GameText Phrase: ${str}`);
            }
            else {
                const cmd  = cmdByChr[str.chr];
                const eCmd = createElement(cmd.name);
                for(let param of cmd.cls.params) {
                    eCmd.setAttribute(param, str[param]);
                }
                ePhrase.append(eCmd);
            }
        }
        return ePhrase;
    }

    toHtml(params) {
        /** Convert to HTML.
         *  @param {Object} params Parameters which may be changed by
         *     control codes.
         *  @returns {HTMLSpanElement} HTML representation.
         */
        if(params.font == undefined) params.font = 1;
        if(params.justify == undefined) params.justify = 'Left';
        if(params.color == undefined) params.color = [255,255,255,255];
        if(params.size == undefined) params.size = 100; //percent

        const res = E.span('phrase');
        let elem;
        const newElem = () => {
            const C = params.color;
            elem = E.span(`font${params.font} justify${params.justify}`);
            elem.style.color = `rgba(${C[0]}, ${C[1]}, ${C[2]}, ${C[3]/255})`;
            elem.style.fontSize = `${params.size}%`;
            res.append(elem);
        };
        newElem();

        for(let str of this._str) {
            if(typeof(str) == 'string') elem.append(str);
            else if(!(str instanceof CommandBase)) {
                throw new TypeError(
                    `Unexpected object in GameText Phrase: ${str}`);
            }
            else if(str instanceof Command.scale) {
                params.size = str.scale / 2.56; newElem();
            }
            else if(str instanceof Command.font) {
                params.font = str.id; newElem();
            }
            else if(str instanceof Command.justleft) {
                params.justify = 'Left'; newElem();
            }
            else if(str instanceof Command.justright) {
                params.justify = 'Right'; newElem();
            }
            else if(str instanceof Command.justcenter) {
                params.justify = 'Center'; newElem();
            }
            else if(str instanceof Command.justfull) {
                params.justify = 'Full'; newElem();
            }
            else if(str instanceof Command.color) {
                params.color = [str.r, str.g, str.b, str.a]; newElem();
            }
            else {
                //hack to display correct color for hints
                if(str instanceof Command.hint) params.hint = str.id;
                elem.append(E.span('command', str.toString()));
            }
        }
        return res;
    }
} //class Phrase

