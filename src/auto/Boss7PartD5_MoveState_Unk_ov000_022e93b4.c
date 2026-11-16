/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Boss7_Unk_ov000_022ee198();
s32 Boss7_Unk_ov000_022e9b70();
extern u8 data_ov000_02368ba2[];
s32 Sound_PlaySeqArc();
extern u8 gBossStatus[];
void Boss7PartD5_MoveState_Unk_ov000_022e9234();

void Boss7PartD5_MoveState_Unk_ov000_022e93b4(u8 *p0) {
    Boss7_Unk_ov000_022ee198(p0, 0x31);
    Boss7_Unk_ov000_022e9b70(p0, 1, 0);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x10);
    *(u16 *)((u8 *)p0 + 0x30) = -*(s16 *)((u8 *)data_ov000_02368ba2 + (*(u32 *)((u8 *)p0 + 0x3d0) * 0x30));
    if (*(u32 *)((u8 *)p0 + 0x3d0) == 0) {
        Sound_PlaySeqArc(0, 0, 2);
    } else {
        Sound_PlaySeqArc(0, 0, 0x13);
    }
    *(u32 *)((u8 *)gBossStatus + 0x4) = 0x28;
    *(u32 *)((u8 *)p0 + 0x440) = (u32)Boss7PartD5_MoveState_Unk_ov000_022e9234;
}
