#include "main.h"

void debugRenderSubMenu_close(const Menu *self) {
    //Close function for submenus of the debug render menu
    curMenu = &menuDebugRender;
    audioPlaySound(NULL, MENU_CLOSE_SOUND);
}


void menuDebugRenderTextures_select(const MenuItem *self, int amount) {
    if(amount) return;
    curMenu = &menuDebugTextureView;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}


void menuDebugRenderEffects_select(const MenuItem *self, int amount) {
    if(amount) return;
    curMenu = &menuDebugRenderEffects;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}


void menuDebugRenderMapGrid_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugRenderFlags & DEBUGRENDER_WORLD_MAP) ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugRenderMapGrid_select(const MenuItem *self, int amount) {
    debugRenderFlags ^= DEBUGRENDER_WORLD_MAP;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugRenderPerf_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugRenderFlags & DEBUGRENDER_PERF_METERS) ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugRenderPerf_select(const MenuItem *self, int amount) {
    debugRenderFlags ^= DEBUGRENDER_PERF_METERS;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugRenderDebugObjs_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugRenderFlags & DEBUGRENDER_DEBUG_OBJS) ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugRenderDebugObjs_select(const MenuItem *self, int amount) {
    debugRenderFlags ^= DEBUGRENDER_DEBUG_OBJS;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugRenderHitboxes_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugRenderFlags & DEBUGRENDER_HITBOXES) ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugRenderHitboxes_select(const MenuItem *self, int amount) {
    debugRenderFlags ^= DEBUGRENDER_HITBOXES;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugRenderRNG_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugRenderFlags & DEBUGRENDER_RNG) ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugRenderRNG_select(const MenuItem *self, int amount) {
    debugRenderFlags ^= DEBUGRENDER_RNG;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


Menu menuDebugRender = {
    "Render", 0,
    genericMenu_run, genericMenu_draw, debugSubMenu_close,
    "Effects",           genericMenuItem_draw,         menuDebugRenderEffects_select,
    "View Textures",     genericMenuItem_draw,         menuDebugRenderTextures_select,
    "Map Grid: %s",      menuDebugRenderMapGrid_draw,  menuDebugRenderMapGrid_select,
    "Perf Meters: %s",   menuDebugRenderPerf_draw,     menuDebugRenderPerf_select,
    "Debug Objects: %s", menuDebugRenderDebugObjs_draw,menuDebugRenderDebugObjs_select,
    "Hitboxes: %s",      menuDebugRenderHitboxes_draw, menuDebugRenderHitboxes_select,
    "RNG: %s",           menuDebugRenderRNG_draw,      menuDebugRenderRNG_select,
    NULL,
};
