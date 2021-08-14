//common among object menu and its submenus

#define OBJ_MENU_XPOS 2
#define OBJ_MENU_YPOS 40
#define OBJ_MENU_WIDTH  310
#define OBJ_MENU_HEIGHT 438
#define OBJ_INFO_XPOS (OBJ_MENU_XPOS + OBJ_MENU_WIDTH)
#define OBJ_INFO_YPOS 40
#define OBJ_INFO_WIDTH  330
#define OBJ_INFO_HEIGHT 438
#define OBJ_MENU_NUM_LINES ((OBJ_MENU_HEIGHT / LINE_HEIGHT) - 1)

//menuDebugObjs.c
extern ObjInstance *objMenuSelected;
void objListSubmenu_close(const Menu *self);
void objMenu_drawObjInfo(ObjInstance *obj);
