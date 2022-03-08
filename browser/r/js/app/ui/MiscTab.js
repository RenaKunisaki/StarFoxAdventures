import { downloadXml, hex } from "../../Util.js";
import GameBitsXmlBuilder from "../../game/GameBitsXmlBuilder.js";
import GameTextXmlBuilder from "../../game/text/XmlBuilder.js";
import MapsXmlBuilder from "../../game/map/MapsXmlBuilder.js";

const type = 'application/xml';

export default class MiscTab {
    /** Misc stuff mainly for development.
     */
    constructor(app) {
        this.app = app;
        document.getElementById('genGameBitsXml').addEventListener('click',
            async e => await this._genGameBitsXml());
        document.getElementById('genGameTextXml').addEventListener('click',
            async e => await this._genGameTextXml());
        document.getElementById('genMapsXml').addEventListener('click',
            async e => await this._genMapsXml());
    }

    async _genGameBitsXml() {
        const xml = await ((new GameBitsXmlBuilder(this.app.game)).build());
        this.app.progress.hide();
        downloadXml(xml, 'gamebits.xml', type,
            document.getElementById('genPrettyXml').checked);
    }

    async _genGameTextXml() {
        this.app.progress.show({
            taskText: "Generating XML",
            numSteps: 1, stepsDone: 0,
        });
        const xml = await ((new GameTextXmlBuilder(this.app.game)).build());
        this.app.progress.hide();
        for(let [lang, file] of Object.entries(xml)) {
            downloadXml(file, lang, type,
                document.getElementById('genPrettyXml').checked);
        }
    }

    async _genMapsXml() {
        const xml = await ((new MapsXmlBuilder(this.app.game)).build());
        this.app.progress.hide();
        downloadXml(xml, 'maps.xml', type,
            document.getElementById('genPrettyXml').checked);
    }
}
