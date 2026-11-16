#include <nitro/types.h>

extern s32 data_ov000_02369f70;

void StageObj_Destructor(void *p);

void func_ov000_0230bdf4(void *p) {
    data_ov000_02369f70 = 0;
    StageObj_Destructor(p);
}
