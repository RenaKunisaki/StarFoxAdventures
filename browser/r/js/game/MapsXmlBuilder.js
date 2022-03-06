import { E } from "../lib/Element.js";
import { hex } from "../Util.js";
import MapParser from "./MapParser.js";

const XML = 'http://www.w3.org/1999/xhtml';

export default class MapsXmlBuilder {
    /** Generates maps.xml. */
    constructor(app) {
        this.app = app;
    }
    async build() {
        this.app.progress.show({
            taskText: "Generating XML",
            subText:  "Parsing maps...",
            numSteps: 1, stepsDone: 0,
        });
        this.maps = await (new MapParser(this.app)).parse();
        this.xml  = document.implementation.createDocument(XML, "maps");
        for(let map of Object.values(this.maps)) {
            this.xml.documentElement.appendChild(await this.makeMapElem(map));
        }
        return this.xml;
    }
    async makeMapElem(map) {
        const attrs = {
            id:          `0x${hex(map.id,2)}`,
            dirId:       `0x${hex(map.dirId,2)}`,
            dir:         map.dirName,
            name:        map.name,
            type:        map.type,
            parent:      `0x${hex(map.parentId,2)}`,
            sizeX:       map.sizeX,
            sizeZ:       map.sizeZ,
            originX:     map.originX,
            originZ:     map.originZ,
            nBlocks:     map.nBlocks,
            romListSize: `0x${hex(map.romListSize)}`,
            objType:     `0x${hex(map.objType,4)}`,
            unk08:       `0x${hex(map.unk08,8)}`,
            unk0C:       map.unk0C.map(it => `0x${hex(it)}`).join(','),
            unk1D:       `0x${hex(map.unk1D,2)}`,
            unk1E:       `0x${hex(map.unk1E,4)}`,
        };
        if(map.romListName != undefined) {
            //no names in kiosk ver
            attrs.romList = map.romListName;
        }
        let delList = [];
        for(let [k,v] of Object.entries(attrs)) {
            if(v == undefined || v == 'undefined'
            || v == '0xundefined') delList.push(k);
        }
        for(let k of delList) delete attrs[k];

        const elem = E.map(attrs);
        return elem;
    }
}
