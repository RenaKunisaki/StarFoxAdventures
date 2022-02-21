import { getXml, int } from "../Util.js";
import GameBit from "../types/GameBit.js";
import GameObject from "../types/GameObject.js";
import DLL from "../types/DLL.js";
import Map from "./map.js";
import Text from "./text/Text.js";
import parseMapGrid from "../types/MapGrid.js";
import parseWarpTab from "../types/Warptab.js";

export const MAP_CELL_SIZE = 640;
export const TEXT_LANGUAGES = ['English', 'French',
    'German', 'Italian', 'Japanese', 'Spanish']; //XXX move

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
        this.version = version;
        this.bits    = null; //force redownload

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
        const objIndex = this.iso.getFile('/OBJINDEX.bin').getData();
        this.objIndex = [];
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

        const objsTab = this.iso.getFile('/OBJECTS.tab').getData();
        this.objects = [];
        for(let i=0; objsTab.getInt32((i+1)*4) >= 0; i++) {
            if(!(i % 100)) {
                await this.app.progress.update({
                    subText:"Parsing OBJECTS.bin...",
                    numSteps: objsTab.byteLength / 4, //approximate
                    stepsDone: i,
                });
            }
            const obj = new GameObject(this.app, i);
            if(revIndex[i] != undefined) obj.index = revIndex[i];
            this.objects.push(obj);
        }
    }

    async _loadMaps() {
        await this.app.progress.update({
            subText:"Downloading maps.xml...",
            numSteps: 1, stepsDone: 0,
        });
        const xml = await getXml(`data/${this.version}/maps.xml`);
        this.maps = {};
        let nextId = -1; //use negative IDs for maps that don't have an ID
        const usedRomLists = {};
        if(xml) {
            for(let elem of xml.getElementsByTagName('map')) {
                const map = new Map(this.app, elem);
                let id = map.id;
                if(id == null) id = nextId--;
                this.maps[id] = map;
                usedRomLists[map.romListName] = true;
            }
        }

        //find romlist files not referenced by maps
        let iFile = 0;
        for(let file of this.iso.files) {
            iFile++;
            if(file.name.endsWith('.romlist.zlb')) {
                await this.app.progress.update({
                    subText:`Parsing ${file.name}...`,
                    numSteps: this.iso.files.length,
                    stepsDone: iFile,
                });
                let name = file.name.split('.')[0];
                if(!usedRomLists[name]) {
                    const map = new Map(this);
                    map.romListName = name;
                    this.maps[nextId--] = map;
                    usedRomLists[name] = true;
                }
            }
        }

        console.log("maps", this.maps);
        await this.app.progress.update({
            subText:"Parsing globalma.bin...",
            numSteps: 1, stepsDone: 0,
        });
        this.mapGrid = parseMapGrid(this.app);
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
