#include <nitro/types.h>

extern void *data_ov001_0231f054;

#pragma thumb on
void func_ov001_022d731c(void) {
    *(vu16 *)0x0400000e = (*(vu16 *)0x0400000e & 0x43) | 0x288;
    data_ov001_0231f054 = NULL;
}
#pragma thumb reset
