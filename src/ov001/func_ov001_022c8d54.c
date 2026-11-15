#include <nitro/types.h>

s32 func_02050c60(void *p);

#pragma thumb on
s32 func_ov001_022c8d54(u8 *p) {
    if (*(s32 *)(p + 0x10) == 0) {
        return func_02050c60(p);
    }
    return *(s32 *)(p + 8);
}
#pragma thumb reset
