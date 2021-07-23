#include "main.h"
bool bAutoSave = false;
static u8 autoSaveMsgTimer = 0;

/* # offsets into saveData
.set SAVEDATA_EXTRA_OPTIONS,  0x01 # unused
.set SAVEDATA_SUBTITLES,      0x02
.set SAVEDATA_CAMERA_OPTIONS, 0x03 # replace unused HUD setting
.set SAVEDATA_WIDESCREEN,     0x06
.set SAVEDATA_CUR_CHAR,       0x07 # unused field
.set SAVEDATA_RUMBLE,         0x08
.set SAVEDATA_SOUND_MODE,     0x09
.set SAVEDATA_MUSIC_VOL,      0x0A
.set SAVEDATA_SFX_VOL,        0x0B
.set SAVEDATA_CUTSCENE_VOL,   0x0C
.set SAVEDATA_OPTIONS,        0x0D # unused field
.set SAVEDATA_FOV,            0x0E # unused field
.set SAVEDATA_MAP_OPACITY,    0x0F # unused field
# XXX: game speed
# autosave is handled elsewhere because I did it earlier

# extra feature flag bits
.set EXTRA_FEATURE_RUMBLE_BLUR, 0x01

.set SAVEDATA_OPTION_MAP_SIZE,0x03
.set SAVEDATA_OPTION_PDA_MODE,0x0C
.set SAVEDATA_OPTION_FUR_FX,  0x10
.set SAVEDATA_OPTION_BACKPACK,0x60 */

void saveLoadHook() {
    //replaces a call when loading saves that sets an unused variable.
    //we'll instead repurpose some unused fields for our own settings.
    SaveGameSettings *save = &saveData.saveSettings;
    overridePlayerNo = save->unused07;

    u8 extraFeatureFlags = save->unused01;
    bRumbleBlur = extraFeatureFlags & 0x01;

    cameraFlags = save->unusedHudSetting;

    overrideFov = save->unused0E;
    if(!overrideFov) overrideFov = 60;

    overrideMinimapAlpha = save->unused0F;
    if(overrideMinimapAlpha == 0) overrideMinimapAlpha = 255;

    overrideMinimapSize = save->unused0D & 0x03;
    minimapMode = (save->unused0D >> 2) & 0x03;
    if(minimapMode == 3) {
        minimapMode = 0;
        pdaOn = false;
    }
    else pdaOn = true;
    furFxMode    = (save->unused0D >> 4) & 3;
    backpackMode = (save->unused0D >> 6) & 3;
    bAutoSave    = (save->unlockedCheats >> 31) & 1;
    hudFlags     = save->unk18;

    //load language setting and get the appropriate text
    curLanguage = save->unk19;
    if(curLanguage < 0 || curLanguage >= NUM_LANGUAGES) curLanguage = 0;
    GameTextDir32 dir = curGameTextDir;
    gameTextLoadDir(GAMETEXT_DIR_Link); //load HUD texts
    gameTextLoadDir(dir); //then load current map's texts

    /* DPRINT("Savedata loaded!");
    char str[1024];
    u8 *data = (u8*)save;
    for(int i=0; i<sizeof(SaveGameSettings); i++) {
        sprintf(&str[i*3], "%02X ", data[i]);
    }
    DPRINT("Data: %s", str); */
    DPRINT("Load savedata: player=%d FOV=%d alpha=%d map=%d lang=%d\n",
        overridePlayerNo, overrideFov, overrideMinimapAlpha, minimapMode, curLanguage);
}

//Update the extra fields in the save data.
void updateSaveData() {
    SaveGameSettings *save = &saveData.saveSettings;
    save->unused07 = overridePlayerNo;
    //XXX rumble blur
    //XXX why are these sometimes not being saved/loaded?
    save->unused0E = overrideFov;
    save->unused0F = overrideMinimapAlpha;

    int mapMode = minimapMode;
    if(!pdaOn) mapMode = 3;
    save->unused0D = overrideMinimapSize | (mapMode << 2) |
        (furFxMode << 4) | (backpackMode << 6);

    save->unused01 = (bRumbleBlur ? 0x01 : 0x00);
    save->unusedHudSetting = cameraFlags;
    save->unk18 = hudFlags;
    save->unk19 = curLanguage;

    //we also need to update these
    save->musicVolume    = (int)(volumeMusic * 127.0);
    save->sfxVolume      = (int)(volumeSFX * 127.0);
    save->cutsceneVolume = volumeCutScenes;
    save->soundMode      = soundMode;
    save->bRumbleEnabled = enableRumble;
    save->bWidescreen    = (renderFlags & RenderFlag_Widescreen) ? 1 : 0;
    save->bRumbleEnabled = enableRumble ? 1 : 0;
    save->bSubtitlesOn   = bSubtitlesEnabled ? 1 : 0;
    if(bAutoSave) save->unlockedCheats |= (1 << 31);
    else save->unlockedCheats &= ~(1 << 31);

    DPRINT("Update savedata: player=%d FOV=%d alpha=%d map=%d lang=%d\n",
        overridePlayerNo, overrideFov, overrideMinimapAlpha, minimapMode, curLanguage);
}

//Called from main loop.
void saveUpdateHook() {
    if(autoSaveMsgTimer) {
        autoSaveMsgTimer--;
        gameTextSetColor(0xFF, 0xFF, 0xFF, 0xFF);
        gameTextShowStr("Saving...", 0x0A, 0, 0);
    }
}

void doAutoSave() {
    if(!bAutoSave) return;
    //don't save during scripted events, or they can break.
    switch(cameraMode) {
        case 0x45: //bike
        case 0x47: //test of strength
        case 0x4C: //cutscene
        case 0x4D: //speaking to NPC
            return;
    }
    if(curSaveSlot < 0) return; //no save slot
    if(saveStatus != 1) return; //can't save
    saveGame_save();

    //technically the message appears after we save, but oh well
    autoSaveMsgTimer = 60;
}

void* saveMapLoadHook(MapDirIdx32 map, DataFileEnum file) {
    //replaces a call to mapLoadDataFile()
    //save immediately before loading a new map, since that's when we're
    //most likely to crash due to out of memory.
    doAutoSave();
    return mapLoadDataFile(map, file);
}

void saveShowMsgHook(int param) {
    //replaces a call to cardShowLoadingMsg
    //replace it with just the popup message at the bottom
    autoSaveMsgTimer = 60;
    DPRINT("Saving!");
}
