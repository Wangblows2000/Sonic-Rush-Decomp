/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BossFPartD8_SetAnimation();
s32 Sound_PlayVoice();
void BossFPartD8_State_PartDCVolleyRaise();

void BossFPartD8_State_PartDCVolleyStart(u8 *p0) {
    BossFPartD8_SetAnimation(p0, 0x26, 0x1000, 0);
    Sound_PlayVoice(0, 0, 0xd0);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)BossFPartD8_State_PartDCVolleyRaise;
}
