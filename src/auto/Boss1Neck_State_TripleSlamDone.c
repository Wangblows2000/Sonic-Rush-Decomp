/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gBoss1[];
s32 ScreenShake_Start();

void Boss1Neck_State_TripleSlamDone(void) {
    u32 t0 = *(u32 *)gBoss1;
    u32 t1 = *(u32 *)((u8 *)t0 + 0x214);
    ScreenShake_Start(0, t0);
    u16 t2 = *(u16 *)((u8 *)(t1 + 0x200) + 0x16);
    *(u16 *)((u8 *)(t1 + 0x200) + 0x16) = (t2 & ~4);
    u16 t3 = *(u16 *)((u8 *)(t1 + 0x200) + 0x42);
    *(u16 *)((u8 *)(t1 + 0x200) + 0x42) = (t3 & ~4);
    u16 t4 = *(u16 *)((u8 *)(t1 + 0x200) + 0x9a);
    *(u16 *)((u8 *)(t1 + 0x200) + 0x9a) = (t4 & ~4);
}
