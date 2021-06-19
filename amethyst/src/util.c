#include "main.h"

/** @description Convert value to binary string.
 *  @param str the destination string.
 *  @param val the value to convert.
 *  @param len the number of bits to write.
 *  @return pointer to the next byte after the last character written in str.
 *  @note does not perform any sort of bounds check.
 *  @note bits are written MSB-first, starting `len` from the LSB.
 *   so eg a `len` of 8 will write the lowest 8 bits of `val`.
 */
char* bin2str(char *str, u32 val, int len) {
    u32 mask = 1 << (len-1);
    for(int i=0; i<len; i++) {
        *(str++) = (val & mask) ? '1' : '0';
        mask >>= 1;
    }
    *str = '\0';
    return str;
}

/** @description Round number up or down to nearest multiple of target.
 *  @param num number to round.
 *  @param target multiple to round to.
 *  @return rounded number.
 */
int roundTo(int num, int target) {
    int val = num % target;
    if(!val) return num;
    if(val >= target / 2) return num + (target - val); //round up
    else return num - val; //round down
}


int strcmpi(const char *sa, const char *sb) {
    while(*sa && *sb) {
        char a = *(sa++);
        char b = *(sb++);
        if(a >= 'a' && a <= 'z') a -= 0x20;
        if(b >= 'a' && b <= 'z') b -= 0x20;
        if(a != b) return a - b;
    }
    return *sa - *sb;
}
