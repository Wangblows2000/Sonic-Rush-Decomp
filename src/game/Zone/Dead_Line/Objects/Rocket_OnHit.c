/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Sound_PlaySeqArcEx();
s32 Rocket_Action_Launch();
s32 Player_Action_Rocket();

void Rocket_OnHit(u8 *p0, u8 *p1) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x18);
    s32 t1 = *(u32 *)((u8 *)p1 + 0x18);
    if (t0 == 0) {
        return;
    }
    if (t1 == 0) {
        return;
    }
    if (*(u8 *)t1 != 1) {
        return;
    }
    Sound_PlaySeqArcEx(0, 0, 0x6d);
    Rocket_Action_Launch(t0, t1);
    if ((*(u16 *)((u8 *)*(u32 *)((u8 *)t0 + 0x1e0) + 0x4) & 1) != 0) {
        Player_Action_Rocket(t1, t0, 0xc, 0);
        return;
    }
    Player_Action_Rocket(t1, t0, -12, 0);
}
