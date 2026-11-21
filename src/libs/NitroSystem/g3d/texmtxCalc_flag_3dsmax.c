#include <nitro/types.h>

typedef struct { u8 unk0[0x18]; fx32 scaleS; fx32 scaleT; fx16 sinR; fx16 cosR; fx32 transS; fx32 transT; u16 origW; u16 origH; } UnkTexMtx_020298a4;

void FX_DivAsync(fx32 a, fx32 b);
fx32 FX_GetDivResult(void);

#define FX_MUL_020298a4(a, b) ((fx32)(((s64)(a) * (b)) >> 12))

void texmtxCalc_flag_3dsmax(fx32 *m, UnkTexMtx_020298a4 *tm) {
    fx32 w;
    fx32 h;
    fx32 ssSin;
    fx32 ssCos;
    fx32 stSin;
    fx32 stCos;
    s32 a;
    s32 b;
    w = tm->origW << 12;
    h = tm->origH << 12;
    FX_DivAsync(h, w);
    ssCos = FX_MUL_020298a4(tm->scaleS, tm->cosR);
    ssSin = FX_MUL_020298a4(tm->scaleS, tm->sinR);
    stCos = FX_MUL_020298a4(tm->scaleT, tm->cosR);
    stSin = FX_MUL_020298a4(tm->scaleT, tm->sinR);
    m[0] = ssCos;
    m[5] = stCos;
    m[1] = (stSin * FX_GetDivResult()) >> 12;
    FX_DivAsync(w, h);
    a = (-tm->origW << 11) - tm->transS * tm->origW;
    b = tm->transT * tm->origH + (-tm->origH << 11);
    m[12] = (fx32)(((s64)ssCos * a - (s64)ssSin * b) >> 8) + (tm->origW << 15);
    m[13] = (fx32)(((s64)stSin * a + (s64)stCos * b) >> 8) + (tm->origH << 15);
    m[4] = (-ssSin * FX_GetDivResult()) >> 12;
}
