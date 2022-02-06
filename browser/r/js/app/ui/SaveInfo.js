import { E } from "../../lib/Element.js";
import { hex } from "../../Util.js";

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

        const testStrength = [];
        const testTracking = [];
        for(let i=0; i<3; i++) {
            testStrength.push(slot.gameBits[`LV_TestStrengthBestTime${i+1}`]);
            testTracking.push(slot.gameBits[`LV_TestTrackingBestTime${i+1}`]);
        }

        const elem = E.div('saveInfo',
            E.table('slotInfo',
                E.tr('title', E.th(null, "Status", {colspan:2})),
                ...Table(
                    ["Name", slot.name],
                    ["Completion", Percent(slot.completion)],
                    ["Time", slot.playTime],
                ),
            ),
            E.table('globals',
                E.tr('title', E.th(null, "Settings", {colspan:2})),
                ...Table(
                    ["Valid",           settings.exists],
                    ["Subtitles",       settings.bSubtitlesOn ? "On" : "Off"],
                    ["Widescreen",      settings.bWidescreen ? "On" : "Off"],
                    ["Rumble",          settings.bRumbleEnabled ? "On" : "Off"],
                    ["Sound",           soundModes[settings.soundMode]],
                    ["Music Volume",    Percent(settings.musicVolume/127)],
                    ["SFX Volume",      Percent(settings.sfxVolume/127)],
                    ["Cutscene Volume", Percent(settings.cutsceneVolume/127)],
                    ["Unused 01",       hex(settings.unused01, 2)],
                    ["Unused 03",       hex(settings.unusedHudSetting, 2)],
                    ["Unused 04",       hex(settings.unusedCameraSetting, 2)],
                    ["Unused 05",       hex(settings.unused05, 2)],
                    ["Unused 07",       hex(settings.unused07, 2)],
                    ["Unused 0D",       hex(settings.unused0D, 2)],
                    ["Unused 0E",       hex(settings.unused0E, 4)],
                    ["Unknown 0E",      hex(settings.unk18, 8)],
                    //XXX cheats
                )
            ),
            E.div('scores',
                E.h2(null, "High Scores (Arwing)"),
                this._makeScoreTable(save, 'scoresToPlanet',    "Dinosaur Planet"),
                this._makeScoreTable(save, 'scoresDarkIce',     "DarkIce Mines"),
                this._makeScoreTable(save, 'scoresCloudRunner', "CloudRunner Fortress"),
                this._makeScoreTable(save, 'scoresWallCity',    "Walled City"),
                this._makeScoreTable(save, 'scoresDragonRock',  "Dragon Rock"),
                E.div('note', "These scores are shared by all save slots."),
                E.h2(null, "Best Times (LightFoot Village)"),
                E.table('bestTimes',
                    E.tr('title', E.th(null, "Test of Strength", {colspan:2})),
                    E.tr(null, E.td(null, "1st"), E.td(null, testStrength[0])),
                    E.tr(null, E.td(null, "2nd"), E.td(null, testStrength[1])),
                    E.tr(null, E.td(null, "3rd"), E.td(null, testStrength[2])),
                ),
                E.table('bestTimes',
                    E.tr('title', E.th(null, "Tracking Test", {colspan:2})),
                    E.tr(null, E.td(null, "1st"), E.td(null, testTracking[0])),
                    E.tr(null, E.td(null, "2nd"), E.td(null, testTracking[1])),
                    E.tr(null, E.td(null, "3rd"), E.td(null, testTracking[2])),
                ),
                E.div('note', "These times are specific to this save slot."),
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
