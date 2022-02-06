import { E, clearElement } from "../../lib/Element.js";

export default class FileSelect {
    /** The UI for selecting an ISO and save file.
      */
    constructor(app) {
        this.app = app;

        //set up ISO file input
        this.eIso = document.getElementById('fileIso');
        this.eIso.addEventListener('change', e => {
            this.app.loadIso(this.eIso.files[0]);
        }, false);

        //set up save file input
        this.eSave = document.getElementById('fileSave');
        this.eSave.addEventListener('change', e => {
            this.app.loadSave(this.eSave.files[0]);
        }, false);

        this._makeSlotSelect(null);
        this.element = document.getElementById('tab-file-select');
        this.app.onSaveLoaded(save => this._onSaveLoaded(save));
        //this.app.onSaveSlotChanged(slot => this._onSaveSlotChanged(slot));
    } //constructor

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
        this._makeSlotSelect(save);
    } //_onSaveLoaded

} //class
