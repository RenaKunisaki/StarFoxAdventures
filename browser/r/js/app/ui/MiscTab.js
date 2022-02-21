import { E, createElement } from "../../lib/Element.js";
import { downloadXml, hex } from "../../Util.js";
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
        const file = this.app.game.iso.getFile('/BITTABLE.bin');
        this.app.progress.show({
            taskText: "Generating XML",
            subText:  "Parsing BITTABLE.bin...",
            numSteps: file.size / 4, stepsDone: 0,
        });


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
