#include "main.h"


void menuDebugTextMaster_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), enableDebugText ? T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuDebugTextMaster_select(const MenuItem *self, int amount) {
    enableDebugText = !enableDebugText;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextPlayerCoords_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), (debugTextFlags & DEBUGTEXT_PLAYER_COORDS) ? T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuDebugTextPlayerCoords_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_PLAYER_COORDS;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextCameraCoords_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), (debugTextFlags & DEBUGTEXT_CAMERA_COORDS) ? T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuDebugTextCameraCoords_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_CAMERA_COORDS;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextRestartPoint_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), (debugTextFlags & DEBUGTEXT_RESTART_POINT) ? T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuDebugTextRestartPoint_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_RESTART_POINT;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextMemoryStats_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), (debugTextFlags & DEBUGTEXT_MEMORY_INFO) ? T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuDebugTextMemoryStats_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_MEMORY_INFO;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextInteractObj_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), (debugTextFlags & DEBUGTEXT_INTERACT_OBJ_INFO) ? T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuDebugTextInteractObj_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_INTERACT_OBJ_INFO;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextHeaps_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), (debugTextFlags & DEBUGTEXT_HEAP_STATE) ? T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuDebugTextHeaps_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_HEAP_STATE;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextBits_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), (debugTextFlags & DEBUGTEXT_GAMEBIT_LOG) ? T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuDebugTextBits_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_GAMEBIT_LOG;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextPlayerState_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), (debugTextFlags & DEBUGTEXT_PLAYER_STATE) ? T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuDebugTextPlayerState_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_PLAYER_STATE;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextTricky_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), (debugTextFlags & DEBUGTEXT_TRICKY) ? T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuDebugTextTricky_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_TRICKY;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextPerf_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), (debugTextFlags & DEBUGTEXT_PERFORMANCE) ? T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuDebugTextPerf_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_PERFORMANCE;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextFPS_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), (debugTextFlags & DEBUGTEXT_FPS) ? T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuDebugTextFPS_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_FPS;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextRNG_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), (debugTextFlags & DEBUGTEXT_RNG) ? T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuDebugTextRNG_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_RNG;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextAudioStreams_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), (debugTextFlags & DEBUGTEXT_AUDIO_STREAMS) ? T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuDebugTextAudioStreams_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_AUDIO_STREAMS;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextAudioSFX_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), (debugTextFlags & DEBUGTEXT_AUDIO_SFX) ? T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuDebugTextAudioSFX_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_AUDIO_SFX;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextEnvironment_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), (debugTextFlags & DEBUGTEXT_ENVIRONMENT) ? T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuDebugTextEnvironment_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_ENVIRONMENT;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}

#define _FMT "%s:\eX300%s"
Menu menuDebugText = {
    "Debug Text", 0,
    genericMenu_run, genericMenu_draw, debugSubMenu_close,
    "Master",          _FMT, menuDebugTextMaster_draw,         menuDebugTextMaster_select,
    "Player State",    _FMT, menuDebugTextPlayerState_draw,  menuDebugTextPlayerState_select,
    "Tricky",          _FMT, menuDebugTextTricky_draw,       menuDebugTextTricky_select,
    "Player Coords",   _FMT, menuDebugTextPlayerCoords_draw, menuDebugTextPlayerCoords_select,
    "Camera Coords",   _FMT, menuDebugTextCameraCoords_draw, menuDebugTextCameraCoords_select,
    "Restart Point",   _FMT, menuDebugTextRestartPoint_draw, menuDebugTextRestartPoint_select,
    "Interact Obj",    _FMT, menuDebugTextInteractObj_draw,  menuDebugTextInteractObj_select,
    "GameBit Changes", _FMT, menuDebugTextBits_draw,         menuDebugTextBits_select,
    "Memory Stats",    _FMT, menuDebugTextMemoryStats_draw,  menuDebugTextMemoryStats_select,
    "Heap Details",    _FMT, menuDebugTextHeaps_draw,        menuDebugTextHeaps_select,
    "Performance",     _FMT, menuDebugTextPerf_draw,         menuDebugTextPerf_select,
    "FPS Meter",       _FMT, menuDebugTextFPS_draw,          menuDebugTextFPS_select,
    "RNG",             _FMT, menuDebugTextRNG_draw,          menuDebugTextRNG_select,
    "Audio Streams",   _FMT, menuDebugTextAudioStreams_draw, menuDebugTextAudioStreams_select,
    "Audio SFX",       _FMT, menuDebugTextAudioSFX_draw,     menuDebugTextAudioSFX_select,
    "Environment",     _FMT, menuDebugTextEnvironment_draw,  menuDebugTextEnvironment_select,
    NULL,
};
#undef _FMT
