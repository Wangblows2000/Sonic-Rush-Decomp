#include <nitro/types.h>

extern u8 *gSpecialStageHud;
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);

void SpecialStageGimmick_Unk_ov001_023099c4(void) {
    *(s32 *)(gSpecialStageHud + 0xbc8) = 0;
    Sound_PlaySeqArc(0, 0x10, 1);
}
