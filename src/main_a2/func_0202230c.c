#include <nitro/types.h>

typedef struct { u8 unk0[0x7C]; u32 unk7C; u8 unk80[0x7C]; u32 flag; } UnkGlb_0202230c;

extern UnkGlb_0202230c NNS_G3dGlb;
extern u32 data_02084d3c[];
extern u32 data_02084d84[];
extern u32 data_02084df4[];
extern u32 data_02084db4[];
extern u32 data_02084e30[];

void NNS_G3dGeBufferOP_N(u32 op, const void *args, u32 n);
void MTX_Inverse33(const void *src, void *dst);

void func_0202230c(void) {
    u32 arg;
    u32 mtx[12];
    NNS_G3dGeBufferOP_N(0x1610, data_02084d3c, 0x11);
    NNS_G3dGeBufferOP_N(0x19, data_02084d84, 0xC);
    NNS_G3dGeBufferOP_N(0x1B19, data_02084df4, 0xF);
    arg = 2;
    NNS_G3dGeBufferOP_N(0x10, &arg, 1);
    MTX_Inverse33(data_02084df4, mtx);
    NNS_G3dGeBufferOP_N(0x17, mtx, 0xC);
    NNS_G3dGeBufferOP_N(NNS_G3dGlb.unk7C, (u8 *)data_02084db4 + 4, 0xE);
    NNS_G3dGeBufferOP_N(0x15, NULL, 0);
    NNS_G3dGeBufferOP_N(0x2A, data_02084e30, 1);
    NNS_G3dGlb.flag |= 1;
    NNS_G3dGlb.flag &= ~2;
}
