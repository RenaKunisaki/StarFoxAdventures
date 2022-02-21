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

        const data = new BinaryFile(file.getData());
        const xml  = document.implementation.createDocument(XML, "gametext");
        for(let i=0; i<file.size / 4; i++) {
            let bit       = new GameBit(this.app, null);
            const item    = data.readU32();
            bit.id        = i;
            bit.offset    = item >> 16;
            bit.size      = ((item >> 8) & 0x1F) + 1;
            const hasHint = ((item >> 13) & 1) != 0;
            bit.table     = (item >> 14) & 3;
            bit.hintId    = hasHint ? ((item & 0xFF) + 0xF4) : null;
            xml.documentElement.appendChild(bit.toXml());
            if(!(i % 100)) await this.app.progress.update({stepsDone:i});
        }
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
