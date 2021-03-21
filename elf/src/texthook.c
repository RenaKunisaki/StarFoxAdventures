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
        char c = shiftJisGetNextChar(str, &cSize);
        if(!c) break;
        if     (c == 'l' || c == 'r') *str = 'w';
        else if(c == 'L' || c == 'R') *str = 'W';
        else { //of course this can't be simple
            for(int i=0; i<46; i++) {
                if(controlCharLength[i].chr == c) {
                    cSize = controlCharLength[i].len * 2;
                    break;
                }
            }
        }
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
