/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BossFPartD8_SetAnimation();
s32 Sound_PlaySeqArc();
s32 Sound_PlayVoice();
void BossFPartD8_State_SkyCharge1();

void BossFPartD8_State_SkyChargeStart1(u8 *p0) {
    BossFPartD8_SetAnimation(p0, 0x21, 0x1000, 1);
    Sound_PlaySeqArc(0, 0, 0xc7);
    Sound_PlayVoice(0, 0, 0xd1);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)BossFPartD8_State_SkyCharge1;
}
