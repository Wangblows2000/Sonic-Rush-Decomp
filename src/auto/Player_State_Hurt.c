/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Player_SettleAirRotation();
s32 Player_Action_LandOnGround();

void Player_State_Hurt(u8 *p0) {
    Player_SettleAirRotation();
    if ((*(u32 *)((u8 *)p0 + 0x54) & 2) != 0) {
        return;
    }
    if ((*(u32 *)((u8 *)p0 + 0x58) & 1) == 0) {
        return;
    }
    s32 t0 = (*(u16 *)((u8 *)p0 + 0x2aa) & ~0x100);
    *(u16 *)((u8 *)p0 + 0x2aa) = t0;
    Player_Action_LandOnGround(p0, 0, ((u8 *)p0 + 0x200), t0);
    ((s32 (*)())*(u32 *)((u8 *)p0 + 0x334))(p0, *(u32 *)((u8 *)p0 + 0x334));
}
