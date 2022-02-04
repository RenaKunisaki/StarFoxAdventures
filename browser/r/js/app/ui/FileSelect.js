import { E } from "../../lib/Element.js";

export default class FileSelect {
    /** The UI for selecting an ISO and save file.
      */
    constructor(app) {
        this.app = app;

        //make ISO file input
        this.eIso = E.input({type:'file', name:'iso', id:'iso'});
        this.eIso.addEventListener('change', e => {
            this.app.loadIso(this.eIso.files[0]);
        }, false);

        //make save file input
        this.eSave = E.input({type:'file', name:'save', id:'save'});
        this.eSave.addEventListener('change', e => {
            this.app.loadSave(this.eSave.files[0]);
        }, false);

        this._makeSlotSelect(null);

        this.element = E.div('fileSelect',
            E.div('row',
                E.h1(null, "ISO File"),
                this.eIso,
                E.div('explanation',
                    `Selecting a Star Fox Adventures ISO file will let you
                    view lots of information extracted from it.`),
            ),
            E.div('row',
                E.h1(null, "Save File"),
                this.eSave, "Slot:", this.eSlot,
                E.div('explanation',
                    `Selecting a Star Fox Adventures save file will let you
                    examine it. Must be a GCI file.`),
            ),
            E.aside('explanation',
                `Neither file will be uploaded; all processing is done
                within your own web browser.`),
        );

        this.app.onSaveLoaded(save => this._onSaveLoaded(save));
        //this.app.onSaveSlotChanged(slot => this._onSaveSlotChanged(slot));
    } //constructor

    _makeSlotSelect(save) {
        //Build the save slot selector.
        //save: SaveGame
        let slots = E.select('saveSlot');
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
