#include <nitro/types.h>

void func_02059b74(s32 v);

#pragma thumb on
void Options_SetTimeLimit(u8 *p, s32 v) {
    if (*(s32 *)(p + 0xc) != v) {
        *(s32 *)(p + 0xc) = v;
        func_02059b74(v);
    }
}
#pragma thumb reset
