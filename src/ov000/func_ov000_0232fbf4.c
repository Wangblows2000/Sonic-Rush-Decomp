#include <nitro/types.h>

extern s32 data_ov000_0236a984;

void StageObj_Destructor(void *p);

void func_ov000_0232fbf4(void *p) {
    data_ov000_0236a984 = 0;
    StageObj_Destructor(p);
}
