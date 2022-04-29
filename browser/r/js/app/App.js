import { SaveGame } from "../game/save/SaveGame.js";
import RamDump from "../game/ram/RamDump.js";
import { getXml } from "../Util.js";
import StructParser from "../lib/Struct/StructParser.js";

import { E } from "../lib/Element.js";
import TaskProgress from "./ui/TaskProgress.js";
import FileList from "./ui/FileList.js";
import FileSelect from "./ui/FileSelect.js";
import FileViewer from "./ui/FileViewer/FileViewer.js";
import TabBar from "./ui/TabBar.js";
import SaveInfo from "./ui/SaveInfo.js";
import GameBits from "./ui/GameBits.js";
import ObjList from "./ui/ObjList.js";
import DllList from "./ui/DllList.js";
import MapList from "./ui/MapList.js";
import MapGrid from "./ui/MapGrid.js";
import MapViewer from "./ui/MapViewer/MapViewer.js";
import MiscTab from "./ui/MiscTab.js";
import RamInfo from "./ui/RamInfo.js";
import Warptab from "./ui/Warptab.js";
import TextList from "./ui/TextList.js";
import { ISO } from "../types/iso/iso.js";
import Game from "../game/Game.js";
import { Language } from "../game/text/Language.js";
import { StateError } from "./errors.js";

export default class App {
    constructor(parent) {
        this._callbacks  = {
            onIsoLoaded: [],
            onSaveLoaded: [],
            onRamLoaded: [],
            onSaveSlotChanged: [],
            onLanguageChanged: [],
        };
        this.progress    = new TaskProgress();
        this.progress.update({taskText: "Initializing..."});
        this.parent      = parent; //parent window's App instance
        this.saveGame    = null; //the loaded savegame file
        this.saveSlot    = null; //the selected slot
        this.saveSlotIdx = 0;
        this.ramDump     = null; //RAM dump file
        this.language    = 'English';
        this.game        = new Game(this);
        //this.progress.hide();
    }

    async run() {
        if(this.parent) {
            for(let elem of document.getElementsByClassName('tabBody')) {
                elem.style.display = 'none';
            }
            //XXX make a proxy
            this.game = this.parent.game;
            this.parent._childWindowLoaded();
        }
        else {
            this.ui = {
                fileList:   new FileList  (this.game),
                fileSelect: new FileSelect(this.game),
                saveInfo:   new SaveInfo  (this.game),
                gameBits:   new GameBits  (this.game),
                objList:    new ObjList   (this.game),
                dllList:    new DllList   (this.game),
                mapList:    new MapList   (this.game),
                mapGrid:    new MapGrid   (this.game),
                mapView:    new MapViewer (this.game),
                miscTab:    new MiscTab   (this.game),
                ramInfo:    new RamInfo   (this.game),
                warpTab:    new Warptab   (this.game),
                textList:   new TextList  (this.game),
            };

            this.progress.update({subText: "Downloading structs..."});
            this.types = new StructParser();
            await this.types.selfTest();

            const eLang = document.getElementById('language');
            for(let lang of Object.keys(Language)) {
                eLang.append(E.option(null, lang, {value:lang}));
            }
            eLang.addEventListener('change', e => this.setLanguage(eLang.value));

            const tabs = {};
            for(let elem of document.getElementsByClassName('tabBody')) {
                tabs[elem.getAttribute('data-tab-name')] = elem;
                if(elem.getAttribute('data-needs-savegame')) {
                    elem.append(E.div('notice', "Select a save file to examine."));
                }
                if(elem.getAttribute('data-needs-iso')) {
                    elem.append(E.div('notice', "Select an ISO file to examine."));
                }
                if(elem.getAttribute('data-needs-ram')) {
                    elem.append(E.div('notice', "Select a RAM dump file to examine."));
                }
            }
            this.ui.tabs = new TabBar(tabs);
            document.getElementById('loading').replaceWith(this.ui.tabs.element);
        }

        await this.setLanguage('English');
        this.ui.gameBits.refresh();
        this.progress.hide();
        this.ui.tabs.showTab('Main');
    }

