import { E, createElement } from "../../lib/Element.js";
import { downloadXml, hex } from "../../Util.js";
import GameTextXmlBuilder from "../../game/text/XmlBuilder.js";

export default class MiscTab {
    /** Misc stuff mainly for development.
     */
    constructor(app) {
        this.app = app;
        document.getElementById('genGameTextXml').addEventListener('click',
            e => this._genGameTextXml());
    }

    _genGameTextXml() {
        const xml = (new GameTextXmlBuilder(this.app)).build();
        for(let [lang, file] of Object.entries(xml)) {
            downloadXml(file, lang);
        }
    }
}
