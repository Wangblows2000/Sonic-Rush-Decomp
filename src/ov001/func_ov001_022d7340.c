#include <nitro/types.h>

extern void *data_ov001_0231f054;

#pragma thumb on
void func_ov001_022d7340(void *p) {
    *(vu16 *)0x0400000e = (*(vu16 *)0x0400000e & 0x43) | 0x390;
    data_ov001_0231f054 = p;
}
#pragma thumb reset
