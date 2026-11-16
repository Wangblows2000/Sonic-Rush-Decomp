/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Player_Action_Swing();

void Swing_OnHit(u8 *p0, u8 *p1) {
    u32 t0 = *(u32 *)((u8 *)p0 + 0x18);
    u32 t1 = *(u32 *)((u8 *)p1 + 0x18);
    if (t0 == 0) {
        return;
    }
    if (t1 == 0) {
        return;
    }
    s16 t2 = *(s16 *)((u8 *)t1 + 0x34);
    if (t2 == 0) {
        return;
    }
    u32 t3 = *(u32 *)((u8 *)t1 + 0x58);
    if ((t3 & 1) == 0) {
        return;
    }
    *(u32 *)((u8 *)t0 + 0x204) = t1;
    u32 t4 = *(u32 *)((u8 *)t0 + 0x54);
    *(u32 *)((u8 *)t0 + 0x54) = (t4 | 2);
    Player_Action_Swing(t1, t0, 0, 0x34);
}
