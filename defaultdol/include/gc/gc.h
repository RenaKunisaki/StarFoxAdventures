#include <stdint.h>
#include <stdbool.h>
#define __SN__ //Dolphin sdk seems to expect this
//note also we use a modified SDK.
//it has `AT_ADDRESS` redefined to do nothing, and the three uses of it
//have `extern` added. this was necessary to build with gcc.
#include <gc/dolphin.h>
#include "types.h"
#include "macros.h"

extern u32 consoleType; //0x8000002C;
extern u32 simMemSize;  //0x800000F0;
extern u32 realMemSize; //0x80000028;
extern u32 aramSize;    //0x800000D0;
