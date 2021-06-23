#include "main.h"

#define LIGHT_MENU_XPOS 32
#define LIGHT_MENU_YPOS 40
#define LIGHT_MENU_WIDTH (SCREEN_WIDTH - (LIGHT_MENU_XPOS * 2))
#define LIGHT_MENU_HEIGHT (SCREEN_HEIGHT - (LIGHT_MENU_YPOS * 2))

void lightMenu_draw(Menu *self) {
    //Draw function for Light List menu
    char str[256];
    Light *light = g_lights[self->selected];
    int x = LIGHT_MENU_XPOS + MENU_PADDING, y = LIGHT_MENU_YPOS + MENU_PADDING;

    menuAnimFrame++;
    drawMenuBox(LIGHT_MENU_XPOS, LIGHT_MENU_YPOS, LIGHT_MENU_WIDTH, LIGHT_MENU_HEIGHT);
    gameTextSetColor(255, 255, 255, 255);

    sprintf(str, "#%2d %08X", self->selected, light);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;
    if(!light) return;

    char name[12];
    getObjName(name, light->obj);
    sprintf(str, "Obj: %08X %s", light->obj, name);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "V004 %f %f %f", light->unk04.x, light->unk04.y, light->unk04.z);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;
    
    sprintf(str, "V010 %f %f %f", light->lightVec.x, light->lightVec.y, light->lightVec.z);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;
    
    sprintf(str, "V01C %f %f %f", light->unk1C.x, light->unk1C.y, light->unk1C.z);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;
    
    sprintf(str, "V028 %f %f %f", light->vIn.x, light->vIn.y, light->vIn.z);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;
    
    sprintf(str, "V034 %f %f %f", light->vOut.x, light->vOut.y, light->vOut.z);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;
    
    sprintf(str, "V040 %f %f %f", light->vCamera.x, light->vCamera.y, light->vCamera.z);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;
    
    sprintf(str, "C0A8 %02X %02X %02X %02X", light->lightColorA8.r, light->lightColorA8.g, 
        light->lightColorA8.b, light->lightColorA8.a);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;
    
    sprintf(str, "C100 %02X %02X %02X %02X", light->lightColor.r, light->lightColor.g, 
        light->lightColor.b, light->lightColor.a);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;
    
    sprintf(str, "C104 %02X %02X %02X %02X", light->unk104.r, light->unk104.g, 
        light->unk104.b, light->unk104.a);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;
    
    sprintf(str, "C108 %02X %02X %02X %02X", light->unk108.r, light->unk108.g, 
        light->unk108.b, light->unk108.a);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;
}

void lightMenu_run(Menu *self) {
    //Run function for Light List menu
    textForceFixedWidth = MENU_FIXED_WIDTH;

    if(buttonsJustPressed == PAD_BUTTON_B) { //close menu
        menuInputDelayTimer = MENU_INPUT_DELAY_CLOSE;
        self->close(curMenu);
    }
    else if(buttonsJustPressed == PAD_TRIGGER_Z) { //edit
        menuInputDelayTimer = MENU_INPUT_DELAY_SELECT;
        hexEditPrevMenu = curMenu;
        hexEditAddr = (u32)g_lights[self->selected];
        curMenu = &menuDebugHexEdit;
        audioPlaySound(NULL, MENU_OPEN_SOUND);
    }
    else if(controllerStates[0].stickX < -MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickX < -MENU_CSTICK_THRESHOLD) { //left
        menuInputDelayTimer = (controllerStates[0].stickX < -MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        self->selected -= 1;
    }
    else if(controllerStates[0].stickX > MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickX > MENU_CSTICK_THRESHOLD) { //right
        menuInputDelayTimer = (controllerStates[0].stickX > MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        self->selected += 1;
    }
    if(self->selected >= MAX_LIGHTS) self->selected = 0;
    if(self->selected < 0) self->selected = MAX_LIGHTS - 1;
}

void lightMenu_close(const Menu *self) {
    //Close function for Light List menu
    curMenu = &menuDebugMapEnv;
    audioPlaySound(NULL, MENU_CLOSE_SOUND);
}

Menu menuDebugMapEnvLights = {
    "Lights", 0,
    lightMenu_run, lightMenu_draw, lightMenu_close,
    NULL,
};
