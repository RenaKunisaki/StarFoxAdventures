#include "main.h"

static int _readHex(const char *str, int len, const char **out) {
    int result = 0;
    while(len--) {
        int n = 0;
        char c = *(str++);
        if(!c) break;
        else if(c >= '0' && c <= '9') n =  c - '0';
        else if(c >= 'A' && c <= 'F') n = (c - 'A') + 10;
        else if(c >= 'a' && c <= 'f') n = (c - 'a') + 10;
        else break;
        result = (result << 4) | n;
    }
    if(out) *out = str;
    return result;
}

static GameTextCharacterStruct* _fontGetChar(GameTextFont *font, int chr) {
    for(int i=0; i<font->numChars; i++) {
        if(font->chars[i].character == chr) {
            return &font->chars[i];
        }
    }
    return NULL;
}

int drawSimpleText(const char *str, int x, int y, int xMax, int yMax, int *outX, int *outY, bool fixed) {
    //0: icons, text, none, none
    //1: none
    //2: error text, none, none, none
    //3: text, none, none, none
    int iFont = 3; //can be changed by control codes

    //OSReport("Font %d has %d chars, %08X", iFont, font->numChars, font->chars);
    Color4b lol;
    lol.value = 0;
    lol.r = 255;
    lol.a = 255;
    gxSetTexEnvColor(0, &lol);
    _gxSetTevColor2(255, 0, 0, 255);
    _gxSetTevColor1(255, 0, 0, 255);
    textureSetupFn_800799c0();
    textRenderSetup();
    textRenderSetupFn_800795e8(); //makes color work (or not)
    textRenderSetupFn_80079804();
    textBlendSetupFn_80078a7c();
    WRITE_NOP(0x8007608c);
    iCacheFlush((void*)0x8007608c, 4);
    //lol we need to duplicate most of drawPartialTexture just to have color
    //because of fucking course we do
    //or can we just use selectTexture and textRenderChar

    int startX = x, startY = y;
    int lineHeight = 0;
    int iChr = 0;
    while(true) {
        int cSize = 0;
        int chr = shiftJisGetNextChar((char*)&str[iChr], &cSize);
        if(!chr) break;
        iChr += cSize;

        if(chr < 0x20) {
            switch(chr) {
                case '\n': y += fixed ? 11 : MAX(lineHeight, 11); //fall thru
                case '\r': x = startX; break;
                case '\t': {
                    int p = x % 64;
                    x += (64 - p);
                    break;
                }

                //nonstandard codes (XXX use ANSI sequences?)
                //\v: toggle fixed width
                case '\v': fixed = !fixed; break;

                //\fN: select font N (N is an ASCII digit)
                case '\f': iFont = str[iChr++] - 0x30; break;

                //\bRRGGBBAA: select color (followed by 8 hex digits)
                case '\b': {
                    Color4b col;
                    col.r = _readHex(str, 2, &str);
                    col.g = _readHex(str, 2, &str);
                    col.b = _readHex(str, 2, &str);
                    col.a = _readHex(str, 2, &str);
                    break;
                }

                default: break;
            }
        }
        else if(chr < 0xE000 || chr > 0xF8FF) {
            //render char
            GameTextFont *font = &gameTextFonts[iFont];
            GameTextCharacterStruct *cStruct = _fontGetChar(font, chr);
            if(!cStruct) cStruct = _fontGetChar(font, '?');
            if(cStruct) {
                int cx      = cStruct->xpos;
                int cy      = cStruct->ypos;
                int cw      = cStruct->width;
                int ch      = cStruct->height;
                int cLeft   = cStruct->left; //padding
                int cTop    = cStruct->top;
                int cRight  = cStruct->right;
                int cBottom = cStruct->bottom;
                drawPartialTexture(x+cLeft, y+cTop, font->texture[cStruct->texture],
                    255, 256, cw, ch, cx, cy); //alpha, scale
                if(fixed) x += 11;
                else x += cw + cRight + cLeft;
                lineHeight = MAX(lineHeight, ch + cTop + cBottom);
            }
        }
        else {
            //XXX handle control codes, Japanese...
            /* SetScale	0xf8f4
            SetFont	0xf8f7
            LeftJustify	0xf8f8
            RightJustify	0xf8f9
            Center	0xf8fa
            FullJustify	0xf8fb
            SetColor	0xf8ff */
        }

        if(x >= xMax) {
            x = startX;
            y += fixed ? 11 : MAX(lineHeight, 11);
        }
        if(y >= yMax) break;
    }
    if(outX) *outX = x;
    if(outY) *outY = y;
    return iChr;
}
