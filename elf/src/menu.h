#define MENU_ANIM_NUM_FRAMES 10
#define MENU_PADDING 8
#define MENU_WIDTH  (SCREEN_WIDTH  * 0.6)
#define MENU_HEIGHT (SCREEN_HEIGHT * 0.4)
#define MENU_XPOS ((SCREEN_WIDTH/2)  - (MENU_WIDTH  / 2))
#define MENU_YPOS ((SCREEN_HEIGHT/2) - (MENU_HEIGHT / 2))
#define MENU_TEXTBOX_ID 0x93 //same as game uses for HUD
#define MENU_LINE_HEIGHT 18
#define MENU_ADJUST_SOUND 0xF4 //generic adjustment sound effect ID
#define MENU_OPEN_SOUND 0xFC //submenu open sound effect ID
#define MENU_CLOSE_SOUND 0xFC //submenu close sound effect ID
#define MENU_FAIL_SOUND 0xFD //"can't use this item" sound

#define DRAW_SCALED_TEXTURE_FLIP_H 0x01
#define DRAW_SCALED_TEXTURE_FLIP_V 0x02
#define DRAW_SCALED_TEXTURE_SCALE_FACTOR 256
#define DRAW_SCALED_TEXTURE_SCALE_ONE 256
#define HUD_TEXTURE_BOX_CORNER   10
#define HUD_TEXTURE_BOX_SIDE     11
#define HUD_TEXTURE_BOX_INTERIOR 12
#define HUD_TEXTURE_BOX_TOP      13
#define BOX_BORDER_WIDTH 5
#define BOX_BORDER_HEIGHT 5
#define MENU_FIXED_WIDTH 11 //pixels for fixed width mode

#define MENU_INPUT_DELAY_CLOSE 8 //when closing menu
#define MENU_INPUT_DELAY_SELECT 8 //when selecting an option
#define MENU_INPUT_DELAY_MOVE 8 //when moving cursor to another option
#define MENU_INPUT_DELAY_MOVE_FAST 0 //when moving cursor with C stick
#define MENU_INPUT_DELAY_ADJUST 8 //when changing an option
#define MENU_INPUT_DELAY_ADJUST_FAST 0 //when changing an option with C stick
#define MENU_ANALOG_STICK_THRESHOLD 20 //how far stick has to move
#define MENU_CSTICK_THRESHOLD 20 //how far C stick has to move
#define MENU_TRIGGER_THRESHOLD 20 //how far L/R have to move

typedef enum {
    MENU_NOT_OPEN,
    MENU_OPENING,
    MENU_OPEN,
    MENU_CLOSING,
} MenuState;

typedef enum {
    //flags telling how the game state was before the menu was opened.
    MENU_FLAG_TIME_STOPPED   = 0x01, //was time stopped?
    MENU_FLAG_PAUSE_DISABLED = 0x02, //was pause disabled?
} MenuGameStateFlags;

typedef struct MenuItem MenuItem;
typedef struct Menu Menu;

typedef struct MenuItem {
    const char *name; //displayed text
    void (*draw)(const MenuItem *self, int x, int y, bool selected); //draw function
    void (*select)(const MenuItem *self, int amount); //select/adjust function
} MenuItem;

typedef struct Menu {
    const char *title; //menu title
    s16 selected; //current selected option
    void (*run  )(Menu *self); //run function
    void (*draw )(Menu *self); //draw function
    void (*close)(const Menu *self); //close function
    MenuItem items[]; //items, ending with one with name = NULL
} Menu;

//menu.c
void openMainMenu();
void drawMenuBox(int cx, int cy, int width, int height);
void genericMenu_draw(Menu *self);
void genericMenu_run(Menu *self);
void genericMenuItem_draw(const MenuItem *self, int x, int y, bool selected);

//menuDebug.c
void debugSubMenu_close(const Menu *self);

//menuDebugGameState.c
void debugGameSubMenu_close(const Menu *self);

//menuDebugMap.c
void debugMapSubMenu_close(const Menu *self);

//menuDebugMisc.c
void debugMiscSubMenu_close(const Menu *self);

//menuDebugRender.c
void debugRenderSubMenu_close(const Menu *self);

//menuMain.c
void mainMenu_close(const Menu *self);
void mainSubMenu_close(const Menu *self);

extern Menu *curMenu;
extern Menu menuAudioSettings;
extern Menu menuControlSettings;
extern Menu menuDebug;
extern Menu menuDebugCheat;
extern Menu menuDebugColor;
extern Menu menuDebugEditPlayer;
extern Menu menuDebugGameBits;
extern Menu menuDebugGameState;
extern Menu menuDebugHexEdit;
extern Menu menuDebugMap;
extern Menu menuDebugMapEnv;
extern Menu menuDebugMisc;
extern Menu menuDebugObjList;
extern Menu menuDebugRender;
extern Menu menuDebugSpawnObj;
extern Menu menuDebugText;
extern Menu menuDebugTextureView;
extern Menu menuDebugWarp;
extern Menu menuGameSettings;
extern Menu menuMain;
extern Menu menuPdaSettings;
extern Menu menuPlayerSettings;
extern Menu menuVideoSettings;

extern u8  menuState;
extern u8  menuAnimFrame;
extern u8  menuInputDelayTimer;
extern u8  menuPrevTimerFlags;
extern u8  menuPrevGameFlags;
