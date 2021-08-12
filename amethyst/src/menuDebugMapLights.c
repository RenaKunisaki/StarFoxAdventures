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

    sprintf(str, "\eF#%2d/%2d %08X %s %d%%", self->selected, nLights, light,
        light && light->bOn ? "On " : "Off",
        (int)((light ? light->lightAmount : 0.0f) * 100.0f));
    menuDrawText(str, x, y, false);
    y += MENU_LINE_HEIGHT;
    if(!light) return;

    sprintf(str, "\eFMode %d State %d Flags %02X", light->mode, light->state, light->lightFlags_0x64);
    menuDrawText(str, x, y, false);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eFDAttn %5.2f %5.2f %5.2f", light->distAttenA0, light->distAttenA1, light->distAttenA2);
    menuDrawText(str, x, y, false);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eFRefDist %5.2f FadeTgt %5.2f Spd %5.2f", light->refDistance, light->fadeTarget, light->fadeSpeed);
    menuDrawText(str, x, y, false);
    y += MENU_LINE_HEIGHT;

    char name[12];
    getObjName(name, light->obj);
    sprintf(str, "\eFObj: %08X %s", light->obj, name);
    menuDrawText(str, x, y, false);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eFv004   %5.2f %5.2f %5.2f", light->unk04.x, light->unk04.y, light->unk04.z);
    menuDrawText(str, x, y, false);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eFvLight %5.2f %5.2f %5.2f", light->vLight.x, light->vLight.y, light->vLight.z);
    menuDrawText(str, x, y, false);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eFvPos   %5.2f %5.2f %5.2f", light->vPos.x, light->vPos.y, light->vPos.z);
    menuDrawText(str, x, y, false);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eFvIn    %5.2f %5.2f %5.2f", light->vIn.x, light->vIn.y, light->vIn.z);
    menuDrawText(str, x, y, false);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eFvOut   %5.2f %5.2f %5.2f", light->vOut.x, light->vOut.y, light->vOut.z);
    menuDrawText(str, x, y, false);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eFvSpec  %5.2f %5.2f %5.2f", light->vSpec.x, light->vSpec.y, light->vSpec.z);
    menuDrawText(str, x, y, false);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eFC0A8 %02X %02X %02X %02X", light->lightColorA8.r, light->lightColorA8.g,
        light->lightColorA8.b, light->lightColorA8.a);
    menuDrawText(str, x, y, false);
    gameTextSetColor(light->lightColorA8.r, light->lightColorA8.g,  light->lightColorA8.b, 255);
    gameTextShowStr("@@", MENU_TEXTBOX_ID, x+200, y);
    gameTextSetColor(255, 255, 255, 255);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eFC100 %02X %02X %02X %02X", light->lightColor.r, light->lightColor.g,
        light->lightColor.b, light->lightColor.a);
    menuDrawText(str, x, y, false);
    gameTextSetColor(light->lightColor.r, light->lightColor.g,  light->lightColor.b, 255);
    gameTextShowStr("@@", MENU_TEXTBOX_ID, x+200, y);
    gameTextSetColor(255, 255, 255, 255);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eFC104 %02X %02X %02X %02X", light->unk104.r, light->unk104.g,
        light->unk104.b, light->unk104.a);
    menuDrawText(str, x, y, false);
    gameTextSetColor(light->unk104.r, light->unk104.g,  light->unk104.b, 255);
    gameTextShowStr("@@", MENU_TEXTBOX_ID, x+200, y);
    gameTextSetColor(255, 255, 255, 255);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eFC108 %02X %02X %02X %02X", light->unk108.r, light->unk108.g,
        light->unk108.b, light->unk108.a);
    menuDrawText(str, x, y, false);
    gameTextSetColor(light->unk108.r, light->unk108.g,  light->unk108.b, 255);
    gameTextShowStr("@@", MENU_TEXTBOX_ID, x+200, y);
    gameTextSetColor(255, 255, 255, 255);
    y += MENU_LINE_HEIGHT;
}

void lightMenu_run(Menu *self) {
    //Run function for Light List menu
    Light *light = g_lights[self->selected];

    if(buttonsJustPressed == PAD_BUTTON_B) { //close menu
        menuInputDelayTimer = MENU_INPUT_DELAY_CLOSE;
        self->close(curMenu);
    }
    else if(buttonsJustPressed == PAD_BUTTON_A) { //turn on/off
        menuInputDelayTimer = MENU_INPUT_DELAY_SELECT;
        if(light) light->bOn = !light->bOn;
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
