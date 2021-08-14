/** Debug GameBits submenu.
 */
#include "main.h"
#include "revolution/os.h"
#define BIT_MENU_XPOS 32
#define BIT_MENU_YPOS 32
#define BIT_MENU_WIDTH  (SCREEN_WIDTH  - (BIT_MENU_XPOS * 2))
#define BIT_MENU_HEIGHT (SCREEN_HEIGHT - (BIT_MENU_YPOS * 2))
#define BIT_MENU_NUM_LINES ((BIT_MENU_HEIGHT / LINE_HEIGHT) - 3)
s8 *bitNames = NULL;
static u8 bitMenuCursorX = 7;

enum {
    BitMenuSortId,
    BitMenuSortName,
    BitMenuSortTable,
    NumBitMenuSortMethods
} BitMenuSortMethods;
static const char *sortModeNames[] = {"ID", "Name", "Table"};
static u8 sortMode = BitMenuSortId;

typedef struct {
    u16 bit;
    const char *name;
} BitMenuListItem;
static BitMenuListItem *sortedList = NULL;

int compareBitsByName(const void *itemA, const void *itemB) {
    BitMenuListItem *A = (BitMenuListItem*)itemA;
    BitMenuListItem *B = (BitMenuListItem*)itemB;
    const char *nA = A->name;
    const char *nB = B->name;

    //if no name, sort by number.
    //prefix with ~ to move to end of list.
    //these names aren't displayed.
    char strA[16], strB[16];
    if(nA[0] == 0) {
        sprintf(strA, "~%04X", A->bit);
        nA = strA;
    }
    if(nB[0] == 0) {
        sprintf(strB, "~%04X", B->bit);
        nB = strB;
    }
    return strcmpi(nA, nB);
}
int compareBitsByTable(const void *itemA, const void *itemB) {
    BitMenuListItem *A = (BitMenuListItem*)itemA;
    BitMenuListItem *B = (BitMenuListItem*)itemB;
    BitTableEntry *entryA = getBitTableEntry(A->bit);
    BitTableEntry *entryB = getBitTableEntry(B->bit);
    int tblA = (entryA->flags >> 6) & 3;
    int tblB = (entryB->flags >> 6) & 3;
    return tblB - tblA;
}

void sortList() {
    if(!sortedList) sortedList = allocTagged(sizeof(BitMenuListItem) * NUM_GAMEBITS,
        ALLOC_TAG_LISTS_COL, "debug:gameBitList");

    //temporary sort buffers
    BitMenuListItem *data = allocTagged(sizeof(BitMenuListItem) * NUM_GAMEBITS,
        ALLOC_TAG_LISTS_COL, "debug:gameBitSort");
    BitMenuListItem **items = allocTagged(sizeof(BitMenuListItem*) * NUM_GAMEBITS,
        ALLOC_TAG_LISTS_COL, "debug:gameBitTemp");
    if(!(sortedList && data && items)) {
        OSReport("GameBit sort alloc failed %08X %08X %08X\n", sortedList, data, items);
        if(items) free(items);
        if(data) free(data);
        if(sortedList) free(sortedList);
        sortedList = NULL;
        return;
    }

    for(int i=0; i<NUM_GAMEBITS; i++) {
        data[i].bit = i;
        data[i].name = getBitName(i);
        items[i] = &data[i];
    }

    CompareFunc func = NULL;
    switch(sortMode) {
        case BitMenuSortName:  func = compareBitsByName;  break;
        case BitMenuSortTable: func = compareBitsByTable; break;
        default: break;
    }

    if(func) quicksort((const void**)items, 0, NUM_GAMEBITS - 1, func);

    //copy sorted items into buffer
    for(int i=0; i<NUM_GAMEBITS; i++) {
        sortedList[i].bit  = items[i]->bit;
        sortedList[i].name = items[i]->name;
    }
    free(items);
    free(data);
}


BitTableEntry* getBitTableEntry(int bit) {
    BitTableEntry *bitTable = (BitTableEntry*)dataFileBuffers[FILE_BITTABLE_BIN];
    return &bitTable[bit];
}

const char* getBitName(int bit) {
    if(!bitNames) {
        //try to load bitnames.dat
        bitNames = loadFileByPath("bitnames.dat", NULL);
        if(!bitNames) return "";
        registerFreeablePtr((void**)&bitNames, "bitnames.dat");
    }

    //this is hilariously inefficient time-wise, but saves a ton of memory.
    const char *res = bitNames;
    while(bit > 0) {
        if(res[0] == 0) bit--;
        res++;
    }
    return res;
}

