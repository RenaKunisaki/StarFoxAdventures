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
    char str[256];
    sprintf(str, self->fmt, T(self->name), (debugRenderFlags & DEBUGRENDER_WORLD_MAP) ?
        T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuDebugRenderMapGrid_select(const MenuItem *self, int amount) {
    debugRenderFlags ^= DEBUGRENDER_WORLD_MAP;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugRenderPerf_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), (debugRenderFlags & DEBUGRENDER_PERF_METERS) ?
        T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuDebugRenderPerf_select(const MenuItem *self, int amount) {
    debugRenderFlags ^= DEBUGRENDER_PERF_METERS;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugRenderDebugObjs_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), (debugRenderFlags & DEBUGRENDER_DEBUG_OBJS) ?
        T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuDebugRenderDebugObjs_select(const MenuItem *self, int amount) {
    debugRenderFlags ^= DEBUGRENDER_DEBUG_OBJS;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugRenderPlayerVel_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), (debugRenderFlags & DEBUGRENDER_PLAYER_VEL) ?
        T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuDebugRenderPlayerVel_select(const MenuItem *self, int amount) {
    debugRenderFlags ^= DEBUGRENDER_PLAYER_VEL;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugRenderHitboxes_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), (debugRenderFlags & DEBUGRENDER_HITBOXES) ?
        T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuDebugRenderHitboxes_select(const MenuItem *self, int amount) {
    debugRenderFlags ^= DEBUGRENDER_HITBOXES;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugRenderAttachPoints_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), (debugRenderFlags & DEBUGRENDER_ATTACH_POINTS) ?
        T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuDebugRenderAttachPoints_select(const MenuItem *self, int amount) {
    debugRenderFlags ^= DEBUGRENDER_ATTACH_POINTS;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugRenderFocusPoints_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), (debugRenderFlags & DEBUGRENDER_FOCUS_POINTS) ?
        T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuDebugRenderFocusPoints_select(const MenuItem *self, int amount) {
    debugRenderFlags ^= DEBUGRENDER_FOCUS_POINTS;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugRenderUnkPoints_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), (debugRenderFlags & DEBUGRENDER_UNK_POINTS) ?
        T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuDebugRenderUnkPoints_select(const MenuItem *self, int amount) {
    debugRenderFlags ^= DEBUGRENDER_UNK_POINTS;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugRenderRNG_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), (debugRenderFlags & DEBUGRENDER_RNG) ?
        T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuDebugRenderRNG_select(const MenuItem *self, int amount) {
    debugRenderFlags ^= DEBUGRENDER_RNG;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}

#define _FMT "%s:\eX290%s"
Menu menuDebugRender = {
    "Render", 0,
    genericMenu_run, genericMenu_draw, debugSubMenu_close,
    "Effects",        "%s", genericMenuItem_draw,             menuDebugRenderEffects_select,
    "View Textures",  "%s", genericMenuItem_draw,             menuDebugRenderTextures_select,
    "Map Grid",       _FMT, menuDebugRenderMapGrid_draw,      menuDebugRenderMapGrid_select,
    "Perf Meters",    _FMT, menuDebugRenderPerf_draw,         menuDebugRenderPerf_select,
    "RNG",            _FMT, menuDebugRenderRNG_draw,          menuDebugRenderRNG_select,
    "Debug Objects",  _FMT, menuDebugRenderDebugObjs_draw,    menuDebugRenderDebugObjs_select,
    "Player Velocity",_FMT, menuDebugRenderPlayerVel_draw,    menuDebugRenderPlayerVel_select,
    "Hitboxes",       _FMT, menuDebugRenderHitboxes_draw,     menuDebugRenderHitboxes_select,
    "Attach Points",  _FMT, menuDebugRenderAttachPoints_draw, menuDebugRenderAttachPoints_select,
    "Focus Points",   _FMT, menuDebugRenderFocusPoints_draw,  menuDebugRenderFocusPoints_select,
    "Unknown Points", _FMT, menuDebugRenderUnkPoints_draw,    menuDebugRenderUnkPoints_select,
    NULL,
};
#undef _FMT
