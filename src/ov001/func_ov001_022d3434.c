#include <nitro/types.h>

void SysSound_Release(void);
void func_02052424(s32 a, s32 b);

#pragma thumb on
void func_ov001_022d3434(void) {
    SysSound_Release();
    func_02052424(-1, -1);
}
#pragma thumb reset
