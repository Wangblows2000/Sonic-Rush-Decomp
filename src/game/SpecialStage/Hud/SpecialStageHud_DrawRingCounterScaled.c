#include <nitro/types.h>

extern u8 *gSpecialStageHud;
u8 *SpecialStage_GetPlayer(void);
void AnimatorSprite_DrawRotScale(void *o, s32 a, s32 b, s32 c);

void SpecialStageHud_DrawRingCounterScaled(s32 t) {
    u8 *g = gSpecialStageHud;
    s32 v = *(s32 *)(SpecialStage_GetPlayer() + 0x148);
    s32 s = 0x1000;

    if (t >= 0 && t <= 0x14)
        s += 1228.8f * (0x14 - t) / 20.0f;
    else if (t > 0x14 && t <= 0x28)
        s += 1228.8f * (0x28 - t) / 20.0f;
    else if (t > 0x28 && t <= 0x3c)
        s += 1228.8f * (0x3c - t) / 20.0f;
    if (v >= 100) {
        AnimatorSprite_DrawRotScale(g + 0x198, s, s, 0);
        AnimatorSprite_DrawRotScale(g + 0x1e0, s, s, 0);
        AnimatorSprite_DrawRotScale(g + 0x228, s, s, 0);
    } else if (v >= 10) {
        AnimatorSprite_DrawRotScale(g + 0x1e0, s, s, 0);
        AnimatorSprite_DrawRotScale(g + 0x228, s, s, 0);
    } else {
        AnimatorSprite_DrawRotScale(g + 0x228, s, s, 0);
    }
}
