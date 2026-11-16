#include <nitro/types.h>

extern s32 data_ov000_0236a98c;

void StageObj_Destructor(void *p);

void func_ov000_0232c794(void *p) {
    data_ov000_0236a98c = 0;
    StageObj_Destructor(p);
}
