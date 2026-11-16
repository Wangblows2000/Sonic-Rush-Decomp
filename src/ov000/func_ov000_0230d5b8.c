#include <nitro/types.h>

extern s32 data_ov000_0236a018;

void StageObj_Destructor(void *p);

void func_ov000_0230d5b8(void *p) {
    data_ov000_0236a018 = 0;
    StageObj_Destructor(p);
}
