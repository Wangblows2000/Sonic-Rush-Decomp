#include <nitro/types.h>

void NNS_G3dGeBufferOP_N(s32 a, void *b, s32 c);

void func_ov001_023156a8(void) {
    s32 a;
    s32 b;
    s32 c;
    s32 d;

    a = 0;
    NNS_G3dGeBufferOP_N(0x10, &a, 1);
    b = 1;
    NNS_G3dGeBufferOP_N(0x12, &b, 1);
    c = 2;
    NNS_G3dGeBufferOP_N(0x10, &c, 1);
    d = 1;
    NNS_G3dGeBufferOP_N(0x12, &d, 1);
}
