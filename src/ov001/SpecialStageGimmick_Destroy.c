#include <nitro/types.h>

void SpecialStageObj_MarkDestroyed(void *p);

void SpecialStageGimmick_Destroy(void *p) {
    SpecialStageObj_MarkDestroyed(p);
}
