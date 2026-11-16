#include <nitro/types.h>

extern s32 data_ov000_02369b18[];
extern u16 data_ov000_02363070[];
extern u16 data_ov000_02363074[];
extern u32 data_ov000_02365414[];
extern u16 data_ov000_0236306c[];

u8 *NNS_G3dGetMdlSet(s32 p);
void NNS_G3dGlbMaterialColorSpecEmi(s32 a, s32 b, s32 c);
void NNSi_G3dModifyMatFlag(void *p, s32 a, s32 b);
void Boss_LoadModelPalettes(s32 a, u32 *p1, u16 *p2, u16 *p3, u16 n);

void func_ov000_022dad70(s32 k, BOOL f) {
    u8 *r = NNS_G3dGetMdlSet(data_ov000_02369b18[2]);
    u8 *q = r + *(u32 *)(r + *(u16 *)(r + 0xe) + 0xc);
    u16 *t;
    if (f) {
        NNS_G3dGlbMaterialColorSpecEmi(0x2108, 0x2108, 0);
        NNSi_G3dModifyMatFlag(q, 0, 0x400);
        t = &data_ov000_02363070[k];
    } else {
        NNSi_G3dModifyMatFlag(q, 1, 0x400);
        t = &data_ov000_02363074[k];
    }
    Boss_LoadModelPalettes(data_ov000_02369b18[2], data_ov000_02365414, t, data_ov000_0236306c, 1);
}
