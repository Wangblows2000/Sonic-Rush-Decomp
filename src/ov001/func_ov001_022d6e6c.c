#include <nitro/types.h>

extern s32 gGameState[];
void func_02063688(void);

#pragma thumb on
void func_ov001_022d6e6c(void) {
    gGameState[4] = 1;
    gGameState[6] |= 0x20;
    gGameState[6] &= ~0x10;
    func_02063688();
}
#pragma thumb reset
