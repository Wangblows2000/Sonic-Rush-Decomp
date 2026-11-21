#include <nitro/types.h>

s32 SpecialStageMap_GetScrollSpeed(void);
s32 _ffix(u32 f);
void SpecialStageMap_SetScrollSpeed(s32 v);

void SpecialStagePlayer_SlowDownMap(void) {
    s32 v = SpecialStageMap_GetScrollSpeed();
    v -= _ffix(0x4336febc);
    if (v < 0) {
        v = 0;
    }
    SpecialStageMap_SetScrollSpeed(v);
}
