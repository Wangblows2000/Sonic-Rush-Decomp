/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Sound_PlaySeqArcEx();
s32 Player_SetAnimation();
void Player_State_BreatheAirBubble();

void Player_Action_BreatheAirBubble(u8 *p0) {
    if (*(u32 *)((u8 *)p0 + 0x3e8) != 0) {
        *(u16 *)((u8 *)p0 + 0x3ca) = 0;
        Sound_PlaySeqArcEx(((u8 *)p0 + 0x384), 0, 0x53, ((u8 *)p0 + 0x300));
        return;
    }
    Sound_PlaySeqArcEx(((u8 *)p0 + 0x384), 0, 0x53);
    Player_SetAnimation(p0, 0x23);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)Player_State_BreatheAirBubble;
    *(u16 *)((u8 *)p0 + 0x30) = 0;
    *(u16 *)((u8 *)p0 + 0x3ca) = 0;
    *(u32 *)((u8 *)p0 + 0xc) = 4;
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x2000);
}
