/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022c4560[];
extern u8 data_022c4590[];
s32 func_ov000_0230ad04();
s32 func_ov000_0230b134();
s32 func_0201c070();
s32 Sound_PlaySeqArcEx();

void GoalRing_OnHit(u8 *p0, u8 *p1) {
    u32 t0 = *(u32 *)((u8 *)p0 + 0x18);
    u32 t1 = *(u32 *)((u8 *)t0 + 0x54);
    if ((t1 & 2) != 0) {
        u32 t2 = *(u32 *)((u8 *)data_022c4560 + 0x10);
        if (t2 == 1) {
            *(u32 *)((u8 *)data_022c4590 + 0x40) = 3;
        }
        return;
    }
    u32 t3 = *(u32 *)((u8 *)p1 + 0x18);
    if (t3 != 0) {
        func_ov000_0230ad04(t0, t3);
        s16 t4 = *(s16 *)((u8 *)t3 + 0x28);
        func_ov000_0230b134(t0, t4);
        u32 t5 = *(u32 *)((u8 *)t0 + 0x1dc);
        func_0201c070(t5, 0);
        u32 t6 = *(u32 *)((u8 *)t0 + 0x1dc);
        Sound_PlaySeqArcEx(t6, 0, 0x45);
        u16 t7 = *(u16 *)((u8 *)(t0 + 0x100) + 0x3e);
        *(u16 *)((u8 *)(t0 + 0x100) + 0x3e) = (t7 | 0x1000);
    }
    u32 t8 = *(u32 *)((u8 *)t0 + 0x54);
    *(u32 *)((u8 *)t0 + 0x54) = (t8 | 2);
}
