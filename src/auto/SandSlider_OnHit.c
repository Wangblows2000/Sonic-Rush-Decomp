/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Player_Action_SandSlider90();
s32 Player_Action_SandSlider();

void SandSlider_OnHit(u8 *p0, u8 *p1) {
    u32 t0 = *(u32 *)((u8 *)p0 + 0x18);
    u32 t1 = *(u32 *)((u8 *)p1 + 0x18);
    if (t0 == 0) {
        return;
    }
    if (t1 == 0) {
        return;
    }
    u32 t2 = *(u32 *)((u8 *)t0 + 0x1e0);
    u16 t3 = *(u16 *)((u8 *)t2 + 0x2);
    if (t3 == 0x90) {
        Player_Action_SandSlider90(t1, t0, t0);
        return;
    }
    Player_Action_SandSlider(t1, t0);
}
