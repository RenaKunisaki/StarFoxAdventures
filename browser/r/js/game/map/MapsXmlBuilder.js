import { E } from "../../lib/Element.js";
import { hex } from "../../Util.js";
import { MapParser } from "./MapParser.js";

const XML = 'http://www.w3.org/1999/xhtml';

export default class MapsXmlBuilder {
    /** Generates maps.xml. */
    constructor(app) {
        this.app = app;
    }
    async build() {
        this.maps = await (new MapParser(this.app)).parse();
        this.xml  = document.implementation.createDocument(XML, "maps");
        for(let map of Object.values(this.maps)) {
            this.xml.documentElement.appendChild(await this.makeMapElem(map));
        }
        return this.xml;
    }
    async makeMapElem(map) {
        const attrs = {
            id:      `0x${hex(map.id,2)}`,
            dirId:   `0x${hex(map.dirId,2)}`,
            romList: map.romListName,
            isUsed:  map.isUsed,
        };
        if(attrs.isUsed === true) attrs.isUsed = '1';
        else if(attrs.isUsed === false) attrs.isUsed = '0';
        let delList = [];
        for(let [k,v] of Object.entries(attrs)) {
            if(v == undefined || v == 'undefined'
            || v == '0xundefined') delList.push(k);
        }
        for(let k of delList) delete attrs[k];

        const elem = E.map(attrs);
        if(map.description) elem.append(E.description(null, map.description));
        return elem;
    }
}
