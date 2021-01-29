import {Dll}       from './Dll.js';
import AnimCurve   from './AnimCurve.js';
import GameObject  from './GameObject.js';
import ObjCategory from './ObjCategory.js';
import GameText    from './GameText.js';
import GameBit     from './GameBit.js';
import GameMap     from './GameMap.js';
import Model       from './Model.js';
import RomList     from './RomList.js';
import {get,getBin,getXml,int,float,hex,getDescriptionAndNotes} from './Util.js';

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
        this.dlls      = await this._downloadFile('dlls',     Dll,        'dll');
        this.texts     = await this._downloadFile('gametext', GameText,   'text');
        this.models    = await this._downloadFile('models',   Model,      'model');
        this.objects   = await this._downloadFile('objects',  GameObject, 'object');
        this.objCats   = await this._downloadFile('objcats',  ObjCategory,'cat');
        this.bits      = await this._downloadFile('gamebits', GameBit,    'bit');
        this.maps      = await this._downloadFile('maps',     GameMap,    'map');
        this.objIndex  = await this._getObjIndex();
        this.sequences = await this._getSeqData();
        this.warps     = await this._getWarps();
        this._objUniqueIds = null;

        this.mapsByDir = {};
        if(this.maps) {
            for(let [id, map] of Object.entries(this.maps)) {
                this.mapsByDir[map.dirId] = map;
                this.mapsByDir[map.dirName] = map;
            }
        }
    }

    getObjectById(id) {
        /** Get an object def by its defNo.
         *  @param id the defNo ID. Can be negative to avoid remapping by
         *  OBJINDEX.bin, as in the game code.
         *  @return the objDef.
         */
        if(id < 0) id = -id;
        else id = this.objIndex[id];
        return this.objects[id];
    }

    async getObjectByUniqueId(id) {
        /** Get an object by its unique ID.
         */
        if(!this._objUniqueIds) {
            console.log("Downloading object ID list...");
            const data = await getXml(`${this.version}/objids.xml`);
            this._objUniqueIds = {};
            for(let elem of data.getElementsByTagName('obj')) {
                let obj = {
                    id:      int(  elem.getAttribute('id')),
                    romlist:       elem.getAttribute('romlist'),
                    idx:     int(  elem.getAttribute('idx')),
                    offset:  int(  elem.getAttribute('offset')),
                    type:    int(  elem.getAttribute('type')),
                    real:    int(  elem.getAttribute('real')),
                    name:          elem.getAttribute('name'),
                    acts:    int(  elem.getAttribute('acts')),
                    bound:   int(  elem.getAttribute('bound')),
                    slot:    int(  elem.getAttribute('slot')),
                    flags:   int(  elem.getAttribute('flags')),
                    x:       float(elem.getAttribute('x')),
                    y:       float(elem.getAttribute('y')),
                    z:       float(elem.getAttribute('z')),
                }
                //if(this._objUniqueIds[obj.id]) {
                //    console.warn(`Unique ID 0x${hex(obj.id, 8)} is not unique:`,
                //        obj, this._objUniqueIds[obj.id]);
                //}
                this._objUniqueIds[obj.id] = obj;
            }
        }
        return this._objUniqueIds[id];
    }

    async _downloadFile(name, cls, tag) {
        /** Download an XML file and construct a class instance from
         *  each instance of the given tag.
         */
        try {
            const data = await getXml(`${this.version}/${name}.xml`);
            const res = {};
            for(let elem of data.getElementsByTagName(tag)) {
                let inst = new cls(this, elem);
                res[inst.id] = inst;
            }
            return res;
        }
        catch(ex) {
            console.error("Error downloading", name, ex);
            return null;
        }

    }

    async _getObjIndex() {
        /** Download and parse OBJINDEX.bin.
         */
        const objIndex   = await getBin(`${this.version}/disc/OBJINDEX.bin`);
        const objIdxView = new DataView(objIndex);
        const result = [];
        for(let i=0; i<objIndex.byteLength; i += 2) {
            result.push(objIdxView.getUint16(i));
        }
        return result;
    }

    async _getSeqData() {
        /** Download and parse OBJSEQ.bin.
         *  XXX this is obsolete, only used for objinfo, should be changed there
         */
        const seqTab = await getBin(`${this.version}/disc/OBJSEQ.tab`);
        const seqBin = await getBin(`${this.version}/disc/OBJSEQ.bin`);

        const vTab = new DataView(seqTab);
        const vBin = new DataView(seqBin);
        const result = [];
        for(let i=0; i<vTab.byteLength; i += 2) {
            const offs = vTab.getUint16(i) * 8;
            const next = vTab.getUint16(i+2) * 8;
            if(next == 0xFFFF * 8) break;
            const seq  = [];
            for(let j=offs; j<next; j += 8) {
                seq.push({
                    offset: j,
                    objId:  vBin.getUint32(j),
                    flags:  vBin.getUint16(j+4),
                    defNo:  vBin.getUint16(j+6),
                });
            }
            result.push(seq);
        }
        return result;
    }

    async _getWarps() {
        /** Download and parse warptab.xml.
         */
        const warpsXml = await getXml(`${this.version}/warptab.xml`);
        const warps = {};
        for(let elem of warpsXml.getElementsByTagName('warp')) {
            const warp = {
                id:    int(elem.getAttribute('id')),
                x:   float(elem.getAttribute('x')),
                y:   float(elem.getAttribute('y')),
                z:   float(elem.getAttribute('z')),
                layer: int(elem.getAttribute('layer')),
                angle: int(elem.getAttribute('rot')),
                name:      elem.getAttribute('name'),
            };
            getDescriptionAndNotes(warp, elem);
            if(warp.x != 0 || warp.y != 0 || warp.z != 0) warps[warp.id] = warp;
        }
        return warps;
    }

    async getSequence(map, id) {
        /** Get sequence data from a directory.
         */
        const origId = Math.abs(id);
        id = map.lookupSequence(id);
        console.log(`Seq ID 0x${hex(origId,4)} => 0x${hex(id,4)} in ${map.dirName}`);

        const V = this.version;
        const seqsXml = await getXml(`${V}/objseqs.xml`);

        //read curves
        let curve = null;
        let offs  = map.animCurvTab.getUint32(id*4);
        let next  = map.animCurvTab.getUint32((id+1)*4);
        console.log(`Seq offs=0x${hex(offs,8)} next=0x${hex(next,8)}`);
        if(offs != 0xFFFFFFFF && next != 0xFFFFFFFF && offs & 0x80000000) {
            const len = (next & 0xFFFFFF) - (offs & 0xFFFFFF);
            curve = new AnimCurve(this, map.animCurvBin, offs & 0xFFFFFF, len);
            curve.id  = origId;
            curve.idx = id;
            const eSeq = seqsXml.getElementById(`0x${hex(origId,4)}`);
            if(eSeq) {
                curve.name = eSeq.getAttribute('name');
                getDescriptionAndNotes(curve, eSeq);
            }
        }

        //read objseqs
        /* console.log("Reading OBJSEQ...");
        const objSeqs = {};
        offs = objSeqTab.getUint16(0);
        for(let iSeq=1; iSeq<objSeqTab.byteLength/2; iSeq++) {
            let next = objSeqTab.getUint16(iSeq*2);
            if(offs == 0xFFFF || next == 0xFFFF) break;
            const objs = [];
            for(let iObj=0; iObj<(next-offs); iObj++) {
                const offsObj = offs * 8;
                objs.push({
                    uniqueId: objSeqBin.getInt32(offsObj),
                    flags:    objSeqBin.getUint16(offsObj+4),
                    defNo:    objSeqBin.getUint16(offsObj+6),
                });
            }
            objSeqs[iSeq-1] = objs;
        } */

        /* return {
            curves:   curves,
            objSeqs:  objSeqs,
            objSeq2C: objSeq2C,
        }; */
        return curve;
    }

    async getRomList(id, name) {
        /** Download a romlist file by ID or name.
         */
        let data;
        if(this.version == 'K') { //romlists are in MAPS.bin
            const tab  = new DataView(await getBin(`${this.version}/disc/MAPS.tab`));
            //const bin  = new DataView(await getBin(`${this.version}/disc/MAPS.bin`));
            const offs = tab.getUint32((id*7*4)+(6*4));
            data = await getBin(
                `/zlb/${this.version}/disc/MAPS.bin?offset=${offs}&header=1`);
        }
        else {
            if(name == null || name == undefined) {
                name = this.maps[id].romlistName;
            }
            if(name == null || name == undefined) {
                throw new Error("Romlist not found");
            }
            data = await getBin(
                `/zlb/${this.version}/disc/${name}.romlist.zlb?offset=0&header=1`);
        }
        return new RomList(this, data);
    }

    async _getAllocTags() {
        /** Download allocation tag file.
         *  The game provides an arbitrary tag value with each heap allocation.
         *  Amethyst mod replaces the tags with the address that called the
         *  allocator, giving more precise information.
         */
        const data = await getXml(`${this.version}/alloctags.xml`);
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
        const data = await getXml(`${this.version}/staticaddrs.xml`);
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
        const data = await getXml(`${this.version}/params.xml`);
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
