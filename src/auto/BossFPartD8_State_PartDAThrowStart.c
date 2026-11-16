/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BossFPartD8_SetAnimation();
s32 Sound_PlaySeqArc();
s32 Sound_PlayVoice();
void BossFPartD8_State_PartDAThrowStartWait();

void BossFPartD8_State_PartDAThrowStart(u8 *p0) {
    BossFPartD8_SetAnimation(p0, 0x15, 0x1000, 0);
    Sound_PlaySeqArc(0, 0, 0xc4);
    Sound_PlayVoice(0, 0, 0xd2);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)BossFPartD8_State_PartDAThrowStartWait;
}
