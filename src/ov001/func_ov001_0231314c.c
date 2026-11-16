#include <nitro/types.h>

void func_ov001_02312aac(void *p);
void SpecialStagePlayer_ChangeState(void *p, s32 v);

void func_ov001_0231314c(void *p) {
    func_ov001_02312aac(p);
    SpecialStagePlayer_ChangeState(p, 0x13);
}
