#include <nitro/types.h>

typedef struct {
    u8 pad0[0xfc];
    u32 flags;
} UnkG3_0203819c;

extern u32 data_02084d3c[];
extern u32 data_02084d84[];
extern u32 data_02084df4[];
extern UnkG3_0203819c NNS_G3dGlb;

void NNS_G3dGeBufferOP_N(u32 op, const u32 *args, u32 n);

void Model_GlbFlushVP(void) {
    u32 v;

    NNS_G3dGeBufferOP_N(0x1610, data_02084d3c, 0x11);
    NNS_G3dGeBufferOP_N(0x19, data_02084d84, 0xc);
    v = 2;
    NNS_G3dGeBufferOP_N(0x10, &v, 1);
    NNS_G3dGeBufferOP_N(0x1b17, data_02084df4, 0xf);
    NNS_G3dGlb.flags &= ~1;
    NNS_G3dGlb.flags |= 2;
}
