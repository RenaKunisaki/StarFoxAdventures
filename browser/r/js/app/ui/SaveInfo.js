import { E } from "../../lib/Element.js";
import { int, float, hex, Percent, Table } from "../../Util.js";

//XXX verify
const soundModes = ["Stereo", "Mono", "Surround", "Headphones"];

export default class SaveInfo {
    /** Displays information about a save file.
     */
    constructor(app) {
        this.app = app;
        this.element = document.getElementById('tab-save-info');

        //this.app.onSaveLoaded(save => this._onSaveLoaded(save));
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
        if(save.global == null) {
            elem.append(E.tr('notice', E.td(null, "No data", {colspan:3})));
        }
        else {
            for(let score of save.global[key]) {
                elem.append(E.tr('score',
                    E.td('name', score.name),
                    E.td('score number', score.score >> 1),
                    E.td('rings', (score.score & 1) ? '*' : ''),
                ));
            }
        }
        return elem;
    }

    _makeScoresTable(save, slot) {
        const testStrength = [];
        const testTracking = [];
        for(let i=0; i<3; i++) {
            testStrength.push(slot.gameBits[`LV_TestStrengthBestTime${i+1}`]);
            testTracking.push(slot.gameBits[`LV_TestTrackingBestTime${i+1}`]);
        }
        return E.div('scores',
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
        );
    }

    _makeSlotTable(slot) {
        return E.table('slotInfo',
            E.tr('title', E.th(null, "Status", {colspan:2})),
            ...Table(
                ["Name", slot.name],
                ["Completion", Percent(slot.completion)],
                ["Time", slot.playTime],
                ["Tricky Food", `${slot.trickyEnergy} / ${slot.maxTrickyEnergy}`],
                ["Tricky Play Count", slot.trickyPlayCount],
                ["Unk1B", hex(slot.unk1B, 2)],
                ["Character",
                    (slot.character == 0 ? "Krystal" : (
                        slot.character == 1  ? "Fox" : slot.character))],
                ["Flags", hex(slot.flags21, 2) + hex(slot.flags22, 2)],
                ["Unk23", hex(slot.unk23, 2)],
                ["Text1", hex(slot.texts[0], 2)],
                ["Text2", hex(slot.texts[1], 2)],
                ["Text3", hex(slot.texts[2], 2)],
                ["Text4", hex(slot.texts[3], 2)],
                ["Text5", hex(slot.texts[4], 2)],
                ["Unk55F", hex(slot.unk55F, 2)],
                ["Unk6A4", hex(slot.unk6A4, 4)],
                ["Unk6A6", hex(slot.unk6A6, 4)],
            ),
        );
    }

    _makeSettingsTable(save) {
        if(save.global == null) {
            return E.table('globals',
                E.tr('title', E.th(null, "Settings")),
                E.tr('notice', E.td(null, "No data")),
            );
        }
        const settings = save.global.settings;
        return E.table('globals',
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
        );
    }

    _makeCharsTable(slot) {
        const FP=slot.charPos[1],   KP=slot.charPos[0];
        const FS=slot.charState[1], KS=slot.charState[0];
        return E.table('chars',
            E.tr('title', E.th(null, "Character States", {colspan:3})),
            E.tr(null, E.th(null, "What"), E.th(null, "Krystal"), E.th(null, "Fox")),
            ...Table(
                ["HP",
                    `${KS.curHealth} / ${KS.maxHealth}`,
                    `${FS.curHealth} / ${FS.maxHealth}`],
                ["Unk02", hex(KS.unk02, 2), hex(FS.unk02, 2)],
                ["Unk03", hex(KS.unk03, 2), hex(FS.unk03, 2)],
                ["MP",
                    `${KS.curMagic} / ${KS.maxMagic}`,
                    `${FS.curMagic} / ${FS.maxMagic}`],
                ["Money", int(KS.money), int(FS.money)],
                ["Lives",
                    `${KS.curBafomDads} / ${KS.maxBafomDads}`,
                    `${FS.curBafomDads} / ${FS.maxBafomDads}`],
                ["Unk0B", hex(KS.unk0B, 2), hex(FS.unk0B, 2)],
                ["Position",
                    `${KP.pos.x.toFixed(2)}, ${KP.pos.y.toFixed(2)}, ${KP.pos.z.toFixed(2)}`,
                    `${FP.pos.x.toFixed(2)}, ${FP.pos.y.toFixed(2)}, ${FP.pos.z.toFixed(2)}`],
                ["X Rot",    int(KP.rotX),     int(FP.rotX)],
                ["Map Layer",int(KP.mapLayer), int(FP.mapLayer)],
                ["Map ID",   hex(KP.mapNo, 2), hex(FP.mapNo, 2)],
                ["Unk0F",    hex(KP.unk0F, 2), hex(FP.unk0F, 2)],
            )
        );
    }

    refresh() {
        const save = this.app.saveGame;
        const slot = this.app.saveSlot;
        console.log("save slot", slot);
        const elem = E.div('saveInfo',
            this._makeSlotTable(slot),
            this._makeSettingsTable(save),
            this._makeCharsTable(slot),
            this._makeScoresTable(save, slot)
        );
        this.element.replaceWith(elem);
        this.element = elem;
    }

    _onSaveSlotChanged(slot) {
        //called when the active save slot is changed.
        this.refresh();
    } //_onSaveSlotChanged
}
