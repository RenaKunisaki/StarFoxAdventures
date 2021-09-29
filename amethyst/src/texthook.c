#include "main.h"


int textHook(char *str) {
    //replaced a call to 800e7e94: BOOL saveFileStruct:isCheatActive(CheatFlag flag)
    //checking if the "dino language" cheat is on.
    //r26 = the message to translate.
    bool leap = (curYear % 4) == 0;
    if(dayOfYear != (leap ? 92 : 91)) {
        return isCheatActive(CheatFlag_DinoLanguage);
    }

    while(*str) {
        int cSize;
        int c = utf8GetNextChar(str, &cSize);
        if(!c) break;
        if     (c == 'l' || c == 'r') *str = 'w';
        else if(c == 'L' || c == 'R') *str = 'W';
        else cSize += getControlCharLen(c) * 2;
        str += cSize;
    }
    return 0;
}


void _textHook(void);
__asm__(
    "_textHook:   \n"
    "mr   3, 26   \n"
    "b    textHook\n"
);

void textHookInit() {
    hookBranch(0x800175bc, _textHook, 1);
}
