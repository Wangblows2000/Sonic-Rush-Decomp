/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Player_Action_Parachute();

void Parachute_OnHit(u8 *p0, u8 *p1) {
    u32 t0 = *(u32 *)((u8 *)p0 + 0x18);
    u32 t1 = *(u32 *)((u8 *)p1 + 0x18);
    if (t0 == 0) {
        return;
    }
    if (t1 == 0) {
        u8 t2 = *(u8 *)t1;
        if (t2 != 1) {
            return;
        }
    }
    u32 t3 = *(u32 *)((u8 *)t0 + 0x1e0);
    s8 t4 = *(s8 *)((u8 *)t3 + 0x6);
    Player_Action_Parachute(t1, t0, t4, t0);
}
