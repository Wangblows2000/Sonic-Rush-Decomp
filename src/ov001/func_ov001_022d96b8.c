#include <nitro/types.h>

void SysSound_PlaySE(s32 a);

#pragma thumb on
void func_ov001_022d96b8(void) {
    SysSound_PlaySE(0);
}
#pragma thumb reset
