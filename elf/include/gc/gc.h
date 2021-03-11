#include <stdint.h>
#include <stdbool.h>
#define __SN__ //Dolphin sdk seems to expect this
//note also we use a modified SDK.
//it has `AT_ADDRESS` redefined to do nothing, and the three uses of it
//have `extern` added. this was necessary to build with gcc.
#include <gc/dolphin.h>
#include "types.h"
#include "macros.h"
