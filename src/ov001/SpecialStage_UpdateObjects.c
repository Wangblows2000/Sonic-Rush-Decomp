#include <nitro/types.h>

extern u8 *data_ov001_0231f5b8;
void SpecialStageObjList_Update(void *p);

void SpecialStage_UpdateObjects(void) {
    SpecialStageObjList_Update(*(void **)(data_ov001_0231f5b8 + 0x46a0));
}
