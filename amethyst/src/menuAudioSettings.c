/** Audio Settings submenu.
 */
#include "main.h"

void menuMusicVol_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (int)(volumeMusic * 100));
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuMusicVol_select(const MenuItem *self, int amount) {
    volumeMusic += (amount * 0.01);
    if(volumeMusic < 0) volumeMusic = 0;
    if(volumeMusic > 2) volumeMusic = 2;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuSfxVol_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (int)(volumeSFX * 100));
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuSfxVol_select(const MenuItem *self, int amount) {
    volumeSFX += (amount * 0.01);
    if(volumeSFX < 0) volumeSFX = 0;
    if(volumeSFX > 2) volumeSFX = 2;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuSceneVol_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (int)((volumeCutScenes / 127.0) * 100.0));
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuSceneVol_select(const MenuItem *self, int amount) {
    int vol = volumeCutScenes + (amount * 13);
    if(vol <   0) vol =   0;
    if(vol > 255) vol = 255;
    volumeCutScenes = vol;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
    //XXX does setting this > 127 actually work?
}


static const char *soundModeNames[] = {"Stereo", "Surround", "Mono", "Headphones"};
void menuSoundMode_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, soundModeNames[soundMode]);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuSoundMode_select(const MenuItem *self, int amount) {
    soundMode = (soundMode + amount) & 3;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


static u8 musicTestIdx = 0;
void menuMusicTest_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->name, musicTestIdx, songTable[musicTestIdx].name);
    //sprintf(str, "Song %02X: %04X %02X %02X %08X %08X %s",
    //    musicTestIdx,
    //    songTable[musicTestIdx].unk00,
    //    songTable[musicTestIdx].unk02,
    //    songTable[musicTestIdx].unk03,
    //    songTable[musicTestIdx].unk08,
    //    songTable[musicTestIdx].unk0C,
    //    songTable[musicTestIdx].name);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuMusicTest_select(const MenuItem *self, int amount) {
    if(amount == 0) { //A button
        musicTestPlaySong(musicTestIdx);
    }
    else {
        int idx = musicTestIdx + amount;
        if(idx < 0) idx = NUM_SONGS - 1;
        if(idx >= NUM_SONGS) idx = 0;
        musicTestIdx = idx;
        audioPlaySound(NULL, MENU_ADJUST_SOUND);
    }
}


static u16 sfxTestIdx = 0;
void menuSfxTest_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    //sound effects don't have names
    sprintf(str, self->name, sfxTestIdx);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuSfxTest_select(const MenuItem *self, int amount) {
    if(amount == 0) { //A button
        audioPlaySound(NULL, sfxTestIdx);
    }
    else { //XXX what is the max SFX ID?
        //int idx = sfxTestIdx + amount;
        //if(idx < 0) idx = NUM_SFX - 1;
        //if(idx >= NUM_SFX) idx = 0;
        //sfxTestIdx = idx;
        sfxTestIdx += amount;
        audioPlaySound(NULL, MENU_ADJUST_SOUND);
    }
}


static s16 streamTestIdx = 0;
void menuStreamTest_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->name, streamTestIdx, pStreamsBin[streamTestIdx].name);
    //sprintf(str, "Stream %04X: %04X %02X %02X %04X %s", streamTestIdx,
    //    pStreamsBin[streamTestIdx].id,
    //    pStreamsBin[streamTestIdx].unk02,
    //    pStreamsBin[streamTestIdx].unk03,
    //    pStreamsBin[streamTestIdx].unk04,
    //    pStreamsBin[streamTestIdx].name);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuStreamTest_select(const MenuItem *self, int amount) {
    if(amount == 0) { //A button
        streamStopAll();
        streamPlay(pStreamsBin[streamTestIdx].id, streamPlay_defaultCallback);
    }
    else {
        int idx = streamTestIdx + amount;
        if(idx < 0) idx = NUM_STREAMS - 1;
        if(idx >= NUM_STREAMS) idx = 0;
        streamTestIdx = idx;
        audioPlaySound(NULL, MENU_ADJUST_SOUND);
    }
}


void menuStopSounds_select(const MenuItem *self, int amount) {
    if(amount) return;
    streamStopAll();
    objStopSound_(NULL, sfxTestIdx);
    //XXX music
}


Menu menuAudioSettings = {
    "Audio Settings", 0,
    genericMenu_run, genericMenu_draw, mainSubMenu_close,
    "Music Volume: %d%%",    menuMusicVol_draw,    menuMusicVol_select,
    "SFX Volume: %d%%",      menuSfxVol_draw,      menuSfxVol_select,
    "CutScene Volume: %d%%", menuSceneVol_draw,    menuSceneVol_select,
    "Sound Mode: %s",        menuSoundMode_draw,   menuSoundMode_select,
    "Play Song: %02X %s",    menuMusicTest_draw,   menuMusicTest_select,
    "Play SFX: %04X",        menuSfxTest_draw,     menuSfxTest_select,
    "Play Stream: %02X %s",  menuStreamTest_draw,  menuStreamTest_select,
    "Stop All Sounds",       genericMenuItem_draw, menuStopSounds_select,
    NULL,
};
