/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022c4638[];
s32 Wireless_GetLinkLevel();

s32 func_02051814(void) {
    s32 t0 = *(u32 *)data_022c4638;
    if (t0 == 0) {
        return 0;
    }
    s32 t1 = Wireless_GetLinkLevel(t0);
    return t1;
}

#pragma thumb reset
