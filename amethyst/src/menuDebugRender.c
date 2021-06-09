#include "main.h"

void debugRenderSubMenu_close(const Menu *self) {
    //Close function for submenus of the debug render menu
    curMenu = &menuDebug;
    audioPlaySound(NULL, MENU_CLOSE_SOUND);
}


void menuDebugRenderBlur_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, bEnableBlurFilter ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugRenderBlur_select(const MenuItem *self, int amount) {
    bEnableBlurFilter = !bEnableBlurFilter;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugRenderMotBlur_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (int)motionBlurIntensity);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugRenderMotBlur_select(const MenuItem *self, int amount) {
    int val = motionBlurIntensity + amount;
    if(val <   0) val = 255;
    if(val > 255) val =   0;
    motionBlurIntensity = val;
    bEnableMotionBlur = (val > 0);
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugRenderHeatFx_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, heatEffectIntensity);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugRenderHeatFx_select(const MenuItem *self, int amount) {
    int val = heatEffectIntensity + amount;
    if(val <   0) val = 255;
    if(val > 255) val =   0;
    heatEffectIntensity = val;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugRenderMono_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, bEnableMonochromeFilter ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugRenderMono_select(const MenuItem *self, int amount) {
    bEnableMonochromeFilter = !bEnableMonochromeFilter;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugRenderSpirit_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, bEnableSpiritVision ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugRenderSpirit_select(const MenuItem *self, int amount) {
    bEnableSpiritVision = !bEnableSpiritVision;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugRenderFilter_select(const MenuItem *self, int amount) {
    if(amount) return;
    curMenu = &menuDebugColor;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

void menuDebugRenderTextures_select(const MenuItem *self, int amount) {
    if(amount) return;
    curMenu = &menuDebugTextureView;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}


void menuDebugRenderMapGrid_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugTextFlags & DEBUGTEXT_WORLD_MAP) ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugRenderMapGrid_select(const MenuItem *self, int amount) {
    debugTextFlags ^= DEBUGTEXT_WORLD_MAP;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


Menu menuDebugRender = {
    "Render", 0,
    genericMenu_run, genericMenu_draw, debugSubMenu_close,
    "Blur Filter: %s",   menuDebugRenderBlur_draw,     menuDebugRenderBlur_select,
    "Motion Blur: %d",   menuDebugRenderMotBlur_draw,  menuDebugRenderMotBlur_select,
    "Heat Effect: %d",   menuDebugRenderHeatFx_draw,   menuDebugRenderHeatFx_select,
    "Monochrome: %s",    menuDebugRenderMono_draw,     menuDebugRenderMono_select,
    "Spirit Vision: %s", menuDebugRenderSpirit_draw,   menuDebugRenderSpirit_select,
    "Color Filters",     genericMenuItem_draw,         menuDebugRenderFilter_select,
    "View Textures",     genericMenuItem_draw,         menuDebugRenderTextures_select,
    "Map Grid: %s",      menuDebugRenderMapGrid_draw,  menuDebugRenderMapGrid_select,
    NULL,
};
