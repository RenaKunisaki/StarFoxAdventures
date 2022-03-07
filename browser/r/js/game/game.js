import { getXml, int } from "../Util.js";
import GameBit from "../types/GameBit.js";
import GameObject from "../types/GameObject.js";
import DLL from "../types/DLL.js";
import { MapParser } from "./MapParser.js";
import Text from "./text/Text.js";
import parseWarpTab from "../types/Warptab.js";

export const MAP_CELL_SIZE = 640;
export const TEXT_LANGUAGES = ['English', 'French',
    'German', 'Italian', 'Japanese', 'Spanish']; //XXX move

//could move this to XML...
export const VERSION_INFO = {
    U0: {
        objNameOffs: 0x91, //offset of name in ObjData
        objNameLen:  11,
    },
    U1: {
        objNameOffs: 0x91, //offset of name in ObjData
        objNameLen:  11,
    },
    E0: {
        objNameOffs: 0x91, //offset of name in ObjData
        objNameLen:  11,
    },
    J0: {
        objNameOffs: 0x91, //offset of name in ObjData
        objNameLen:  11,
    },
    J1: {
        objNameOffs: 0x91, //offset of name in ObjData
        objNameLen:  11,
    },
}

export default class Game {
    /** Info and methods relating to the game itself.
     */
    constructor(app) {
        this.app       = app;
        this.version   = null;
        this.iso       = null;
        this.addresses = {}; //name => {address, count}
        this.bits      = null; //GameBits
        this.objCats   = null; //object categories
        this.dlls      = null;
        this.maps      = null;
        this.mapDirs   = null;
        this.warpTab   = null;
        this.texts     = null;

        this.setVersion('U0');
        this.app.onLanguageChanged(lang => this._loadTexts(lang));
    }

    async loadIso(iso) {
        this.iso = iso;

        let version;
        switch(this.iso.bootBin.gameCode) {
            case 'GSAE': version = 'U'; break;
            case 'GSAP': version = 'E'; break;
            case 'GSAJ': {
                if(this.iso.bootBin.gameName == '08 2002.05.17 E3_2002_StarFox')
                    version = 'K';
                else version = 'J';
                break;
            }
            default: {
                console.warn("Unrecognized game ID:", this.iso.bootBin.gameCode);
                version = '?';
            }
        }
        version += this.iso.bootBin.version.toString();
        console.log("Game version:", version);
        await this.setVersion(version);
    }

    async setVersion(version) {
        this.version  = version;
        this.bits     = null; //force redownload
        this._verInfo = VERSION_INFO[this.version];

        //get addresses
        await this.app.progress.update({subText:"Downloading addresses.xml..."});
        const addrsXml = await getXml(`data/${this.version}/addresses.xml`);
        if(addrsXml) {
            for(let elem of addrsXml.getElementsByTagName('address')) {
                this.addresses[elem.getAttribute('name')] = {
                    address: int(elem.getAttribute('address')),
                    type:    elem.getAttribute('type'),
                    count:   int(elem.getAttribute('count')),
                };
            }
        }

        await this._loadTexts(this.app.language);
        //await this.getBits();

        //get object categories
        this.objCats = {};
        await this.app.progress.update({subText:"Downloading objcats.xml..."});
        const objCatsXml = await getXml(`data/${version}/objcats.xml`);
        if(objCatsXml) {
            for(let elem of objCatsXml.getElementsByTagName('cat')) {
                this.objCats[parseInt(elem.getAttribute('id'))] =
                    elem.getAttribute('name');
            }
        }

        if(this.iso) {
            await this._loadDlls();
            await this._loadObjects();
            this.warpTab = parseWarpTab(this.app);
            await this._loadMaps();
        }
    }

    async getBits() {
        if(!this.bits) {
            await this.app.progress.update({subText:"Downloading gamebits.xml..."});
            this.bits = await this.app._getXml(GameBit,
                this.version, 'gamebits', 'bit');
            this.app.progress.hide();
        }
        return this.bits;
    }

    getMapAt(layer, x, z) {
        /** Get map by coordinates.
         *  Accepts raw coords (not divided by cell size).
         */
        if(!this.mapGrid) throw new Error("Maps not loaded yet");
        let res = this.mapGrid[layer];
        if(res) res = res[Math.floor(x / MAP_CELL_SIZE)];
        if(res) res = res[Math.floor(z / MAP_CELL_SIZE)];
        if(res) res = res.map;
        return res;
    }

    getObjName(defNo) {
        if(!this.objsTab) return '(no ISO)';
        const offs = this.objsTab.getUint32(defNo * 4);
        let   res  = '';
        const len  = this._verInfo.objNameLen;
        const base = this._verInfo.objNameOffs;
        for(let i=0; i<len; i++) {
            let b = this.objsBin.getUint8(offs+base+i);
            if(b >= 0x20 && b <= 0x7E) res += String.fromCharCode(b);
            else if(b == 0) break;
            else res += '?';
        }
        return res;
    }

    async _loadDlls() {
        await this.app.progress.update({subText:"Downloading dlls.xml..."});
        const xml = await getXml(`data/${this.version}/dlls.xml`);
        if(!xml) return;
        this.dllTableAddr = int(xml.getElementsByTagName('dlls')[0].
            getAttribute('tableAddress'));
        this.dlls = {};
        for(let elem of xml.getElementsByTagName('dll')) {
            const dll = new DLL(this.app, elem);
            this.dlls[dll.id] = dll;
        }
    }

    async _loadObjects() {
        this.objsTab = this.app.game.iso.getFile('/OBJECTS.tab').getData();
        this.objsBin = this.app.game.iso.getFile('/OBJECTS.bin').getData();

        //parse OBJINDEX.bin
        const objIndex = this.iso.getFile('/OBJINDEX.bin').getData();
        this.objIndex  = [];
        const revIndex = {};
        for(let i=0; i<objIndex.byteLength; i += 2) {
            if(!(i % 100)) {
                await this.app.progress.update({
                    subText:"Parsing OBJINDEX.bin...",
                    numSteps: objIndex.byteLength / 2,
                    stepsDone: i / 2,
                });
            }
            const idx = objIndex.getInt16(i);
            this.objIndex.push(idx);
            //file is padded with zeros so don't let those overwrite
            //the actual index zero
            if(idx >= 0 && revIndex[idx] == undefined) revIndex[idx] = i >> 1;
        }

        //parse OBJECTS.bin
        this.objects = [];
        for(let i=0; this.objsTab.getInt32((i+1)*4) >= 0; i++) {
            if(!(i % 100)) {
                await this.app.progress.update({
                    subText:"Parsing OBJECTS.bin...",
                    numSteps: this.objsTab.byteLength / 4, //approximate
                    stepsDone: i,
                });
            }
            const obj = new GameObject(this.app, i);
            if(revIndex[i] != undefined) obj.index = revIndex[i];
            this.objects.push(obj);
        }
    }

    async _loadMaps() {
        const parser = new MapParser(this);
        await parser.parse();
    }

    async _loadTexts(lang) {
        await this.app.progress.update({
            subText:"Downloading gametext...",
            numSteps: 1, stepsDone: 0,
        });
        const xml = await getXml(`data/${this.version}/gametext/${lang}.xml`);
        if(!xml) return;
        this.texts = {};
        let iText = 0;
        for(let eText of xml.getElementsByTagName('text')) {
            if(!(iText++ % 100)) {
                await this.app.progress.update({
                    subText:"Parsing gametext...",
                    numSteps: 1, stepsDone: 0,
                });
            }
            let text = Text.fromXml(eText);
            this.texts[text.id] = text;
        }
    }
}
