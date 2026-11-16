#include <nitro/types.h>

extern s32 gGameState[];
void func_020636ec(void);

#pragma thumb on
void func_ov001_022d6e8c(void) {
    gGameState[4] = 1;
    gGameState[6] |= 0x20;
    gGameState[6] &= ~0x10;
    func_020636ec();
}
#pragma thumb reset
