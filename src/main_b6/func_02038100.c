#include <nitro/types.h>

typedef struct {
    u8 pad0[0xfc];
    u32 flags;
} UnkG3_02038100;

extern u32 data_02084d3c[];
extern u32 data_02084d84[];
extern u32 data_02084df4[];
extern UnkG3_02038100 NNS_G3dGlb;

void NNS_G3dGeBufferOP_N(u32 op, const u32 *args, u32 n);

void func_02038100(void) {
    u32 v;

    NNS_G3dGeBufferOP_N(0x1610, data_02084d3c, 0x11);
    NNS_G3dGeBufferOP_N(0x19, data_02084d84, 0xc);
    NNS_G3dGeBufferOP_N(0x1b19, data_02084df4, 0xf);
    v = 2;
    NNS_G3dGeBufferOP_N(0x10, &v, 1);
    NNS_G3dGeBufferOP_N(0x15, NULL, 0);
    NNS_G3dGlb.flags |= 1;
    NNS_G3dGlb.flags &= ~2;
}
