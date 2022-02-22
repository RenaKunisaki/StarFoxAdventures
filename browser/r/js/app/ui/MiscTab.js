import { E, createElement } from "../../lib/Element.js";
import { downloadXml, hex } from "../../Util.js";
import GameBitsXmlBuilder from "../../game/GameBitsXmlBuilder.js";
import GameTextXmlBuilder from "../../game/text/XmlBuilder.js";
import BinaryFile from "../../lib/BinaryFile.js";
import GameBit from "../../types/GameBit.js";

const XML = 'http://www.w3.org/1999/xhtml';

export default class MiscTab {
    /** Misc stuff mainly for development.
     */
    constructor(app) {
        this.app = app;
        document.getElementById('genGameBitsXml').addEventListener('click',
            async e => await this._genGameBitsXml());
        document.getElementById('genGameTextXml').addEventListener('click',
            async e => await this._genGameTextXml());
    }

    async _genGameBitsXml() {
        const xml = await ((new GameBitsXmlBuilder(this.app)).build());
        this.app.progress.hide();
        downloadXml(xml, 'gamebits.xml');
    }

    async _genGameTextXml() {
        this.app.progress.show({
            taskText: "Generating XML",
            numSteps: 1, stepsDone: 0,
        });
        const xml = await ((new GameTextXmlBuilder(this.app)).build());
        this.app.progress.hide();
        for(let [lang, file] of Object.entries(xml)) {
            downloadXml(file, lang);
        }
    }
}
