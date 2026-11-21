#include <nitro/types.h>

typedef struct { u32 w[0x99]; } UnkGlb_020224bc;

extern UnkGlb_020224bc NNS_G3dGlb;
extern u8 data_02084d84[];
extern u8 data_02084d40[];
extern u8 data_02084df4[];

void MTX_Identity43_(void *m);
void MTX_Identity44_(void *m);
void MTX_Identity33_(void *m);

void NNS_G3dGlbInit(void) {
    NNS_G3dGlb.w[0x00 / 4] = 0x17101610;
    NNS_G3dGlb.w[0x04 / 4] = 0;
    NNS_G3dGlb.w[0x48 / 4] = 2;
    NNS_G3dGlb.w[0x7C / 4] = 0x32323232;
    NNS_G3dGlb.w[0x90 / 4] = 0x60293130;
    NNS_G3dGlb.w[0xA4 / 4] = 0x33333333;
    NNS_G3dGlb.w[0xB8 / 4] = 0x002A1B19;
    MTX_Identity43_(data_02084d84);
    MTX_Identity44_(data_02084d40);
    NNS_G3dGlb.w[0x80 / 4] = 0x2D8B62D8;
    NNS_G3dGlb.w[0x84 / 4] = 0x40000200;
    NNS_G3dGlb.w[0x88 / 4] = 0x800001FF;
    NNS_G3dGlb.w[0x8C / 4] = 0xC0080000;
    NNS_G3dGlb.w[0x94 / 4] = 0x4210C210;
    NNS_G3dGlb.w[0x98 / 4] = 0x4210C210;
    NNS_G3dGlb.w[0x9C / 4] = 0x001F008F;
    NNS_G3dGlb.w[0xA0 / 4] = 0xBFFF0000;
    NNS_G3dGlb.w[0xA8 / 4] = 0x00007FFF;
    NNS_G3dGlb.w[0xAC / 4] = 0x4000001F;
    NNS_G3dGlb.w[0xB0 / 4] = 0x800003E0;
    NNS_G3dGlb.w[0xB4 / 4] = 0xC0007C00;
    NNS_G3dGlb.w[0xE0 / 4] = 0;
    NNS_G3dGlb.w[0xE4 / 4] = 0;
    NNS_G3dGlb.w[0xE8 / 4] = 0;
    MTX_Identity33_(data_02084df4);
    NNS_G3dGlb.w[0xEC / 4] = 0x1000;
    NNS_G3dGlb.w[0xF0 / 4] = 0x1000;
    NNS_G3dGlb.w[0xF4 / 4] = 0x1000;
    NNS_G3dGlb.w[0xF8 / 4] = 0;
    NNS_G3dGlb.w[0xFC / 4] = 0;
    NNS_G3dGlb.w[0x248 / 4] = 0;
    NNS_G3dGlb.w[0x244 / 4] = 0;
    NNS_G3dGlb.w[0x240 / 4] = 0;
    NNS_G3dGlb.w[0x254 / 4] = 0;
    NNS_G3dGlb.w[0x24C / 4] = 0;
    NNS_G3dGlb.w[0x250 / 4] = 0x1000;
    NNS_G3dGlb.w[0x25C / 4] = 0;
    NNS_G3dGlb.w[0x258 / 4] = 0;
    NNS_G3dGlb.w[0x260 / 4] = -0x1000;
}
