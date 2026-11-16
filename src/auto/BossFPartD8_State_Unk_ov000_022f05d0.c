/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BossFPartD8_SetAnimation();
s32 Sound_PlaySeqArc();
s32 Sound_PlayVoice();
void BossFPartD8_State_Unk_ov000_022f052c();

void BossFPartD8_State_Unk_ov000_022f05d0(u8 *p0) {
    BossFPartD8_SetAnimation(p0, 0x21, 0x1000, 1);
    Sound_PlaySeqArc(0, 0, 0xc7);
    Sound_PlayVoice(0, 0, 0xd0);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)BossFPartD8_State_Unk_ov000_022f052c;
}
