#include <nitro/types.h>

void SysSound_PlaySE(s32 a);

#pragma thumb on
void func_ov001_022d96c4(void) {
    SysSound_PlaySE(6);
}
#pragma thumb reset
