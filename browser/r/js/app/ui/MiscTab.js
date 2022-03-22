import { assertType, downloadXml, getXml, hex, int } from "../../Util.js";
import { E } from "../../lib/Element.js";
import GameBitsXmlBuilder from "../../game/GameBitsXmlBuilder.js";
import GameTextXmlBuilder from "../../game/text/XmlBuilder.js";
import MapsXmlBuilder from "../../game/map/MapsXmlBuilder.js";
import Game from "../../game/Game.js";

const type = 'application/xml';

export default class MiscTab {
    /** Misc stuff mainly for development.
     */
    constructor(game) {
        this.game = assertType(game, Game);
        this.app  = game.app;
        document.getElementById('genGameBitsXml').addEventListener('click',
            async e => await this._genGameBitsXml());
        document.getElementById('genGameTextXml').addEventListener('click',
            async e => await this._genGameTextXml());
        document.getElementById('genMapsXml').addEventListener('click',
            async e => await this._genMapsXml());
        document.getElementById('genDllsXml').addEventListener('click',
            async e => await this._genDllsXml());
    }

    async _genGameBitsXml() {
        const xml = await ((new GameBitsXmlBuilder(this.game)).build());
        this.app.progress.hide();
        downloadXml(xml, 'gamebits.xml', type,
            document.getElementById('genPrettyXml').checked);
    }

    async _genGameTextXml() {
        this.app.progress.show({
            taskText: "Generating XML",
            numSteps: 1, stepsDone: 0,
        });
        const xml = await ((new GameTextXmlBuilder(this.game)).build());
        this.app.progress.hide();
        for(let [lang, file] of Object.entries(xml)) {
            downloadXml(file, lang, type,
                document.getElementById('genPrettyXml').checked);
        }
    }

    async _genMapsXml() {
        const xml = await ((new MapsXmlBuilder(this.game)).build());
        this.app.progress.hide();
        downloadXml(xml, 'maps.xml', type,
            document.getElementById('genPrettyXml').checked);
    }

    async _genDllsXml() {
        //XXX remove this method.
        this.app.progress.show({
            taskText: "Building",
            subText: "Downloading dlls.xml...",
            numSteps: 1, stepsDone: 0,
        });
        const xml = await getXml(`data/${this.game.version}/dlls.xml`);

        const dlls = xml.getElementsByTagName('dll');
        for(let i=0; i<dlls.length; i++) {
            if(!(i & 16)) {
                await this.app.progress.update({
                    subText:"Generating...",
                    numSteps: dlls.length, stepsDone: i+1,
                });
            }
            const eDll    = dlls[i];
            const eParams = eDll.getElementsByTagName('objparams').item(0);
            if(!eParams) continue;

            //read the existing params
            const params = [];
            const notes  = [];
            const elems  = eParams.getElementsByTagName('param');
            for(let eParam of elems) {
                if(eParam.getAttribute('type') == 'note') {
                    notes.push(eParam.textContent);
                    continue;
                }
                let param = {
                    name:   eParam.getAttribute('name'),
                    offset: int(eParam.getAttribute('offset')),
                    type:   eParam.getAttribute('type'),
                    description: null,
                    notes: [],
                };
                if(param.name == null) delete param.name;
                let desc = eParam.getElementsByTagName('description');
                if(desc && desc[0]) param.description = desc[0].textContent;
                for(let eNote of eParam.getElementsByTagName('note')) {
                    param.notes.push(eNote.textContent);
                }
                params.push(param);
            }

            while(elems.length) eParams.removeChild(elems[0]);

            //add notes
            for(let note of notes) {
                eParams.append(E.note(null, note));
            }

            //replace them with field elements
            for(let param of params) {
                const attrs = {
                    type: param.type,
                    offset: `0x${hex(param.offset)}`
                };
                if(param.name) attrs.name = param.name;
                let eField = E.field(attrs);
                if(param.description) {
                    eField.append(E.description(null, param.description));
                }
                for(let note of param.notes) {
                    eField.append(E.note(null, note));
                }
                eParams.append(eField);
            }
        }
        downloadXml(xml, 'dlls.xml', type,
            document.getElementById('genPrettyXml').checked);
        this.app.progress.hide();
    }
}
