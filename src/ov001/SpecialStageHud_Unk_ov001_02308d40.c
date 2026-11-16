#include <nitro/types.h>

extern u8 *gSpecialStageHud;
void AnimatorSprite_Draw(void *p);

void SpecialStageHud_Unk_ov001_02308d40(void) {
    AnimatorSprite_Draw(gSpecialStageHud + 0x7c8);
}
