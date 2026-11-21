/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_0207c3a0[];
extern u8 data_020766a4[];
extern u8 data_02074960[];
extern u8 data_020761b4[];

s32 PlayerDemoLight_GetColors(s32 p0, s32 p1) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)((u8 *)data_0207c3a0 + ((s32)p0 << 2)) + ((s32)p1 << 3));
    if (t0 == 0) {
        return (((s32)data_02074960 + (0x12 * *(s8 *)((u8 *)((s32)data_020766a4 + (0xc * (s32)p0)) + 8))) + (6 * *(s8 *)((u8 *)((s32)data_020766a4 + (0xc * (s32)p0)) + 9)));
    }
    return ((s32)data_020761b4 + (6 * t0));
}

#pragma thumb reset
