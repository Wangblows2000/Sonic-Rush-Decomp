/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 WirelessLobby_GetResult();
s32 OptionsWfsHost_HasError();

s32 OptionsMenu_HasWirelessError(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    if (t0 != 0) {
        s32 t1 = WirelessLobby_GetResult(t0);
        if (t1 != 0) {
            return 1;
        }
    }
    s32 t2 = *(u32 *)((u8 *)p0 + 0x14);
    if (t2 != 0) {
        s32 t3 = OptionsWfsHost_HasError(t2);
        if (t3 != 0) {
            return 1;
        }
    }
    return 0;
}

#pragma thumb reset
