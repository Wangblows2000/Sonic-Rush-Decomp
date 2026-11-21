/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Player_Action_LandOnGround();
s32 Player_ApplyGroundInput();
s32 Player_ApplyAirInput();
s32 Player_Action_Fall();

void Player_State_BreatheAirBubble(u8 *p0) {
    *(u16 *)((u8 *)p0 + 0x3ca) = 0;
    *(u32 *)((u8 *)p0 + 0xc) = (*(u32 *)((u8 *)p0 + 0xc) - 0x1);
    s32 t0 = *(u32 *)((u8 *)p0 + 0xc);
    if (t0 == 0) {
        *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x2000);
    }
    s32 t1 = *(u32 *)((u8 *)p0 + 0x58);
    if ((*(u32 *)((u8 *)p0 + 0x58) & 0x2000) == 0) {
        if ((t1 & 1) != 0) {
            if ((t1 & 0x10) != 0) {
                Player_Action_LandOnGround(p0, 0);
                *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x10);
            }
            Player_ApplyGroundInput(p0);
        } else {
            Player_ApplyAirInput(p0);
        }
    }
    if ((*(u16 *)((u8 *)p0 + 0x5c) & 8) == 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x2000);
    if ((*(u32 *)((u8 *)p0 + 0x58) & 1) != 0) {
        ((s32 (*)())*(u32 *)((u8 *)p0 + 0x334))(p0, *(u32 *)((u8 *)p0 + 0x334));
        return;
    }
    s32 t2 = (*(u16 *)((u8 *)p0 + 0x5c) | 0x400);
    *(u16 *)((u8 *)p0 + 0x5c) = t2;
    Player_Action_Fall(p0, t2);
}
