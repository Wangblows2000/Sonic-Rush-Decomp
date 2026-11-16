/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_0236aad8[];
s32 func_ov000_0233c37c();

s32 func_ov000_023422e4(s32 p0) {
    s32 t0 = (*(u32 *)((u8 *)data_ov000_0236aad8 + 0xc8) & 1);
    if (t0 != 0) {
        return (s32)p0;
    }
    s32 t1 = func_ov000_0233c37c(p0, t0);
    s32 v2 = (t1 & 1);
    if ((t1 & 1) != 0) {
        *(u32 *)((u8 *)data_ov000_0236aad8 + 0xc8) = (*(u32 *)((u8 *)data_ov000_0236aad8 + 0xc8) | 0x800);
        v2 = (s32)data_ov000_0236aad8;
    }
    return v2;
}
