import { E, createElement } from "../../lib/Element.js";
import { downloadXml, hex } from "../../Util.js";
import GameTextXmlBuilder from "../../game/text/XmlBuilder.js";

export default class MiscTab {
    /** Misc stuff mainly for development.
     */
    constructor(app) {
        this.app = app;
        document.getElementById('genGameTextXml').addEventListener('click',
            async e => await this._genGameTextXml());
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
