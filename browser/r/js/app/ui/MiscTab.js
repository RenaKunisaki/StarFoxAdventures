import { E, createElement } from "../../lib/Element.js";
import { downloadXml, hex } from "../../Util.js";
//import GameTextXmlBuilder from "../../game/text/XmlBuilder.js";

export default class MiscTab {
    /** Misc stuff mainly for development.
     */
    constructor(app) {
        this.app = app;
        document.getElementById('genGameTextXml').addEventListener('click',
            e => this._genGameTextXml());
    }

    _genGameTextXml() {
        //downloadXml((new GameTextXmlBuilder(this.app)).build(), 'gametext');
    }
}
