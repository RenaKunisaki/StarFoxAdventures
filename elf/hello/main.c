//#include <stdint.h>
//#include <gccore.h>
#include "../include/sfa/sfa.h"
#ifdef __cplusplus
extern "C" {
#endif

void _start(void) {
    for(int i=0; i<5; i++) {
    	OSReport("Hello World! %d\n", i);
    }
}

#ifdef __cplusplus
}; //extern "C"
#endif
