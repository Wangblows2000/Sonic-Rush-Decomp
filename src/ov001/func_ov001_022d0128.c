#include <nitro/types.h>

extern u8 *data_ov001_0231f050;
void func_020371c8(void *p);

#pragma thumb on
void func_ov001_022d0128(s32 i) {
    if (((void **)(data_ov001_0231f050 + 0xbc8))[i] != NULL) {
        func_020371c8(((void **)(data_ov001_0231f050 + 0xbc8))[i]);
        ((void **)(data_ov001_0231f050 + 0xbc8))[i] = NULL;
    }
}
#pragma thumb reset
