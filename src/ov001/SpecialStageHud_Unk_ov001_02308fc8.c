#include <nitro/types.h>

extern u8 *gSpecialStageHud;
void AnimatorSprite_Draw(void *p);
void SpecialStageHud_DrawRingCounterScaled(s32 a);
void SpecialStageHud_Unk_ov001_023081d8(s32 a);

void SpecialStageHud_Unk_ov001_02308fc8(void) {
    u8 *g = gSpecialStageHud;
    if (*(s32 *)(g + 0xbc8) >= 1 && *(s32 *)(g + 0xbc8) < 0x78) {
        AnimatorSprite_Draw(g + 0x7c8);
    } else if (*(s32 *)(g + 0xbc8) >= 0xf0 && *(s32 *)(g + 0xbc8) < 0x168) {
        AnimatorSprite_Draw(g + 0x780);
    }
    SpecialStageHud_DrawRingCounterScaled(*(s32 *)(g + 0xbc8) - 1);
    SpecialStageHud_Unk_ov001_023081d8(*(s32 *)(g + 0xbc8) - 0xb4);
}
