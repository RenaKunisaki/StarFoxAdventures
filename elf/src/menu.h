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
    void (*run)(const Menu *self); //run function
    void (*close)(const Menu *self); //close function
    MenuItem items[]; //items, ending with one with name = NULL
} Menu;

//menuMain.c
void mainSubMenu_close(const Menu *self);
void genericMenu_run(const Menu *self);
void genericMenuItem_draw(const MenuItem *self, int x, int y, bool selected);

extern Menu *curMenu;
extern Menu menuMain;
extern Menu menuGameSettings;
extern Menu menuControlSettings;
extern Menu menuPlayerSettings;
extern Menu menuVideoSettings;
extern Menu menuAudioSettings;
extern Menu menuPdaSettings;

extern u8  menuState;
extern u8  menuAnimFrame;
extern u8  menuInputDelayTimer;
extern u8  menuPrevTimerFlags;
extern u8  menuPrevGameFlags;
