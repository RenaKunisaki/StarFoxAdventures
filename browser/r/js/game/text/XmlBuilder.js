import { E } from "../../lib/Element.js";
import { hex } from "../../Util.js";
import { Language, LangById } from "./Language.js";
import BinaryReader from "./BinaryReader.js";

const XML = 'http://www.w3.org/1999/xhtml';

export default class GameTextXmlBuilder {
    /** Builds gametext.xml file */
    constructor(app) {
        this.app = app;
    }

    async build() {
        this.texts = {}; //lang => {id => Text}
        for(let lang of Object.keys(Language)) {
            this.texts[lang] = {};
        }
        await this._readFiles();
        return this._genXml();
    }

    async _readFiles() {
        //iterate all gametext files to collect all texts

        //build list of files
        await this.app.progress.update({
            taskText:  "Generating XML",
            subText:   "Getting file list...",
            numSteps:  1,
            stepsDone: 0,
        });
        const files = [];
        for(let file of this.app.game.iso.files) {
            if(file.isDir) continue;
            if(!file.path.startsWith('/gametext/')) continue;
            if(file.path.endsWith('.new')) continue; //skip leftover source files
            files.push(file);
        }

        let iFile = 0;
        for(let file of files) {
            iFile++;
            await this.app.progress.update({
                taskText:  "Generating XML",
                subText:   file.path,
                numSteps:  files.length,
                stepsDone: iFile,
            });
            let textFile;
            try {
                textFile = new BinaryReader(this.app, file);
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
            const texts = this.texts[LangById[text.language]];
            console.assert(texts);
            if(!texts[text.id]) texts[text.id] = text;
        }
    }

    _genXml() {
        //generate the XML
        console.log("Generating XML");
        const xml = {};
        for(let lang of Object.keys(Language)) {
            xml[lang] = document.implementation.createDocument(XML, "gametext");
            xml[lang].documentElement.setAttribute('language', lang);
        }
        for(let [lang, texts] of Object.entries(this.texts)) {
            for(let [id, text] of Object.entries(texts)) {
                xml[lang].documentElement.appendChild(
                    this._makeTextElem(id, text));
            }
        }
        return xml;
    }

    _makeTextElem(id, text) {
        //make the element for one Text
        const eText = E.text({
            id:     `0x${hex(id,4)}`,
            window: `0x${hex(text.window,2)}`,
            alignH: text.align[0],
            alignV: text.align[1],
        });

        for(let phrase of text.phrases) {
            eText.append(phrase.toXml());
        }

        return eText;
    }
}
