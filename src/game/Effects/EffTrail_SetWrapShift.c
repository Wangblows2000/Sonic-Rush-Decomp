#include <nitro/types.h>

extern s32 gEffTrailWrapShift;
extern u8 gEffTrailWrapPending;

void EffTrail_SetWrapShift(s32 v) {
    gEffTrailWrapShift = v;
    gEffTrailWrapPending = 1;
}
