import { SaveGame } from "../types/SaveGame.js";
import { getXml } from "../Util.js";
//XXX move these
import { E } from "../lib/Element.js";
import FileSelect from "./ui/FileSelect.js";
import TabBar from "./ui/TabBar.js";
import SaveInfo from "./ui/SaveInfo.js";
import GameBits from "./ui/GameBits.js";
import GameBit from "../types/GameBit.js";

export default class App {
    constructor() {
        this.iso         = null; //the loaded ISO file
        this.saveGame    = null; //the loaded savegame file
        this.saveSlot    = null; //the selected slot
        this.saveSlotIdx = 0;
        this.gameBits    = null; //gamebits.xml
        this._callbacks  = {
            onIsoLoaded: [],
            onSaveLoaded: [],
            onSaveSlotChanged: [],
        };
    }

    async run() {
        this.ui = {
            fileSelect: new FileSelect(this),
            saveInfo:   new SaveInfo(this),
            gameBits:   new GameBits(this),
        };

        const tabs = {};
        for(let elem of document.getElementsByClassName('tabBody')) {
            tabs[elem.getAttribute('data-tab-name')] = elem;
            if(elem.getAttribute('data-needs-savegame')) {
                elem.append(E.div('notice', "Select a save file to examine."));
            }
            if(elem.getAttribute('data-needs-iso')) {
                elem.append(E.div('notice', "Select an ISO file to examine."));
            }
        }
        this.ui.tabs = new TabBar(tabs);
        document.getElementById('loading').replaceWith(this.ui.tabs.element);
    }

    async _getXml(cls, version, name, tag) {
        const data = await getXml(`data/${version}/${name}.xml`);
        const res  = {};
        for(let elem of data.getElementsByTagName(tag)) {
            let inst = new cls(this, elem);
            res[inst.id] = inst;
        }
        return res;
    }

    async getFilesForVersion(version) {
        this.gameBits = await this._getXml(GameBit, version, 'gamebits', 'bit');
        //console.log("gameBits=", this.gameBits);
    }

    async loadIso(file) {
        //load given ISO file (type File)
        //TODO
        this._doCallback('onIsoLoaded', file);
    }

    async loadSave(file) {
        //load given save (GCI) file (type File)
        await this.getFilesForVersion('U0'); //XXX
        this.saveGame = new SaveGame(this);
        await this.saveGame.load(file);
        this.saveSlot = this.saveGame.saves[this.saveSlotIdx];
        this._doCallback('onSaveLoaded', this.saveGame);
        this._doCallback('onSaveSlotChanged', this.saveSlot);
    }

    async useSaveSlot(slot) {
        //change active save slot
        if(this.saveGame == null) {
            throw new Error("Can't change save slot when no save file is loaded");
        }
        this.saveSlotIdx = slot;
        this.saveSlot = this.saveGame.saves[this.saveSlotIdx];
        this._doCallback('onSaveSlotChanged', this.saveSlot);
    }

    //callbacks
    onIsoLoaded(cb) { this._callbacks.onIsoLoaded.push(cb) }
    onSaveLoaded(cb) { this._callbacks.onSaveLoaded.push(cb) }
    onSaveSlotChanged(cb) { this._callbacks.onSaveSlotChanged.push(cb) }

    _doCallback(evt, ...args) {
        console.log("callback:", evt, args);
        for(let cb of this._callbacks[evt]) {
            try {
                cb(...args);
            }
            catch(ex) {
                console.error("Exception in callback", ex);
            }
        }
    }
} //class
