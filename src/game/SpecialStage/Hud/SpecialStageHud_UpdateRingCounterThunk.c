#include <nitro/types.h>

void SpecialStageHud_UpdateRingCounter(void);

void SpecialStageHud_UpdateRingCounterThunk(void) {
    SpecialStageHud_UpdateRingCounter();
}
