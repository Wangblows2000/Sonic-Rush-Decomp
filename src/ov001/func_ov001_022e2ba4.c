#include <nitro/types.h>

#pragma thumb on
void func_ov001_022e2ba4(BOOL on) {
    *(vu16 *)0x04000050 = on ? 0x1841 : 0;
}
#pragma thumb reset
