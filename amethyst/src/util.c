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

void getObjFileName(char *dest, ObjectFileStruct *file) {
    //get name of object from file
    //dest must be at least 12 bytes

    //copy the name, filtering out any control codes.
    if(PTR_VALID(file)) {
        int p = 0;
        for(int i=0; i<11; i++) {
            char c = file->name[i];
            if(c == 0) break;
            else if(c >= 0x20 && c <= 0x7E) dest[p++] = c;
        }
        dest[p] = 0;
    }
    else strcpy(dest, "N/A");
}

void getObjName(char *dest, ObjInstance *obj) {
    if(PTR_VALID(obj)) getObjFileName(dest, obj->file);
    else strcpy(dest, "N/A");
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

//compare functions for sorting object lists
int compareObjsByType(const void *objA, const void *objB) {
    ObjInstance *A = ((ObjInstance*)objA);
    ObjInstance *B = ((ObjInstance*)objB);
    return ((A ? A->defNo : 0) - (B ? B->defNo : 0));
}

int compareObjsById(const void *objA, const void *objB) {
    ObjInstance *A = ((ObjInstance*)objA);
    ObjInstance *B = ((ObjInstance*)objB);
    return ((A && A->objDef ? A->objDef->id : 0) -
        (B && B->objDef ? B->objDef->id : 0));
}

int compareObjsByName(const void *objA, const void *objB) {
    ObjInstance *A = ((ObjInstance*)objA);
    ObjInstance *B = ((ObjInstance*)objB);
    if(A && A->file && B && B->file) {
        return strcmpi(A->file->name, B->file->name);
    }
    else if(A && A->file) return -1;
    else return 1;
}

int compareObjsByDistance(const void *objA, const void *objB) {
    ObjInstance *focus = pCamera ? pCamera->focus : NULL;
    if(!focus) focus = pPlayer;
    if(!focus) return 0; //should not be allowed to reach here.
        //ensure this method isn't used if this condition exists.
    vec3f *fp = &focus->pos.pos;

    ObjInstance *A = ((ObjInstance*)objA);
    ObjInstance *B = ((ObjInstance*)objB);
    float dA = A ? vec3f_distance(&A->pos.pos, fp) : 0;
    float dB = B ? vec3f_distance(&B->pos.pos, fp) : 0;
    if(dA < dB) return -1;
    if(dA > dB) return  1;
    return 0;
}

int compareObjsByAddr(const void *objA, const void *objB) {
    return objA - objB;
}

double u64toDouble(u64 val) {
    //this is necessary to make gcc not try to use soft float.
    //XXX is 8028656c the same as this?
    u32 hi = val >> 32LL;
    u32 lo = val & 0xFFFFFFFFLL;
    return (double)lo + (double)(hi * 4294967296.0);
}

double ticksToSecs(u64 ticks) {
    return u64toDouble(ticks) / (__OSBusClock / 4);
}

/** Convert HSV color to RGB.
 *  @param h Hue (0..255 = 0..359 degrees)
 *  @param s Saturation (0..255 = 0..100%)
 *  @param v Value (0..255 = 0..100%)
 *  @param a Alpha (0..255 = 0..100%)
 *  @return a Color4b with the corresponding RGBA values.
 */
Color4b hsv2rgb(u8 h, u8 s, u8 v, u8 a) {
    float sf = s / 255.0;
    float vf = v / 255.0;
    float c = vf * sf;
    int h2 = h / 43; //h / 60 degrees
    int n = (h2 % 2 - 1);
    if(n < 0) n = -n;
    float x = c * (1 - n);
    float r=0, g=0, b=0;
    switch(h2) {
        case 0: r=c; g=x, b=0; break;
        case 1: r=x; g=c, b=0; break;
        case 2: r=0; g=c, b=x; break;
        case 3: r=0; g=x, b=c; break;
        case 4: r=x; g=0, b=c; break;
        case 5: r=c; g=0, b=x; break;
    }
    float m = vf - c;
    Color4b res = {.r = (r+m)*255, .g = (g+m)*255, .b = (b+m)*255, .a = a};
    return res;
}
