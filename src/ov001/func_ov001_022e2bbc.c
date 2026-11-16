#include <nitro/types.h>

void NNS_G3dGeBufferOP_N(s32 a, void *p, s32 n);

#pragma thumb on
void func_ov001_022e2bbc(void) {
    u32 v = 0x20000000;
    NNS_G3dGeBufferOP_N(0x2a, &v, 1);
}
#pragma thumb reset
