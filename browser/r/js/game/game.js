import { getXml } from "../Util.js";
import GameBit from "../types/GameBit.js";
import GameObject from "../types/GameObject.js";
import DLL from "../types/DLL.js";

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

        this.bits = await this.app._getXml(GameBit, version, 'gamebits', 'bit');
        this.dlls = await this.app._getXml(DLL, version, 'dlls', 'dll');

        this.objCats = {};
        const objCatsXml = await getXml(`data/${version}/objcats.xml`);
        for(let elem of objCatsXml.getElementsByTagName('cat')) {
            this.objCats[parseInt(elem.getAttribute('id'))] = elem.getAttribute('name');
        }

        if(this.iso) {
            this._loadObjects();
        }
    }

    _loadObjects() {
        const objsTab = this.iso.getFile('/OBJECTS.tab').getData();
        this.objects = [];
        for(let i=0; objsTab.getInt32((i+1)*4) >= 0; i++) {
            this.objects.push(new GameObject(this.app, i));
        }
    }
}