void bitMenu_draw(Menu *self) {
    //Draw function for GameBit menu
    char str[256];
    menuAnimFrame++;

    drawMenuBox(BIT_MENU_XPOS, BIT_MENU_YPOS, BIT_MENU_WIDTH, BIT_MENU_HEIGHT);
    int x = BIT_MENU_XPOS + MENU_PADDING, y = BIT_MENU_YPOS + MENU_PADDING;

    if(!sortedList) return;

    //Draw title and header
    //box type 0 is (center, y+40), no background
    gameTextSetColor(255, 255, 255, 255);
    gameTextShowStr(self->title, 0, x, y-40);

    sprintf(str, "\eF%s %s\t%s\t%s", T("Bit"), T("T"), T("Sz"), T("Value"));
    menuDrawText(str, x, y, false);

    int start   = MAX(0, self->selected - (BIT_MENU_NUM_LINES-1));
    int bitIdx  = start - 1;
    int nLines  = 0;
    int cursorY = 0;
    while(nLines < BIT_MENU_NUM_LINES && bitIdx < NUM_GAMEBITS) {
        bitIdx++;
        int bit = sortedList[bitIdx].bit;
        BitTableEntry *entry = getBitTableEntry(bit);
        int tbl = (entry->flags >> 6) & 3;
        y += LINE_HEIGHT;
        nLines++;
        //debugPrintf(DPRINT_FIXED "Bit %04X t %d sz %2d offs %04X\n" DPRINT_NOFIXED,
        //    bitIdx, tbl, entry->flags & GameBitFlags_Size, entry->offset);

        bool selected = bitIdx == self->selected;
        if(selected) {
            cursorY = y;

            //display offset and address
            void *tblAddr;
            switch(tbl) {
                case 0: default: tblAddr = tempGameBits; break;
                case 1: tblAddr = pLastSavedGame->gameBits1; break;
                case 2: tblAddr = pLastSavedGame->gameBits2; break;
                case 3: tblAddr = pLastSavedGame->gameBits3; break;
            }
            debugPrintf("Bit " DPRINT_FIXED "T=%d offs=%04X -> %08X\n" DPRINT_NOFIXED,
                tbl, entry->offset, tblAddr + (entry->offset >> 3));
        }

        sprintf(str, "\eF%04X %d %2d %08X %s", bit, tbl,
            (entry->flags & GameBitFlags_Size) + 1,
            mainGetBit(bit), sortedList[bitIdx].name);
        menuDrawText(str, x, y, selected);
    }

    //draw instructions
    y += LINE_HEIGHT;
    gameTextSetColor(255, 255, 255, 255);
    sprintf(str, "B:%s X:+ Y:- L/R:%s Z:%s:%s",
        T("Exit"), T("Page"), T("Sort"), T(sortModeNames[sortMode]));
    menuDrawText(str, x, y, false);

    //draw cursor
    int w = (curLanguage == LANG_JAPANESE) ? FIXED_CHR_WIDTH_JAPANESE : FIXED_CHR_WIDTH;
    drawBox(BIT_MENU_XPOS + 4 + ((bitMenuCursorX + 10) * w),
        cursorY - 4, 20, 24, 255, false);
}

static void moveCursorY(Menu *self, int amount) {
    int sel = self->selected + amount;
    if(sel < 0) sel = NUM_GAMEBITS - 1;
    if(sel >= NUM_GAMEBITS) sel = 0;
    self->selected = sel;
}

void bitMenu_run(Menu *self) {
    //Run function for GameBit menu
    if(!sortedList) sortList();
    if(!sortedList) { //out of memory
        gameTextSetColor(255, 0, 0, 255);
        gameTextShowStr(T("OUT OF MEMORY"), MENU_TEXTBOX_ID,
            BIT_MENU_XPOS + MENU_PADDING, BIT_MENU_YPOS + MENU_PADDING);
        if(buttonsJustPressed == PAD_BUTTON_B) {
            menuInputDelayTimer = MENU_INPUT_DELAY_CLOSE;
            curMenu->close(curMenu);
        }
        return;
    }

    if(buttonsJustPressed == PAD_BUTTON_B) {
        if(sortedList) free(sortedList);
        sortedList = NULL;
        menuInputDelayTimer = MENU_INPUT_DELAY_CLOSE;
        curMenu->close(curMenu);
    }
    else if(buttonsJustPressed == PAD_BUTTON_X
    ||      buttonsJustPressed == PAD_BUTTON_Y) {
        int bit = sortedList[self->selected].bit;
        menuInputDelayTimer = MENU_INPUT_DELAY_ADJUST;
        u32 val = mainGetBit(bit);
        u32 mask = 0xF << ((7-bitMenuCursorX) * 4);
        int adj  = (buttonsJustPressed == PAD_BUTTON_Y) ? -1 : 1;
        adj <<= ((7-bitMenuCursorX) * 4);
        val = (val & ~mask) | ((val + adj) & mask);
        mainSetBits(bit, val);
    }
    else if(buttonsJustPressed == PAD_TRIGGER_Z) {
        sortMode++;
        if(sortMode >= NumBitMenuSortMethods) sortMode = 0;
        sortList();
        menuInputDelayTimer = MENU_INPUT_DELAY_ADJUST;
        //XXX re-select previous selection
    }
    else if(controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY > MENU_CSTICK_THRESHOLD) { //up
        menuInputDelayTimer =
            (controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        moveCursorY(self, -1);
    }
    else if(controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY < -MENU_CSTICK_THRESHOLD) { //down
        menuInputDelayTimer = (controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        moveCursorY(self, 1);
    }
    else if(controllerStates[0].stickX < -MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickX < -MENU_CSTICK_THRESHOLD) { //left
        menuInputDelayTimer = (controllerStates[0].stickX < -MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        bitMenuCursorX = (bitMenuCursorX - 1) & 7;
    }
    else if(controllerStates[0].stickX > MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickX > MENU_CSTICK_THRESHOLD) { //right
        menuInputDelayTimer = (controllerStates[0].stickX > MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        bitMenuCursorX = (bitMenuCursorX + 1) & 7;
    }
    else if(controllerStates[0].triggerLeft > MENU_TRIGGER_THRESHOLD) { //L
        moveCursorY(self, -0x100);
        menuInputDelayTimer = MENU_INPUT_DELAY_MOVE;
    }
    else if(controllerStates[0].triggerRight > MENU_TRIGGER_THRESHOLD) { //R
        moveCursorY(self, 0x100);
        menuInputDelayTimer = MENU_INPUT_DELAY_MOVE;
    }
}

Menu menuDebugGameBits = {
    "GameBits", 0,
    bitMenu_run, bitMenu_draw, debugGameSubMenu_close,
    NULL,
};
