import { E, createElement } from "../../lib/Element.js";
import { downloadXml, hex } from "../../Util.js";
import GameTextXmlBuilder from "../../game/text/XmlBuilder.js";
import BinaryFile from "../../lib/BinaryFile.js";
import GameBit from "../../types/GameBit.js";
import { RomList } from "../app/ui/FileViewer/RomListViewer.js";

const XML = 'http://www.w3.org/1999/xhtml';

export default class GameBitsXmlBuilder {
    /** Generates gamebits.xml. */
    constructor(app) {
        this.app = app;
    }

    async build() {
        this.bitElems = [];
        await this.parseBitTable();
    }

    async parseBitTable() {
        const bitTable = this.app.game.iso.getFile('/BITTABLE.bin');
        this.app.progress.show({
            taskText: "Generating XML",
            subText:  "Parsing BITTABLE.bin...",
            numSteps: bitTable.size / 4, stepsDone: 0,
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
            let elem      = bit.toXml();
            xml.documentElement.appendChild(elem);
            this.bitElems.push(elem);
            if(!(i % 100)) await this.app.progress.update({stepsDone:i});
        }
    }

    async parseRomLists() {
        //build file list
        await this.app.progress.update({
            taskText:  "Generating XML",
            subText:   "Getting romlist file list...",
            numSteps:  1, stepsDone: 0,
        });
        const files = [];
        for(let file of this.app.game.iso.files) {
            if(file.isDir) continue;
            if(!file.path.endsWith('/romlist.zlb')) continue;
            files.push(file);
        }
    }
}
