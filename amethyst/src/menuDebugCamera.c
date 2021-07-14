#include "main.h"


static const char *camModeNames[] = {"Normal", "Stay", "Free", "Bird's Eye"};
void menuDebugCamMode_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, camModeNames[debugCameraMode]);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    debugPrintf("pCamera = %08X\n", pCamera);
}
void menuDebugCamMode_select(const MenuItem *self, int amount) {
    debugCameraMode += amount;
    if(debugCameraMode < 0) debugCameraMode = NUM_CAM_MODES - 1;
    if(debugCameraMode >= NUM_CAM_MODES) debugCameraMode = 0;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


Menu menuDebugCamera = {
    "Camera", 0, genericMenu_run, genericMenu_draw, debugSubMenu_close,
    "Mode: %s", menuDebugCamMode_draw, menuDebugCamMode_select,
    NULL,
};
