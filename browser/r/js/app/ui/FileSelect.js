import Game from "../../game/Game.js";
import { E, clearElement } from "../../lib/Element.js";
import { assertType, Table } from "../../Util.js";

export default class FileSelect {
    /** The UI for selecting an ISO and save file.
      */
    constructor(game) {
        this.game = assertType(game, Game);
        this.app  = game.app;

        this._makeIsoInput();
        this._makeSaveInput();
        this._makeSlotSelect(null);
        this.element = document.getElementById('tab-file-select');
        this.app.onSaveLoaded(save => this._onSaveLoaded(save));
        //this.app.onSaveSlotChanged(slot => this._onSaveSlotChanged(slot));
        this.app.onIsoLoaded(iso => this._showIsoInfo(iso));
    } //constructor

    _makeIsoInput() {
        //build the ISO file input field.
        const eProgress = document.getElementById('fileIsoProgress');
        this.eIso = document.getElementById('fileIso');
        this.eIso.addEventListener('change', async e => {
            let elem = document.getElementById('isoFileBeforeLoadInfo');
            if(elem) elem.style.display = 'none';
            elem = document.getElementById('selectedIsoInfo');
            if(elem) {
                elem.style.display = 'block';
                clearElement(elem);
                elem.append(E.div('info', "Loading..."));
            }
            this.app.progress.show();
            await this.app.progress.update({
                taskText: "Reading ISO",
                subText: "Reading file...",
                stepsDone: 0,
                numSteps: 1,
            });
            try {
                await this.app.loadIso(this.eIso.files[0]);
            }
            catch(err) {
                clearElement(elem);
                elem.append(E.div('error', err.toString()));
                throw err;
            }
        }, false);
        this.eIso.addEventListener('progress', e => {
            console.log("progress", e);
            const task = {};
            if(e.lengthComputable) {
                task.numSteps = e.total;
                task.stepsDone = e.loaded;
                eProgress.innerText = `${((e.loaded/e.total)*100).toFixed(2)}%`;
            }
            this.app.progress.update(task);
        }, false);
    }

    _makeSaveInput() {
        //build the save file input field.
        this.eSave = document.getElementById('fileSave');
        this.eSave.addEventListener('change', async e => {
            const elem = document.getElementById('selectedSaveInfo');
            clearElement(elem);
            elem.append(E.div('info', "Loading..."));
            try {
                await this.app.loadSave(this.eSave.files[0]);
            }
            catch(err) {
                clearElement(elem);
                elem.append(E.div('error', err.toString()));
                throw err;
            }
        }, false);
    }

    _makeSlotSelect(save) {
        //Build the save slot selector.
        //save: SaveGame
        let slots = document.getElementById('saveSlot');
        clearElement(slots);
        if(save == null) {
            slots.append(E.option(null, "(no savedata loaded)"));
        }
        else {
            for(let slot of save.saves) {
                let name = "(empty)", percent = '  0', time = '00h 00m 00s';
                if(!slot.isEmpty) {
                    name    = slot.name;
                    percent = (slot.completion * 100).toFixed(1).padStart(3);
                    time    = slot.playTime;
                }
                slots.append(E.option(null,
                    `#${slot.index+1}: ${name} ${percent}% ${time}`,
                    {value:slot.index}));
            }
        }
        if(this.eSlot) this.eSlot.replaceWith(slots);
        this.eSlot = slots;
        this.eSlot.addEventListener('change', e => {
            this.app.useSaveSlot(this.eSlot.value);
        }, false);
    }

    _onSaveLoaded(save) {
        //called when app loads a new save file.
        //save: SaveGame
        document.getElementById('saveSlotContainer').style.display = 'block';
        document.getElementById('saveFileBeforeLoadInfo').style.display = 'none';
        this._showSaveInfo(save);
        this._makeSlotSelect(save);
    } //_onSaveLoaded

    _showIsoInfo(iso) {
        //update the Selected File pane with the given ISO.
        const elem = document.getElementById('selectedIsoInfo');
        clearElement(elem);
        elem.append(
            E.table(
                ...Table(
                    ["Title",   iso.bootBin.gameName],
                    ["Game ID", iso.bootBin.gameCode],
                    ["Company", iso.bootBin.company],
                    ["Disc #",  iso.bootBin.discNo],
                    ["Version", iso.bootBin.version],
                )
            )
        );
        if(!iso.bootBin.gameCode.startsWith('GSA')) {
            elem.append(E.div('error', "Unsupported game"));
        }
    }

    _showSaveInfo(save) {
        //update the Selected File pane with the given save.
        const elem = document.getElementById('selectedSaveInfo');
        clearElement(elem);
        if(save.gciHeader) {
            const header = save.gciHeader;
            //GC epoch is 2000-01-01, and JS dates want milliseconds
            const lastMod = new Date((header.lastModified + 946702800) * 1000);
            elem.append(
                E.table(
                    ...Table(
                        ["Game ID", header.gameCode],
                        ["Company", header.company],
                        ["File Name", header.fileName],
                        ["Last Modified", lastMod.toISOString()],
                        ["Copy Count", header.copyCount],
                        //XXX comment string
                    )
                )
            );
            if(!save.gciHeader.gameCode.startsWith('GSA')) {
                elem.append(E.div('error', "Unsupported game"));
            }
            else if(save._version != this.game.version
            && this.game.iso) {
                elem.append(E.div('notice', "Version doesn't match ISO"));
            }
            else if(!this.game.iso) this.game.setVersion(save._version);
        }
        else {
            elem.append(E.div('notice', "No info available"));
        }
    }

} //class
