#define DPRINT_COLOR   "\x81" //followed by u8 r, g, b, a
#define DPRINT_POS     "\x82" //followed by s16 x, y (low byte first)
#define DPRINT_NOFIXED "\x83" //leave fixed-width mode
#define DPRINT_FIXED   "\x84" //enter fixed-width mode
#define DPRINT_BGCOLOR "\x85" //followed by u8 r, g, b, a
