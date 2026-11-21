#include <nitro/types.h>

void SpecialStageMap_SetScrollSpeed(s32 v);
void SpecialStageCamera_SetMode(s32 v);

void SpecialStagePlayer_EndDash(u8 *p) {
    SpecialStageMap_SetScrollSpeed(0x3000);
    *(s32 *)(p + 0x144) = 0;
    SpecialStageCamera_SetMode(0);
}
