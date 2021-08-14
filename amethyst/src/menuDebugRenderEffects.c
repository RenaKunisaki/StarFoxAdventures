#include "main.h"

void menuDebugRenderEffectsBlur_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), bEnableBlurFilter ? T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuDebugRenderEffectsBlur_select(const MenuItem *self, int amount) {
    bEnableBlurFilter = !bEnableBlurFilter;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugRenderEffectsMotBlur_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), (int)motionBlurIntensity);
    menuDrawText(str, x, y, selected);
}
void menuDebugRenderEffectsMotBlur_select(const MenuItem *self, int amount) {
    int val = motionBlurIntensity + amount;
    if(val <   0) val = 255;
    if(val > 255) val =   0;
    motionBlurIntensity = val;
    bEnableMotionBlur = (val > 0);
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugRenderEffectsHeatFx_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), heatEffectIntensity);
    menuDrawText(str, x, y, selected);
}
void menuDebugRenderEffectsHeatFx_select(const MenuItem *self, int amount) {
    int val = heatEffectIntensity + amount;
    if(val <   0) val = 255;
    if(val > 255) val =   0;
    heatEffectIntensity = val;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugRenderEffectsMono_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), bEnableMonochromeFilter ? T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuDebugRenderEffectsMono_select(const MenuItem *self, int amount) {
    bEnableMonochromeFilter = !bEnableMonochromeFilter;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugRenderEffectsSpirit_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), bEnableSpiritVision ? T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuDebugRenderEffectsSpirit_select(const MenuItem *self, int amount) {
    bEnableSpiritVision = !bEnableSpiritVision;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugRenderColFiltOn_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), bEnableColorFilter ? T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuDebugRenderColFiltOn_select(const MenuItem *self, int amount) {
    bEnableColorFilter = !bEnableColorFilter;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugRenderColFiltR_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), colorFilterColor.r);
    menuDrawText(str, x, y, selected);
}
void menuDebugRenderColFiltR_select(const MenuItem *self, int amount) {
    int val = colorFilterColor.r + amount;
    if(val <   0) val = 255;
    if(val > 255) val =   0;
    colorFilterColor.r = val;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugRenderColFiltG_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), colorFilterColor.g);
    menuDrawText(str, x, y, selected);
}
void menuDebugRenderColFiltG_select(const MenuItem *self, int amount) {
    int val = colorFilterColor.g + amount;
    if(val <   0) val = 255;
    if(val > 255) val =   0;
    colorFilterColor.g = val;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugRenderColFiltB_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), colorFilterColor.b);
    menuDrawText(str, x, y, selected);
}
void menuDebugRenderColFiltB_select(const MenuItem *self, int amount) {
    int val = colorFilterColor.b + amount;
    if(val <   0) val = 255;
    if(val > 255) val =   0;
    colorFilterColor.b = val;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugRenderColScale_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    if(overrideColorScale >= 0){
        sprintf(str, "%s: %d%%", T("Color Scale"), (int)((overrideColorScale / 255.0) * 100.0));
    }
    else sprintf(str, "%s: %s", T("Color Scale"), T("Normal"));
    menuDrawText(str, x, y, selected);
}
void menuDebugRenderColScale_select(const MenuItem *self, int amount) {
    int val = overrideColorScale + amount;
    if(val <  -1) val = 255;
    if(val > 255) val =  -1;
    overrideColorScale = val;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}

Menu menuDebugRenderEffects = {
    "Effects", 0,
    genericMenu_run, genericMenu_draw, debugRenderSubMenu_close,
    "Blur Filter",   "%s: %s", menuDebugRenderEffectsBlur_draw,     menuDebugRenderEffectsBlur_select,
    "Motion Blur",   "%s: %d", menuDebugRenderEffectsMotBlur_draw,  menuDebugRenderEffectsMotBlur_select,
    "Heat Effect",   "%s: %d", menuDebugRenderEffectsHeatFx_draw,   menuDebugRenderEffectsHeatFx_select,
    "Monochrome",    "%s: %s", menuDebugRenderEffectsMono_draw,     menuDebugRenderEffectsMono_select,
    "Spirit Vision", "%s: %s", menuDebugRenderEffectsSpirit_draw,   menuDebugRenderEffectsSpirit_select,
    "Color Filter",  "%s: %s", menuDebugRenderColFiltOn_draw,       menuDebugRenderColFiltOn_select,
    "Filter R",      "%s: %d", menuDebugRenderColFiltR_draw,        menuDebugRenderColFiltR_select,
    "Filter G",      "%s: %d", menuDebugRenderColFiltG_draw,        menuDebugRenderColFiltG_select,
    "Filter B",      "%s: %d", menuDebugRenderColFiltB_draw,        menuDebugRenderColFiltB_select,
    "", "%s",                  menuDebugRenderColScale_draw,        menuDebugRenderColScale_select,
    NULL,
};
