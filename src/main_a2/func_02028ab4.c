#include <nitro/types.h>

typedef struct { u8 unk0[0x18]; fx32 scaleS; fx32 scaleT; fx16 sinR; fx16 cosR; fx32 transS; fx32 transT; u16 origW; u16 origH; } UnkTexMtx_02028ab4;

void FX_DivAsync(fx32 a, fx32 b);
fx32 FX_GetDivResult(void);

#define FX_MUL_02028ab4(a, b) ((fx32)(((s64)(a) * (b)) >> 12))

void func_02028ab4(fx32 *m, UnkTexMtx_02028ab4 *tm) {
    fx32 w;
    fx32 h;
    fx32 ssSin;
    fx32 ssCos;
    fx32 stSin;
    fx32 stCos;
    w = tm->origW << 12;
    h = tm->origH << 12;
    FX_DivAsync(h, w);
    ssSin = FX_MUL_02028ab4(tm->scaleS, tm->sinR);
    ssCos = FX_MUL_02028ab4(tm->scaleS, tm->cosR);
    stSin = FX_MUL_02028ab4(tm->scaleT, tm->sinR);
    stCos = FX_MUL_02028ab4(tm->scaleT, tm->cosR);
    m[0] = ssCos;
    m[5] = stCos;
    m[1] = (-stSin * FX_GetDivResult()) >> 12;
    FX_DivAsync(w, h);
    m[12] = ((tm->scaleS - (ssSin + ssCos)) * tm->origW << 3) - (fx32)(((s64)tm->scaleS * tm->transS) >> 8) * tm->origW;
    m[13] = ((stSin - stCos - tm->scaleT + 0x2000) * tm->origH << 3) + (fx32)(((s64)tm->scaleT * tm->transT) >> 8) * tm->origH;
    m[4] = (ssSin * FX_GetDivResult()) >> 12;
}
