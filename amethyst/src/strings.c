#include "main.h"
#include "revolution/os.h"

//XXX figure out some better way, maybe using the original GameText system

//assign a table to each game language
//untranslated will use English until translation happens
const char **stringTables[] = {
    strings_English, //English
    strings_English, //French
    strings_English, //German
    strings_English, //Italian
    strings_Japanese, //Japanese
    strings_English, //Spanish
};

const char* T(const char *s) {
    const char **table = stringTables[curLanguage];
    if(table == strings_English) return s;

    //look up the string in the English table (XXX cache?)
    for(int i=0; strings_English[i]; i++) {
        //string interning for constant strings
        if(strings_English[i] == s) return table[i];
    }

    //could loop again and strcmp here...
    OSReport("String not found in translation table: \"%s\"", s);
    return s;
}
