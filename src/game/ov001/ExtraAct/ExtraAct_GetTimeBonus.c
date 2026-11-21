#include <nitro/types.h>

extern u32 gExtraActTimeBonusThresholds[10];
extern s32 gExtraActTimeBonusTable[10];

s32 ExtraAct_GetTimeBonus(u32 v) {
    s32 i;
    s32 r;

    i = 0;
    r = 0;
    for (; i < 10; i++) {
        if (gExtraActTimeBonusThresholds[i] >= v) {
            r = gExtraActTimeBonusTable[i];
            break;
        }
    }
    return r;
}