    async setLanguage(lang) {
        this.language = lang;
        await this._doCallback('onLanguageChanged', lang);
    }

    openChildWindow(url=null) {
        /** Open a child window and wait for it to load.
         *  @param url The URL to open to.
         *  @return The new window object, or null on failure.
         *  @note The URL must point to another instance of this app;
         *     otherwise, the promise will never resolve.
         */
        if(url == null) url = window.location;
        const win = window.open(url);
        if(!win) {
            alert("Unable to open new window (popup blocker?)");
            return null;
        }
        return new Promise((resolve, reject) => {
            this._pWaitForChildWindow = () => resolve(win);
        });
    }

    _childWindowLoaded() {
        /** Called by child window once it's loaded. */
        this._pWaitForChildWindow();
        this._pWaitForChildWindow = null;
    }

    async showFileInNewWindow(file) {
        const win = await this.openChildWindow();
        console.log("Opened window", win);
        if(win) win.app.showFile(file);
        return win;
    }

    async _getXml(cls, version, name, tag) {
        try {
            const data = await getXml(`data/${version}/${name}.xml`);
            const res  = {};
            for(let elem of data.getElementsByTagName(tag)) {
                let inst = new cls(this, elem);
                res[inst.id] = inst;
            }
            return res;
        }
        catch(err) {
            console.error("Error loading XML", version, name);
            throw err;
        }
    }

    async loadIso(file) {
        //load given ISO file (type File)
        await this.progress.update({subText: "Parsing..."});
        const iso = new ISO(this).readBuffer(await file.arrayBuffer());
        await this.game.loadIso(iso);
        console.log("ISO loaded", this.game.iso);

        //trigger callbacks
        this._doCallback('onIsoLoaded', this.game.iso);
        this.progress.hide();
    }

    async loadSave(file) {
        //load given save (GCI) file (type File)
        this.saveSlotIdx = 0;
        this.saveGame = new SaveGame(this.game);
        await this.saveGame.load(file);

        //this will trigger downloading gamebits.xml
        if(this.game.version == null)
            await this.game.setVersion(this.saveGame._version);

        this.saveGame.getGameBits();

        this.saveSlot = this.saveGame.saves[this.saveSlotIdx];
        this._doCallback('onSaveLoaded', this.saveGame);
        this._doCallback('onSaveSlotChanged', this.saveSlot);
    }

    async useSaveSlot(slot) {
        //change active save slot
        if(this.saveGame == null) {
            throw new StateError("Can't change save slot when no save file is loaded");
        }
        this.saveSlotIdx = slot;
        this.saveSlot = this.saveGame.saves[this.saveSlotIdx];
        this._doCallback('onSaveSlotChanged', this.saveSlot);
    }

    async loadRam(file) {
        //load given RAM dump file
        this.ramDump = new RamDump(this.game);
        await this.ramDump.load(file);
        this._doCallback('onRamLoaded', this.ramDump);
    }

    showFile(file) {
        //Display contents of file. Used for child window.
        document.title = file.path;
        const viewer = new FileViewer(this.game, file);
        document.getElementById('loading').replaceWith(viewer.element);
    }

    //callbacks (XXX auto-generate?)
    onIsoLoaded(cb) { this._callbacks.onIsoLoaded.push(cb) }
    onSaveLoaded(cb) { this._callbacks.onSaveLoaded.push(cb) }
    onRamLoaded(cb) { this._callbacks.onRamLoaded.push(cb) }
    onSaveSlotChanged(cb) { this._callbacks.onSaveSlotChanged.push(cb) }
    onLanguageChanged(cb) { this._callbacks.onLanguageChanged.push(cb) }

    async _doCallback(evt, ...args) {
        console.log("callback:", evt, args);
        for(let cb of this._callbacks[evt]) {
            try {
                let r = cb(...args);
                if(r instanceof Promise) await r;
            }
            catch(ex) {
                console.error("Exception in callback", ex);
            }
        }
    }
} //class
