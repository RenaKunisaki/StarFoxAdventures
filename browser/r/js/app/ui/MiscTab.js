import { E, clearElement, createElement } from "../../lib/Element.js";
import { download, hex, prettyXml } from "../../Util.js";
import GameTextFile from "../../types/GameTextFile.js";

const XML = 'http://www.w3.org/1999/xhtml';

export default class MiscTab {
    /** Misc stuff mainly for development.
     */
    constructor(app) {
        this.app = app;
        document.getElementById('genGameTextXml').addEventListener('click',
            e => this._genGameTextXml());
    }

    _saveXml(file, name) {
        download(
            prettyXml(new XMLSerializer().serializeToString(file)),
            name+'.xml', 'application/xml');
    }

    _genGameTextXml() {
        //iterate all gametext files to collect all texts
        const texts = {}; //id => {lang => [phrase, phrase...]}
        for(let file of this.app.game.iso.files) {
            if(file.isDir) continue;
            if(!file.path.startsWith('/gametext/')) continue;
            console.log("Reading", file.path);
            let textFile;
            try {
                textFile = new GameTextFile(this.app, file);
            }
            catch(ex) { //probably not a GameText file
                continue;
            }
            for(let text of textFile.texts) {
                if(texts[text.id] == undefined) texts[text.id] = {};
                if(texts[text.id][text.language] == undefined) {
                    texts[text.id][text.language] = [];
                    for(let phrase of text.phrases) {
                        texts[text.id][text.language].push(phrase.str);
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

        //generate the XML
        console.log("Generating XML");
        const xml  = document.implementation.createDocument(XML, "gametext");
        for(let [id, text] of Object.entries(texts)) {
            const eText = E.text({id:`0x${hex(id,4)}`});
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
            xml.documentElement.appendChild(eText);
        }
        this._saveXml(xml, 'gametext');
    }
}
