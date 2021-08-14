#include "main.h"
#include "revolution/os.h"

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

void _drawChar(GameTextCharacterStruct *chr, GameTextFont *font, int x, int y,
u8 alpha, float scale) {
    drawPartialTexture(x + chr->left, y + chr->top, font->texture[chr->texture],
        alpha, (int)(scale * 256), chr->width, chr->height, chr->xpos, chr->ypos);
}

void _drawCharColored(GameTextCharacterStruct *chr, GameTextFont *font, int x, int y,
Color4b color, float scale) {
    static volatile float *fifoFloat = (volatile float*)GX_FIFO_BASE;
    static volatile u8    *fifoU8    = (volatile u8*)   GX_FIFO_BASE;
    static volatile s16   *fifoS16   = (volatile s16*)  GX_FIFO_BASE;

    int cx      = chr->xpos;
    int cy      = chr->ypos;
    int cw      = chr->width;
    int ch      = chr->height;
    int cLeft   = chr->left; //padding
    int cTop    = chr->top;
    int cRight  = chr->right;
    int cBottom = chr->bottom;
    Texture *texture = font->texture[chr->texture];

    int   w   = cw * 4 * scale;
    int   h   = ch * 4 * scale;
    int   sx  = hudScale * (x + chr->left);
    int   sy  = hudScale * (y + chr->top);
    float vs1 = (float)cx / (float)texture->width;
    float vt1 = (float)cy / (float)texture->height;
    float vs2 = (float)(cw + cx) / (float)texture->width;
    float vt2 = (float)(ch + cy) / (float)texture->height;
    s16 z = -8;

    //XXX is there any function in the game for this?
    //this is just a combination of hudDrawColored and drawPartialTexture,
    //plus some settings pulled from textRenderStr.
    //and yes we need to do this for every single character
    gxResetVtxDescr();
    gxSetVtxDescr(GX_VA_PNMTXIDX, GX_DIRECT);
    gxSetVtxDescr(GX_VA_POS,      GX_DIRECT);
    gxSetVtxDescr(GX_VA_TEX0,     GX_DIRECT);
    gxSetTexEnvColor(0, &color);
    gxSetTevKsel(0, 0xc);
    gxSetKSel(0, 0x1c);

    gxSetRasTref(0, 0, 0, 0xff);
    gxResetIndCmd(0);
    gxSetTexColorEnv0(0, GX_CC_C1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_KONST);
    _gxSetTexColorEnv(0,7,2,4,7); //XXX these names are wrong
    gxSetTexColorEnv1(1,0,0);
    _gxSetTexColorEnv0(0,0,0,0,1,0);
    _gxSetTexColorEnv1(0,0,0,0,1,0);

    if (texture->unk50 == 0) {
        gxTextureFn_8025c2a0(1);
    }
    else {
        gxSetKSel(1, 0x1c);
        gxSetRasTref(0, 0, 1, 0xff);
        gxSetTexColorEnv0(1,0xf,0xf,0xf,0);
        _gxSetTexColorEnv(1,7,4,6,7);
        gxSetTexColorEnv1(1,0,0);
        _gxSetTexColorEnv0(1,0,0,0,1,0);
        _gxSetTexColorEnv1(1,0,0,2,1,0);
        gxTextureFn_8025c2a0(2);
    }
    gxTextureFn_8025b6f0(0);
    gxSetColorCtrl(4,0,0,0,0,0,2);
    gxSetColorCtrl(5,0,0,0,0,0,2);
    gxSetNumColors(1);
    gxSetNumTextures(1);
    gxSetTextureParams(0,1,4,0x3c,0,0x7d);
    textureFn_8004c264(texture,0);
    gxSetBackfaceCulling(0);
    gxSetProjection(&hudMatrix, TRUE);
    gxSetZMode_(0, 7, 0);
    gxSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);

    _gxSetTevColor2(color.r, color.g, color.g, color.a);
    _gxSetTevColor1(color.r, color.g, color.g, color.a);

    GXBegin(GX_DRAW_QUADS, 1, 4);
    //PNMTXIDX     Vx             Vy             Vz          Vs              Vt
    *fifoU8=0x3C; *fifoS16=sx;   *fifoS16=sy;   *fifoS16=z; *fifoFloat=vs1; *fifoFloat=vt1;
    *fifoU8=0x3C; *fifoS16=sx+w; *fifoS16=sy;   *fifoS16=z; *fifoFloat=vs2; *fifoFloat=vt1;
    *fifoU8=0x3C; *fifoS16=sx+w; *fifoS16=sy+h; *fifoS16=z; *fifoFloat=vs2; *fifoFloat=vt2;
    *fifoU8=0x3C; *fifoS16=sx;   *fifoS16=sy+h; *fifoS16=z; *fifoFloat=vs1; *fifoFloat=vt2;

    resetPerspectiveMtxs();
}

