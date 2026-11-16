#include <nitro/types.h>

void SpecialStageObj_MarkDestroyed(void *p);

void SpecialStageActObj_Destroy(void *p) {
    SpecialStageObj_MarkDestroyed(p);
}
