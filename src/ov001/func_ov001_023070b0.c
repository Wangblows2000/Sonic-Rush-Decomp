#include <nitro/types.h>

extern u8 *data_ov001_0231f5b8;
void SpecialStageObjList_Draw(void *p);

void func_ov001_023070b0(void) {
    SpecialStageObjList_Draw(*(void **)(data_ov001_0231f5b8 + 0x46a0));
}
