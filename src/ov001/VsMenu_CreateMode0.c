#include <nitro/types.h>

void VsMenu_Create(s32 a, s32 b, s32 c);

#pragma thumb on
void VsMenu_CreateMode0(s32 b, s32 c) {
    VsMenu_Create(0, b, c);
}
#pragma thumb reset
