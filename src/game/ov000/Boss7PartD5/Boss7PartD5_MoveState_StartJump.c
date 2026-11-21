/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Boss7_SetRivalAnimation();
s32 Boss7_SetCollidersEnabled();
extern u8 data_ov000_02368ba2[];
s32 Sound_PlaySeqArc();
extern u8 gBossStatus[];
void Boss7PartD5_MoveState_Jump();

void Boss7PartD5_MoveState_StartJump(u8 *p0) {
    Boss7_SetRivalAnimation(p0, 0x31);
    Boss7_SetCollidersEnabled(p0, 1, 0);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x10);
    *(u16 *)((u8 *)p0 + 0x30) = -*(s16 *)((u8 *)data_ov000_02368ba2 + (*(u32 *)((u8 *)p0 + 0x3d0) * 0x30));
    if (*(u32 *)((u8 *)p0 + 0x3d0) == 0) {
        Sound_PlaySeqArc(0, 0, 2);
    } else {
        Sound_PlaySeqArc(0, 0, 0x13);
    }
    *(u32 *)((u8 *)gBossStatus + 0x4) = 0x28;
    *(u32 *)((u8 *)p0 + 0x440) = (u32)Boss7PartD5_MoveState_Jump;
}
