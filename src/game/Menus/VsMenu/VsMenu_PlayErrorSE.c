#include <nitro/types.h>

void SysSound_PlaySE(s32 a);

#pragma thumb on
void VsMenu_PlayErrorSE(void) {
    SysSound_PlaySE(6);
}
#pragma thumb reset
