import {Dll}      from './Dll.js';
import GameObject from './GameObject.js';
import GameText   from './GameText.js';
import GameBit    from './GameBit.js';
import GameMap    from './GameMap.js';
import Model      from './Model.js';
import RomList    from './RomList.js';
import {get,int}  from './Util.js';

export default class Game {
    /** Represents the SFA game itself.
     *  This class is responsible for fetching the data files relevant
     *  to the particular game version it's given.
     */
    static getVersions() {
        //this method exists because my browser doesn't support
        //a class having fields
        return {
            K:  "Kiosk Demo",
            U0: "US V1.0",
            U1: "US V1.1",
            J0: "JP V1.0",
            J1: "JP V1.1",
            E0: "EU V1.0",
            E1: "EU V1.1",
        };
    }

    constructor(version) {
        /** Instantiate game.
         *  @param version which version of the game.
         */
        this.version = version;
    }

    async downloadInfo() {
        /** Download data files.
         */
        this.dlls     = await this._downloadFile('dlls',     Dll,        'dll');
        this.texts    = await this._downloadFile('gametext', GameText,   'text');
        this.models   = await this._downloadFile('models',   Model,      'model');
        this.objects  = await this._downloadFile('objects',  GameObject, 'object');
        this.bits     = await this._downloadFile('gamebits', GameBit,    'bit');
        this.maps     = await this._downloadFile('maps',     GameMap,    'map');
        this.objIndex = await this._getObjIndex();

        this.mapsByDir = {};
        for(let [id, map] of Object.entries(this.maps)) {
            this.mapsByDir[map.dirId] = map;
            this.mapsByDir[map.dirName] = map;
        }
    }

    async _downloadFile(name, cls, tag) {
        /** Download an XML file and construct a class instance from
         *  each instance of the given tag.
         */
        const data = (await get({
            path:     `${this.version}/${name}.xml`,
            mimeType: 'text/xml; charset=utf-8',
        })).responseXML;

        const res = {};
        for(let elem of data.getElementsByTagName(tag)) {
            let inst = new cls(this, elem);
            res[inst.id] = inst;
        }
        return res;
    }

    async _getObjIndex() {
        /** Download and parse OBJINDEX.bin.
         */
        const objIndex = (await get({
            path:         '/disc/OBJINDEX.bin',
            mimeType:     'application/octet-stream',
            responseType: 'arraybuffer',
        })).response;

        const objIdxView = new DataView(objIndex);
        const result = [];
        for(let i=0; i<objIndex.byteLength; i += 2) {
            result.push(objIdxView.getUint16(i));
        }
        return result;
    }

    async getRomList(name) {
        /** Download a romlist file.
         */
        //XXX need different disc path per version
        const data = (await get({
            path:         `/zlb/disc/${name}.romlist.zlb?offset=0&header=1`,
            mimeType:     'application/octet-stream',
            responseType: 'arraybuffer',
        })).response;
        return new RomList(this, data);
    }

    async _getAllocTags() {
        /** Download allocation tag file.
         *  The game provides an arbitrary tag value with each heap allocation.
         *  Amethyst mod replaces the tags with the address that called the
         *  allocator, giving more precise information.
         */
        const data = (await get({
            path:    `${this.version}/alloctags.xml`,
            mimeType:'text/xml; charset=utf-8',
        })).responseXML;
        this.allocTags = {};
        for(let tag of data.getElementsByTagName('tag')) {
            this.allocTags[int(tag.getAttribute('id'))] = tag.getAttribute('name');
        }
    }

    async _getStaticAddrs() {
        /** Download static address file.
         *  This is used in conjunction with the alloc tags file.
         *  It defines static pointers often used as function parameters,
         *  which wouldn't appear in a heap scan.
         */
        const data = (await get({
            path:    `${this.version}/staticaddrs.xml`,
            mimeType:'text/xml; charset=utf-8',
        })).responseXML;
        this.staticAddrs = {};
        for(let tag of data.getElementsByTagName('var')) {
            this.allocTags[int(tag.getAttribute('addr'))] = {
                name: tag.getAttribute('name'),
                type: tag.getAttribute('type'),
            };
        }
    }

    async getFuncParamRecords() {
        /** Download the function parameter records.
         */
        const data = (await get({
            path:     `${this.version}/params.xml`,
            mimeType: 'text/xml; charset=utf-8',
        })).responseXML;

        await this._getAllocTags();
        await this._getStaticAddrs();

        this.funcParams = {};
        for(let eFunc of data.getElementsByTagName('function')) {
            let addr  = int(eFunc.getAttribute('address'));
            let count = int(eFunc.getAttribute('count'));
            let records = [];
            for(let eRecord of eFunc.getElementsByTagName('record')) {
                let record = [];
                for(let eParam of eRecord.getElementsByTagName('param')) {
                    let param = {
                        reg:    int(eParam.getAttribute('reg')),
                        val:    int(eParam.getAttribute('val')),
                        tag:    int(eParam.getAttribute('tag')),
                        offset: int(eParam.getAttribute('offs')),
                        name:   null,
                        type:   null, //not a Pokemon
                    };
                    if(this.allocTags[param.tag]) {
                        param.name = this.allocTags[param.tag].name;
                        //param.type = this.allocTags[param.tag].type;
                    }
                    else if(this.staticAddrs[param.val]) {
                        param.name = this.staticAddrs[param.val].name;
                        param.type = this.staticAddrs[param.val].type;
                    }
                    record.push(param);
                }
                records.push(record);
            }
            this.funcParams[addr] = {
                count: count,
                records: records,
            };
        }
    }

}
