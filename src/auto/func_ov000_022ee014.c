/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BossEffect_CreateClash();

void func_ov000_022ee014(s32 p0, s32 p1, s32 p2) {
    s32 t0 = BossEffect_CreateClash(1, p0, p1, p2);
    *(u8 *)((u8 *)t0 + 0x13e) = 0x13;
    *(u8 *)((u8 *)t0 + 0x13f) = 3;
    *(u8 *)((u8 *)t0 + 0x140) = 0;
    *(u32 *)((u8 *)t0 + 0x14c) = 0x1a66;
    *(u32 *)((u8 *)t0 + 0x150) = 0x1a66;
    *(u32 *)((u8 *)t0 + 0x154) = 0x1a66;
}