int _returnFindChar(GameTextFont *font, GameTextCharacterStruct *cStruct,
GameTextFont **outFont, GameTextCharacterStruct **outChr, int ret) {
    if(outFont) *outFont = font;
    if(outChr)  *outChr  = cStruct;
    return ret;
}

int _findCharInAnyFont(int chr, GameTextFont **outFont, GameTextCharacterStruct **outChr) {
    //fonts:
    //0: icons/faces, some letters
    //1: doesn't exist?
    //2: big letters
    //3: normal letters
    static s8 fontIdxsE[] = {3, 2, 0, 1, -1};
    static s8 fontIdxsJ[] = {3, 2, 1, 0, -1};
    s8 *fontIdxs = (curLanguage == LANG_JAPANESE) ? fontIdxsJ : fontIdxsE;
    for(int i=0; fontIdxs[i] >= 0; i++) {
        //don't fall back to icons
        if(fontIdxs[i] == 0) {
            bool ok = true;
            static const char *icons = "ABCDFIJS";
            for(int j=0; icons[j]; j++) {
                if(icons[j] == chr) {
                    ok = false;
                    break;
                }
            }
            if(!ok) continue; //skip font 0 for this character
        }
        GameTextFont *font = &gameTextFonts[fontIdxs[i]];
        GameTextCharacterStruct *cStruct = NULL;
        if(font) cStruct = _fontGetChar(font, chr);
        if(cStruct) return _returnFindChar(font, cStruct, outFont, outChr, 1);
    }
    return 0;
}

int findChar(int chr, GameTextFont *font, GameTextFont **outFont, GameTextCharacterStruct **outChr) {
    //GameTextFont *font = &gameTextFonts[iFont];
    GameTextCharacterStruct *cStruct = _fontGetChar(font, chr);
    if(cStruct) return _returnFindChar(font, cStruct, outFont, outChr, 1);

    //try alternate fonts
    int r = _findCharInAnyFont(chr, outFont, outChr);
    if(r) return r;

    //try alternate case
    //if((chr >= 'A' && chr <= 'Z') || (chr >= 'a' && chr <= 'z')) {
    //    r = _findCharInAnyFont(chr ^ 0x20, outFont, outChr);
    //    if(r) return r;
    //}

    //fall back to placeholder character
    return _findCharInAnyFont('.', outFont, outChr);
    OSReport("Can't find char 0x%X (%c)", chr, chr);
    return 0;
}

/** Draw some text on the screen using the GameText fonts.
 *  @param str The string to draw.
 *  @param x X coord of top left pixel.
 *  @param y Y coord of top left pixel.
 *  @param outX Receives X coord that next character would draw at. (Can be NULL.)
 *  @param outY Receives Y coord that next character would draw at. (Can be NULL.)
 *  @param flags Flags:
 *    TEXT_FIXED: Render fixed-width text.
 *    TEXT_COLORED: Render colored, "soft-looking" text.
 *    TEXT_SHADOW: Render a drop shadow under the text.
 *  @param color Color, if enabled.
 *  @param scale Text scale.
 *  @return The number of bytes read from `str`.
 *  @note Control codes can be used in the string, prefixed by byte 0x1B (ESC):
 *    F: Toggle fixed width
 *    fN: Select font N
 *    cRRGGBBAA: Set text color (and enable color)
 *    C: Disable color
 *    S: Toggle shadow
 *  @note Rendering with color disabled is less pretty, but faster.
 */
