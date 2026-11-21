#include <nitro/types.h>

extern u8 *gSpecialStageHud;
u8 *SpecialStage_GetPlayer(void);
void AnimatorSprite_Draw(void *p);

void SpecialStageHud_DrawRingCounter(void) {
    u8 *g = gSpecialStageHud;
    s32 v = *(s32 *)(SpecialStage_GetPlayer() + 0x148);
    if (v >= 100) {
        AnimatorSprite_Draw(g + 0x198);
        AnimatorSprite_Draw(g + 0x1e0);
        AnimatorSprite_Draw(g + 0x228);
    } else if (v >= 10) {
        AnimatorSprite_Draw(g + 0x1e0);
        AnimatorSprite_Draw(g + 0x228);
    } else {
        AnimatorSprite_Draw(g + 0x228);
    }
}
