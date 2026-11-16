#include <nitro/types.h>

BOOL func_ov001_02312138(void *p);
void SpecialStagePlayer_ChangeState(void *p, s32 v);

void func_ov001_02311afc(void *p) {
    if (!func_ov001_02312138(p)) {
        return;
    }
    SpecialStagePlayer_ChangeState(p, 5);
}