int drawText(const char *str, int x, int y, int *outX, int *outY, u32 flags,
Color4b color, float scale) {
    //0: icons, text, none, none
    //1: none
    //2: error text, none, none, none
    //3: text, none, none, none
    //icons:
    //A: A button
    //B: B button
    //C: C button
    //D: German flag
    //F: French flag (reversed)
    //I: Italian flag
    //J: joystick
    //S: Spanish flag (or Start button, in French)
    //There don't seem to be icons for the English or Japanese flags in this version.
    //Not all icons are available at all times.
    int iFont = 3; //can be changed by control codes
    int startX = x, startY = y;
    int lineHeight = 0;
    int iChr = 0;
    Color4b shadowColor = {.r=0x20, .g=0x20, .b=0x20, .a=0x3F};
    bool japanese = (curLanguage == LANG_JAPANESE);

    while(true) {
        int cSize = 0;
        int chr = shiftJisGetNextChar((char*)&str[iChr], &cSize);
        if(!chr) break;
        iChr += cSize;

        if(chr < 0x20) {
            switch(chr) {
                case '\n': y += (flags & TEXT_FIXED) ? 16 :
                    MAX(lineHeight, japanese ? LINE_HEIGHT_JAPANESE : LINE_HEIGHT);
                    //fall thru
                case '\r': x = startX; break;
                case '\t': {
                    int p = x % (japanese ? TAB_WIDTH_JAPANESE : TAB_WIDTH);
                    x += ((japanese ? TAB_WIDTH_JAPANESE : TAB_WIDTH) - p);
                    break;
                }

                case 0x1B: { //escape
                    //note: not standard ANSI codes
                    char c = str[iChr++];
                    switch(c) {
                        case 'F': flags ^= TEXT_FIXED; break; //F: toggle fixed width
                        case 'f': iFont = str[iChr++] - 0x30; break; //fN: select font N
                        case 'c': { //cRRGGBBAA: set color
                            const char *s2 = &str[iChr];
                            color.r = _readHex(s2, 2, &s2);
                            color.g = _readHex(s2, 2, &s2);
                            color.b = _readHex(s2, 2, &s2);
                            color.a = _readHex(s2, 2, &s2);
                            iChr = s2 - str;
                            flags |= TEXT_COLORED;
                            break;
                        }
                        case 'C': flags &= ~TEXT_COLORED; break; //C: disable color
                        case 'S': flags ^= TEXT_SHADOW; break; //S: toggle shadow
                    }
                }
                default: break;
            }
        }
        else if(chr < 0xE000 || chr > 0xF8FF) { //normal character
            //get the character from the font
            GameTextFont *font = &gameTextFonts[iFont];
            GameTextCharacterStruct *cStruct = NULL;

            if(findChar(chr, font, &font, &cStruct)) {
                int cx = x, cy = y;
                if(flags & TEXT_FIXED && curLanguage != LANG_JAPANESE) { //correct some positions
                    switch(chr) {
                        case 'i': case 'I': case 'l': case ':': cx += 5; break;
                        case 'M': case 'W': cx -= 3; break;
                    }
                }

                if(flags & TEXT_SHADOW) { //render shadow
                    _drawCharColored(cStruct, font, cx, cy, shadowColor, scale * 1.2);
                }
                //render the actual character
                if(flags & TEXT_COLORED) {
                    _drawCharColored(cStruct, font, cx, cy, color, scale);
                }
                else _drawChar(cStruct, font, cx, cy, color.a, scale);

                //update the cursor position
                if(flags & TEXT_FIXED) x += (japanese ? FIXED_CHR_WIDTH_JAPANESE : FIXED_CHR_WIDTH);
                else x += cStruct->width + cStruct->right + cStruct->left;
                lineHeight = MAX(lineHeight, cStruct->height + cStruct->top + cStruct->bottom);
            }
            else if(flags & TEXT_FIXED) x += (japanese ? FIXED_CHR_WIDTH_JAPANESE : FIXED_CHR_WIDTH);
            else x += 8;
        }
        else {
            //TODO handle original game control codes
            /* SetScale	0xf8f4
            SetFont	0xf8f7
            LeftJustify	0xf8f8
            RightJustify	0xf8f9
            Center	0xf8fa
            FullJustify	0xf8fb
            SetColor	0xf8ff */
        }
    }
    if(outX) *outX = x;
    if(outY) *outY = y;
    return iChr;
}

int drawColorText(const char *str, int x, int y, Color4b color) {
    return drawText(str, x, y, NULL, NULL, TEXT_COLORED | TEXT_SHADOW, color, 1.0);
}

int drawSimpleText(const char *str, int x, int y) {
    Color4b color = {.r=192, .g=192, .b=192, .a=255};
    return drawText(str, x, y, NULL, NULL, TEXT_COLORED | TEXT_SHADOW, color, 1.0);
}