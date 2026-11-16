/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022c4638[];
s32 MIi_CpuCopy16();

s32 func_02051150(s32 p0, s32 p1) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022c4638 + 0x10);
    s32 v1 = *(u32 *)data_022c4638;
    if (*(u32 *)data_022c4638 == 0) {
        v1 = 7;
    } else {
        v1 = *(u32 *)((u8 *)t0 + 0xc);
    }
    if (v1 < 3) {
        return 0;
    }
    s32 v2 = (s32)p1;
    if ((s32)p1 > 0xa) {
        v2 = 0xa;
    }
    MIi_CpuCopy16((t0 + 0x36), p0, (v2 << 1));
    s32 t3 = *(u16 *)(t0 + 0x4a);
    s32 v4 = t3;
    if (v2 < t3) {
        v4 = v2;
    }
    return v4;
}

#pragma thumb reset
