#include <nitro/types.h>

typedef struct { u8 unk0[0x20]; fx16 sinR; fx16 cosR; fx32 transS; fx32 transT; u16 origW; u16 origH; } UnkTexMtx_020297ac;

void FX_DivAsync(fx32 a, fx32 b);
fx32 FX_GetDivResult(void);

void texmtxCalc_flagS_3dsmax(fx32 *m, UnkTexMtx_020297ac *tm) {
    fx32 w = tm->origW << 12;
    fx32 h = tm->origH << 12;
    s32 a;
    s32 b;
    FX_DivAsync(h, w);
    m[0] = tm->cosR;
    m[5] = tm->cosR;
    m[1] = (tm->sinR * FX_GetDivResult()) >> 12;
    FX_DivAsync(w, h);
    a = (-tm->origW << 11) - tm->transS * tm->origW;
    b = tm->transT * tm->origH + (-tm->origH << 11);
    m[12] = (fx32)(((s64)tm->cosR * a - (s64)tm->sinR * b) >> 8) + (tm->origW << 15);
    m[13] = (fx32)(((s64)tm->sinR * a + (s64)tm->cosR * b) >> 8) + (tm->origH << 15);
    m[4] = (-tm->sinR * FX_GetDivResult()) >> 12;
}
