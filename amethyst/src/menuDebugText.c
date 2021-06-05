#include "main.h"


void menuDebugTextMaster_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, enableDebugText ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugTextMaster_select(const MenuItem *self, int amount) {
    enableDebugText = !enableDebugText;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextPlayerCoords_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugTextFlags & DEBUGTEXT_PLAYER_COORDS) ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugTextPlayerCoords_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_PLAYER_COORDS;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextCameraCoords_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugTextFlags & DEBUGTEXT_CAMERA_COORDS) ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugTextCameraCoords_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_CAMERA_COORDS;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextRestartPoint_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugTextFlags & DEBUGTEXT_RESTART_POINT) ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugTextRestartPoint_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_RESTART_POINT;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextMemoryStats_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugTextFlags & DEBUGTEXT_MEMORY_INFO) ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugTextMemoryStats_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_MEMORY_INFO;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextInteractObj_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugTextFlags & DEBUGTEXT_INTERACT_OBJ_INFO) ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugTextInteractObj_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_INTERACT_OBJ_INFO;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextHeaps_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugTextFlags & DEBUGTEXT_HEAP_STATE) ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugTextHeaps_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_HEAP_STATE;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextBits_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugTextFlags & DEBUGTEXT_GAMEBIT_LOG) ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugTextBits_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_GAMEBIT_LOG;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextPlayerState_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugTextFlags & DEBUGTEXT_PLAYER_STATE) ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugTextPlayerState_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_PLAYER_STATE;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextHeapGraph_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugTextFlags & DEBUGTEXT_HEAP_GRAPH) ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugTextHeapGraph_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_HEAP_GRAPH;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextMapGrid_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugTextFlags & DEBUGTEXT_WORLD_MAP) ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugTextMapGrid_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_WORLD_MAP;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugTextTricky_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugTextFlags & DEBUGTEXT_TRICKY) ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugTextTricky_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_TRICKY;
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
    "Heap Graph: %s",      menuDebugTextHeapGraph_draw,    menuDebugTextHeapGraph_select,
    "Map Grid: %s",        menuDebugTextMapGrid_draw,      menuDebugTextMapGrid_select,
    NULL,
};
