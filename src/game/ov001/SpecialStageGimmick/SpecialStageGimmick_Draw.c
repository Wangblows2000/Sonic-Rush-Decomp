#include <nitro/types.h>

void SpecialStageObjList_Draw(void *p);

void SpecialStageGimmick_Draw(void **p) {
    SpecialStageObjList_Draw(*p);
}
