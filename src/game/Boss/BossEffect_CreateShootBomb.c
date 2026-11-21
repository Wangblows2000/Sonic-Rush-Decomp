/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_02367d88[];
s32 BossEffect_Create();

void BossEffect_CreateShootBomb(s32 p0, s32 p1, s32 p2, s32 p3) {
    s32 t0 = BossEffect_Create(data_ov000_02367d88, 7, p0, p1, p2, p3);
    *(u8 *)((u8 *)t0 + 0x13e) = 0x13;
    *(u8 *)((u8 *)t0 + 0x13f) = 3;
    *(u8 *)((u8 *)t0 + 0x140) = 0;
}
