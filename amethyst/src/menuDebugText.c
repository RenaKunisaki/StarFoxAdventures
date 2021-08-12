#include "main.h"


void menuDebugTextMaster_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, enableDebugText ? "On" : "Off");
    menuDrawText(str, x, y, selected);
}
void menuDebugTextMaster_select(const MenuItem *self, int amount) {
    enableDebugText = !enableDebugText;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextPlayerCoords_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugTextFlags & DEBUGTEXT_PLAYER_COORDS) ? "On" : "Off");
    menuDrawText(str, x, y, selected);
}
void menuDebugTextPlayerCoords_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_PLAYER_COORDS;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextCameraCoords_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugTextFlags & DEBUGTEXT_CAMERA_COORDS) ? "On" : "Off");
    menuDrawText(str, x, y, selected);
}
void menuDebugTextCameraCoords_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_CAMERA_COORDS;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextRestartPoint_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugTextFlags & DEBUGTEXT_RESTART_POINT) ? "On" : "Off");
    menuDrawText(str, x, y, selected);
}
void menuDebugTextRestartPoint_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_RESTART_POINT;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextMemoryStats_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugTextFlags & DEBUGTEXT_MEMORY_INFO) ? "On" : "Off");
    menuDrawText(str, x, y, selected);
}
void menuDebugTextMemoryStats_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_MEMORY_INFO;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextInteractObj_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugTextFlags & DEBUGTEXT_INTERACT_OBJ_INFO) ? "On" : "Off");
    menuDrawText(str, x, y, selected);
}
void menuDebugTextInteractObj_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_INTERACT_OBJ_INFO;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextHeaps_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugTextFlags & DEBUGTEXT_HEAP_STATE) ? "On" : "Off");
    menuDrawText(str, x, y, selected);
}
void menuDebugTextHeaps_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_HEAP_STATE;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextBits_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugTextFlags & DEBUGTEXT_GAMEBIT_LOG) ? "On" : "Off");
    menuDrawText(str, x, y, selected);
}
void menuDebugTextBits_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_GAMEBIT_LOG;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextPlayerState_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugTextFlags & DEBUGTEXT_PLAYER_STATE) ? "On" : "Off");
    menuDrawText(str, x, y, selected);
}
void menuDebugTextPlayerState_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_PLAYER_STATE;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextTricky_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugTextFlags & DEBUGTEXT_TRICKY) ? "On" : "Off");
    menuDrawText(str, x, y, selected);
}
void menuDebugTextTricky_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_TRICKY;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextPerf_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugTextFlags & DEBUGTEXT_PERFORMANCE) ? "On" : "Off");
    menuDrawText(str, x, y, selected);
}
void menuDebugTextPerf_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_PERFORMANCE;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextFPS_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugTextFlags & DEBUGTEXT_FPS) ? "On" : "Off");
    menuDrawText(str, x, y, selected);
}
void menuDebugTextFPS_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_FPS;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextRNG_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugTextFlags & DEBUGTEXT_RNG) ? "On" : "Off");
    menuDrawText(str, x, y, selected);
}
void menuDebugTextRNG_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_RNG;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextAudioStreams_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugTextFlags & DEBUGTEXT_AUDIO_STREAMS) ? "On" : "Off");
    menuDrawText(str, x, y, selected);
}
void menuDebugTextAudioStreams_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_AUDIO_STREAMS;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextAudioSFX_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugTextFlags & DEBUGTEXT_AUDIO_SFX) ? "On" : "Off");
    menuDrawText(str, x, y, selected);
}
void menuDebugTextAudioSFX_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_AUDIO_SFX;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}

Menu menuDebugText = {
    "Debug Text", 0,
    genericMenu_run, genericMenu_draw, debugSubMenu_close,
    "Master: %s",        menuDebugTextMaster_draw,         menuDebugTextMaster_select,
    "Player State: %s",    menuDebugTextPlayerState_draw,  menuDebugTextPlayerState_select,
    "Tricky: %s",          menuDebugTextTricky_draw,       menuDebugTextTricky_select,
    "Player Coords: %s",   menuDebugTextPlayerCoords_draw, menuDebugTextPlayerCoords_select,
    "Camera Coords: %s",   menuDebugTextCameraCoords_draw, menuDebugTextCameraCoords_select,
    "Restart Point: %s",   menuDebugTextRestartPoint_draw, menuDebugTextRestartPoint_select,
    "Interact Obj: %s",    menuDebugTextInteractObj_draw,  menuDebugTextInteractObj_select,
    "GameBit Changes: %s", menuDebugTextBits_draw,         menuDebugTextBits_select,
    "Memory Stats: %s",    menuDebugTextMemoryStats_draw,  menuDebugTextMemoryStats_select,
    "Heap Details: %s",    menuDebugTextHeaps_draw,        menuDebugTextHeaps_select,
    "Performance: %s",     menuDebugTextPerf_draw,         menuDebugTextPerf_select,
    "FPS Meter: %s",       menuDebugTextFPS_draw,          menuDebugTextFPS_select,
    "RNG: %s",             menuDebugTextRNG_draw,          menuDebugTextRNG_select,
    "Audio Streams: %s",   menuDebugTextAudioStreams_draw, menuDebugTextAudioStreams_select,
    "Audio SFX: %s",       menuDebugTextAudioSFX_draw,     menuDebugTextAudioSFX_select,
    NULL,
};
