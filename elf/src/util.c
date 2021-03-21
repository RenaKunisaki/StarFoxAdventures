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
