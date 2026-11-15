#include <nitro/types.h>

typedef struct { u8 unk0[0x20]; fx16 sinR; fx16 cosR; fx32 transS; fx32 transT; u16 origW; u16 origH; } UnkTexMtx_020289d8;

void FX_DivAsync(fx32 a, fx32 b);
fx32 FX_GetDivResult(void);

void func_020289d8(fx32 *m, UnkTexMtx_020289d8 *tm) {
    fx32 w = tm->origW << 12;
    fx32 h = tm->origH << 12;
    FX_DivAsync(h, w);
    m[0] = tm->cosR;
    m[5] = tm->cosR;
    m[1] = (-tm->sinR * FX_GetDivResult()) >> 12;
    FX_DivAsync(w, h);
    m[12] = (((fx32)-(tm->sinR + tm->cosR) + 0x1000) * tm->origW << 3) - ((tm->transS * tm->origW) << 4);
    m[13] = ((tm->sinR - tm->cosR + 0x1000) * tm->origH << 3) + ((tm->transT * tm->origH) << 4);
    m[4] = (tm->sinR * FX_GetDivResult()) >> 12;
}
