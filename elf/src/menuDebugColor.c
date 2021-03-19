#include "main.h"


void menuDebugRenderColFiltOn_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, bEnableColorFilter ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugRenderColFiltOn_select(const MenuItem *self, int amount) {
    bEnableColorFilter = !bEnableColorFilter;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugRenderColFiltR_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, colorFilterColor.r);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugRenderColFiltR_select(const MenuItem *self, int amount) {
    int val = colorFilterColor.r + amount;
    if(val <   0) val = 255;
    if(val > 255) val =   0;
    colorFilterColor.r = val;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugRenderColFiltG_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, colorFilterColor.g);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugRenderColFiltG_select(const MenuItem *self, int amount) {
    int val = colorFilterColor.g + amount;
    if(val <   0) val = 255;
    if(val > 255) val =   0;
    colorFilterColor.g = val;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugRenderColFiltB_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, colorFilterColor.b);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugRenderColFiltB_select(const MenuItem *self, int amount) {
    int val = colorFilterColor.b + amount;
    if(val <   0) val = 255;
    if(val > 255) val =   0;
    colorFilterColor.b = val;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugRenderColScale_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    if(overrideColorScale >= 0){
        sprintf(str, "Color Scale: %d%%", (int)((overrideColorScale / 255.0) * 100.0));
    }
    else strcpy(str, "Color Scale: Normal");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugRenderColScale_select(const MenuItem *self, int amount) {
    int val = overrideColorScale + amount;
    if(val <  -1) val = 255;
    if(val > 255) val =  -1;
    overrideColorScale = val;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


Menu menuDebugColor = {
    "Color Filters", 0,
    genericMenu_run, genericMenu_draw, debugRenderSubMenu_close,
    "Color Filter: %s", menuDebugRenderColFiltOn_draw, menuDebugRenderColFiltOn_select,
    "Filter R: %d",     menuDebugRenderColFiltR_draw,  menuDebugRenderColFiltR_select,
    "Filter G: %d",     menuDebugRenderColFiltG_draw,  menuDebugRenderColFiltG_select,
    "Filter B: %d",     menuDebugRenderColFiltB_draw,  menuDebugRenderColFiltB_select,
    "",                 menuDebugRenderColScale_draw,  menuDebugRenderColScale_select,
    NULL,
};
