#include "main.h"

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
    OSReport("Savedata loaded!");
}

void saveUpdateHook() {
    //called from main loop, stores our settings into save data
    //so that they'll be preserved
    SaveGameSettings *save = &saveData.saveSettings;
    save->unused07 = overridePlayerNo;
    //XXX rumble blur
    save->unused0E = overrideFov;
    save->unused0F = overrideMinimapAlpha;

    int mapMode = minimapMode;
    if(!pdaOn) mapMode = 3;
    save->unused0D = overrideMinimapSize | (mapMode << 2) |
        (furFxMode << 4) | (backpackMode << 6);

    save->unused01 = (bRumbleBlur ? 0x01 : 0x00);

    //we also need to update these
    save->musicVolume    = volumeMusic * 127;
    save->sfxVolume      = volumeSFX * 127;
    save->cutsceneVolume = volumeCutScenes;
    save->bWidescreen    = (renderFlags & RenderFlag_Widescreen) ? 1 : 0;
    save->bRumbleEnabled = enableRumble ? 1 : 0;
    save->bSubtitlesOn   = bSubtitlesEnabled ? 1 : 0;
}
