import { E, clearElement, createElement } from "../lib/Element.js";
import { download, hex, prettyXml } from "../Util.js";
import GameTextFile from "../types/GameTextFile.js";

const XML = 'http://www.w3.org/1999/xhtml';

export default class GameTextXmlBuilder {
    /** Builds gametext.xml file */
    constructor(app) {
        this.app = app;
    }

    build() {
        this.texts = {}; //id => {lang => [phrase, phrase...]}
        this._readFiles();
        return this._genXml();
    }

    _readFiles() {
        //iterate all gametext files to collect all texts
        for(let file of this.app.game.iso.files) {
            if(file.isDir) continue;
            if(!file.path.startsWith('/gametext/')) continue;
            if(file.path.endsWith('.new')) continue; //skip leftover source files
            console.log("Reading", file.path);
            let textFile;
            try {
                textFile = new GameTextFile(this.app, file);
            }
            catch(ex) { //probably not a GameText file
                continue;
            }
            this._readFile(textFile);
        }
    }

    _readFile(textFile) {
        //extract texts from one gametext file
        for(let text of textFile.texts) {
            if(this.texts[text.id] == undefined) this.texts[text.id] = {};
            if(this.texts[text.id][text.language] == undefined) {
                this.texts[text.id][text.language] = [];
                for(let phrase of text.phrases) {
                    this.texts[text.id][text.language].push(phrase.str);
                }
            }
            else { //check if text is identical
                /* const n = Math.max(texts[text.id][text.lang].length,
                    text.phrases.length);
                for(let i=0; i<n; i++) {
                    const t1 = texts[text.id][text.lang][i];
                    const t2 = text.phrases[i].str;
                    if(t1 != t2) { //XXX this won't work...
                        console.log(`Text mismatch, ID 0x${hex(text.id,4)}`,
                            t1, t2);
                        break;
                    }
                } */
            }
        }
    }

    _genXml() {
        //generate the XML
        console.log("Generating XML");
        const xml = document.implementation.createDocument(XML, "gametext");
        for(let [id, text] of Object.entries(this.texts)) {
            xml.documentElement.appendChild(this._makeTextElem(id, text));
        }
        return xml;
    }

    _makeTextElem(id, text) {
        //make the element for one Text
        const eText = E.text({
            id:`0x${hex(id,4)}`,
            window: `0x${hex(text.window,2)}`,
            alignH: text.alignH,
            alignV: text.alignV,
        });

        for(let [lang, phrases] of Object.entries(text)) {
            const eLang = E.lang({id:lang});
            for(let phrase of phrases) {
                const ePhrase = E.phrase();
                for(let s of phrase) {
                    if(typeof(s) == 'string') ePhrase.append(s);
                    else if(s.cmd == 'str') ePhrase.append(s.str);
                    else {
                        const eCmd = createElement(s.cmd);
                        for(let [k,v] of Object.entries(s)) {
                            if(k != 'cmd') eCmd.setAttribute(k,v);
                        }
                        ePhrase.append(eCmd);
                    }
                }
                eLang.append(ePhrase);
            }
            eText.append(eLang);
        }

        return eText;
    }
}
