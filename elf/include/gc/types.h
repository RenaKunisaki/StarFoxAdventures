//#include <gccore.h>

//#define BOOL  int //used by game code
//#define bool  int8_t
//#define true  1
//#define false 0
//#define s8   int8_t
//#define s16  int16_t
//#define s32  int32_t
//#define s64  int64_t
//#define u8  uint8_t
//#define u16 uint16_t
//#define u32 uint32_t
//#define u64 uint64_t
//#define f32 float
//#define f64 double
//#define uchar  unsigned char
//#define ushort unsigned short
//#define uint   unsigned int

typedef s16 DataFileEnum16;
typedef s32 DataFileEnum32;
typedef s16 DLL_ID16;
typedef s32 DLL_ID32;
typedef s16 GameBit16;
typedef s32 GameBit32;
typedef u16 GamepadButton;
typedef u32 GamepadButton32;
typedef s16 GameTextId;
typedef s16 GameTextId16;
typedef s32 GameTextId32;
typedef s8  GameTextWindow8;
typedef s16 GameTextWindow16;
typedef s32 GameTextWindow32;
typedef s8  MapDirIdx8;
typedef s16 MapDirIdx16;
typedef s32 MapDirIdx32;
typedef s8  MapId8;
typedef s16 MapId16;
typedef s32 MapId32;
typedef s32 ObjUniqueId;
typedef s32 PlayerStateEnum32;

//for ghidra exports
typedef void* pointer;
typedef u32 uint;
typedef u16 ushort;
typedef u8 uchar;
typedef u8 ubyte;
typedef int dword;
typedef short word;
typedef s8 byte;
typedef s8 undefined;
typedef s8 undefined1;
typedef s16 undefined2;
typedef s32 undefined4;
typedef double undefined8; //could be s64 but never is
