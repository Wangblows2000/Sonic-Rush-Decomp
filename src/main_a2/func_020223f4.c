#include <nitro/types.h>

typedef struct { u8 unk0[0xFC]; u32 flag; } UnkGlb_020223f4;

extern UnkGlb_020223f4 NNS_G3dGlb;
extern u32 data_02084d3c[];
extern u32 data_02084d84[];
extern u32 data_02084db4[];

void NNS_G3dGeBufferOP_N(u32 op, const void *args, u32 n);

void func_020223f4(void) {
    u32 arg;
    NNS_G3dGeBufferOP_N(0x1610, data_02084d3c, 0x11);
    NNS_G3dGeBufferOP_N(0x19, data_02084d84, 0xC);
    arg = 2;
    NNS_G3dGeBufferOP_N(0x10, &arg, 1);
    NNS_G3dGeBufferOP_N(0x15, data_02084db4, 0x20);
    NNS_G3dGlb.flag &= ~1;
    NNS_G3dGlb.flag |= 2;
}
