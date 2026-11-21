/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gSpecialStageHud[];
s32 AnimatorSprite_Draw();

void SpecialStageHud_DrawDigits(void) {
    s32 t0 = *(u32 *)gSpecialStageHud;
    AnimatorSprite_Draw((*(u32 *)gSpecialStageHud + 0x270));
    AnimatorSprite_Draw((t0 + 0x2b8));
    AnimatorSprite_Draw((t0 + 0x300));
}
