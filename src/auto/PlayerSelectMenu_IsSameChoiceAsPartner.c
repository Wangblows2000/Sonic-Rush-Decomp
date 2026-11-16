/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_020514e4();
s32 func_02051474();

s32 PlayerSelectMenu_IsSameChoiceAsPartner(void) {
    s32 t0 = func_020514e4();
    s32 t1 = func_02051474();
    if (*(u8 *)((u8 *)t0 + 0x1) == 0) {
        return 0;
    }
    if (*(u8 *)((u8 *)t1 + 0x1) == 0) {
        return 0;
    }
    if (*(u8 *)t0 == *(u8 *)t1) {
        return 1;
    }
    return 0;
}

#pragma thumb reset
