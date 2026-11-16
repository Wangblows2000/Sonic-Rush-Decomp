/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Player_Action_Wheel2D();

void Wheel2D_OnHit(u8 *p0, u8 *p1) {
    u32 t0 = *(u32 *)((u8 *)p0 + 0x18);
    u32 t1 = *(u32 *)((u8 *)p1 + 0x18);
    if (t0 == 0) {
        return;
    }
    if (t1 == 0) {
        return;
    }
    *(u32 *)((u8 *)t0 + 0x204) = t1;
    Player_Action_Wheel2D(t1, t0, t0);
}
