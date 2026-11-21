#include <nitro/types.h>

void SysSound_PlaySE(s32 a);

#pragma thumb on
void VsMenu_PlayDecideSE(void) {
    SysSound_PlaySE(0);
}
#pragma thumb reset
