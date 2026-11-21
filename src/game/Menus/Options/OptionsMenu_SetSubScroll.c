#include <nitro/types.h>

extern u16 gDisplaySub[];

#pragma thumb on
void OptionsMenu_SetSubScroll(void *unused, s32 x, s32 y) {
    if (x < -0x100) {
        x = -0x100;
    }
    if (x > 0x100) {
        x = 0x100;
    }
    gDisplaySub[0] = -x;
    gDisplaySub[1] = -y;
    gDisplaySub[2] = -x;
    gDisplaySub[3] = -y;
}
#pragma thumb reset
