import { E } from "../../lib/Element.js";

//XXX verify
const soundModes = ["Stereo", "Mono", "Surround", "Headphones"];

//XXX move
const Percent = val => (val * 100).toFixed(0).padStart(3) + '%';

function Table(...rows) {
    let elems = [];
    for(let row of rows) {
        let tr = E.tr(null);
        for(let cell of row) tr.append(E.td(null, cell));
        elems.push(tr);
    }
    return elems;
}

export default class SaveInfo {
    /** Displays information about a save file.
     */
    constructor(app) {
        this.app = app;
        this.element = E.div('notice', "Select a save file to examine.");

        this.app.onSaveLoaded(save => this._onSaveLoaded(save));
        this.app.onSaveSlotChanged(slot => this._onSaveSlotChanged(slot));
    } //constructor

    _makeScoreTable(save, key, title) {
        const elem = E.table('scores',
            E.tr('title', E.th(null, title, {colspan:3})),
            E.tr(null,
                E.th(null, "Name"),
                E.th(null, "Score"),
                E.th(null, "10 Rings"),
            ),
        );
        for(let score of save.global[key]) {
            elem.append(E.tr('score',
                E.td('name', score.name),
                E.td('score number', score.score >> 1),
                E.td('rings', (score.score & 1) ? '*' : ''),
            ));
        }
        return elem;
    }

    refresh() {
        const save = this.app.saveGame;
        const slot = this.app.saveSlot;
        const settings = save.global.settings;
        const elem = E.div('saveInfo',
            E.table('globals', ...Table(
                ["Valid",           settings.exists],
                ["Subtitles",       settings.bSubtitlesOn ? "On" : "Off"],
                ["Widescreen",      settings.bWidescreen ? "On" : "Off"],
                ["Rumble",          settings.bRumbleEnabled ? "On" : "Off"],
                ["Sound",           soundModes[settings.soundMode]],
                ["Music Volume",    Percent(settings.musicVolume/127)],
                ["SFX Volume",      Percent(settings.sfxVolume/127)],
                ["Cutscene Volume", Percent(settings.cutsceneVolume/127)],
                ["Unused 01",       settings.unused01],
                ["Unused 03",       settings.unusedHudSetting],
                ["Unused 04",       settings.unusedCameraSetting],
                ["Unused 05",       settings.unused05],
                ["Unused 07",       settings.unused07],
                ["Unused 0D",       settings.unused0D],
                ["Unused 0E",       settings.unused0E],
                ["Unknown 0E",      settings.unk18],
                //XXX cheats
            )),

            this._makeScoreTable(save, 'scoresToPlanet',    "Dinosaur Planet"),
            this._makeScoreTable(save, 'scoresDarkIce',     "DarkIce Mines"),
            this._makeScoreTable(save, 'scoresCloudRunner', "CloudRunner Fortress"),
            this._makeScoreTable(save, 'scoresWallCity',    "Walled City"),
            this._makeScoreTable(save, 'scoresDragonRock',  "Dragon Rock"),

            E.table('slotInfo',
                E.tr(null,
                ...Table(
                    ["Name", slot.name],
                    ["Completion", Percent(slot.completion)],
                    ["Time", slot.playTime],
                )),
            ),
        );
        this.element.replaceWith(elem);
    }

    _onSaveLoaded(save) {
        //called when app loads a new save file.
        //save: SaveGame
        this.refresh();
    } //_onSaveLoaded

    _onSaveSlotChanged(slot) {
        //called when the active save slot is changed.
        this.refresh();
    } //_onSaveSlotChanged
}
