#include <nitro/types.h>

extern u8 *gSpecialStageHud;

void SpecialStageHud_EndTrickRating(void) {
    *(s32 *)(gSpecialStageHud + 0xaf0) = 5;
}
