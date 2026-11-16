#include <nitro/types.h>

typedef struct UnkStruct_02315714_m {
    s32 m[16];
} UnkStruct_02315714_m;

void G3i_OrthoW_(s32 t, s32 b, s32 l, s32 r, s32 n, s32 f, s32 s, s32 z, UnkStruct_02315714_m *m);
void NNS_G3dGeBufferOP_N(s32 a, void *b, s32 c);

void func_ov001_02315714(void) {
    s32 a;
    s32 b;
    UnkStruct_02315714_m m;

    G3i_OrthoW_(0, 0xc00, 0, 0x1000, -0x1000, 0x1000, 0x1000, 0, &m);
    a = 0;
    NNS_G3dGeBufferOP_N(0x10, &a, 1);
    NNS_G3dGeBufferOP_N(0x11, 0, 0);
    NNS_G3dGeBufferOP_N(0x15, 0, 0);
    NNS_G3dGeBufferOP_N(0x16, &m, 0x10);
    b = 2;
    NNS_G3dGeBufferOP_N(0x10, &b, 1);
    NNS_G3dGeBufferOP_N(0x11, 0, 0);
    NNS_G3dGeBufferOP_N(0x15, 0, 0);
}
