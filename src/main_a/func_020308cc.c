#include <nitro/types.h>

typedef struct { u32 m[9]; } UnkMtx_020308cc;
extern u8 data_02084d84[];
void func_02038328(void *src, UnkMtx_020308cc *dst);
void NNS_G3dGeBufferOP_N(u32 a, void *b, u32 c);

void func_020308cc(void) {
    UnkMtx_020308cc m;
    func_02038328(data_02084d84, &m);
    NNS_G3dGeBufferOP_N(0x1A, &m, 9);
}
