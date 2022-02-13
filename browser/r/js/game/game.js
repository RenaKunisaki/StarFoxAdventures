import { getXml, int } from "../Util.js";
import GameBit from "../types/GameBit.js";
import GameObject from "../types/GameObject.js";
import DLL from "../types/DLL.js";
import Map from "./map.js";

export default class Game {
    /** Info and methods relating to the game itself.
     */
    constructor(app) {
        this.app     = app;
        this.version = null;
        this.iso     = null;
        this.bits    = null; //GameBits
        this.objCats = null; //object categories
        this.dlls    = null;
        this.dllTableAddr = null; //RAM address of g_dlls
    }

    async loadIso(iso) {
        this.iso = iso;

        let version;
        switch(this.iso.bootBin.gameCode) {
            case 'GSAE': version = 'U'; break;
            case 'GSAP': version = 'E'; break;
            case 'GSAJ': version = 'J'; break;
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

        //get addresses
        this.addresses = {};
        const addrsXml = await getXml(`data/${this.version}/addresses.xml`);
        for(let elem of addrsXml.getElementsByTagName('address')) {
            this.addresses[elem.getAttribute('name')] =
                int(elem.getAttribute('val'));
        }

        //get GameBits
        this.bits = await this.app._getXml(GameBit, version, 'gamebits', 'bit');

        //get object categories
        this.objCats = {};
        const objCatsXml = await getXml(`data/${version}/objcats.xml`);
        for(let elem of objCatsXml.getElementsByTagName('cat')) {
            this.objCats[parseInt(elem.getAttribute('id'))] =
                elem.getAttribute('name');
        }

        if(this.iso) {
            await this._loadDlls();
            this._loadObjects();
            await this._loadMaps();
        }
    }

    async _loadDlls() {
        const xml = await getXml(`data/${this.version}/dlls.xml`);
        this.dllTableAddr = int(xml.getElementsByTagName('dlls')[0].
            getAttribute('tableAddress'));
        this.dlls = {};
        for(let elem of xml.getElementsByTagName('dll')) {
            const dll = new DLL(this.app, elem);
            this.dlls[dll.id] = dll;
        }
    }

    _loadObjects() {
        const objsTab = this.iso.getFile('/OBJECTS.tab').getData();
        this.objects = [];
        for(let i=0; objsTab.getInt32((i+1)*4) >= 0; i++) {
            this.objects.push(new GameObject(this.app, i));
        }
    }

    async _loadMaps() {
        const xml = await getXml(`data/${this.version}/maps.xml`);
        this.maps = {};
        for(let elem of xml.getElementsByTagName('map')) {
            const map = new Map(this.app, elem);
            this.maps[map.id] = map;
        }
        console.log("maps", this.maps);
    }
}
