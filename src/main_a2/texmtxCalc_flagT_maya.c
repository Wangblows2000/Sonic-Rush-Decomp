#include <nitro/types.h>

typedef struct { u8 unk0[0x18]; fx32 scaleS; fx32 scaleT; fx16 sinR; fx16 cosR; u8 unk24[8]; u16 origW; u16 origH; } UnkTexMtx_02028824;

void FX_DivAsync(fx32 a, fx32 b);
fx32 FX_GetDivResult(void);

#define FX_MUL_02028824(a, b) ((fx32)(((s64)(a) * (b)) >> 12))

void texmtxCalc_flagT_maya(fx32 *m, UnkTexMtx_02028824 *tm) {
    fx32 w;
    fx32 h;
    fx32 ssSin;
    fx32 ssCos;
    fx32 stSin;
    fx32 stCos;
    w = tm->origW << 12;
    h = tm->origH << 12;
    FX_DivAsync(h, w);
    ssSin = FX_MUL_02028824(tm->scaleS, tm->sinR);
    ssCos = FX_MUL_02028824(tm->scaleS, tm->cosR);
    stSin = FX_MUL_02028824(tm->scaleT, tm->sinR);
    stCos = FX_MUL_02028824(tm->scaleT, tm->cosR);
    m[0] = ssCos;
    m[5] = stCos;
    m[1] = (-stSin * FX_GetDivResult()) >> 12;
    FX_DivAsync(w, h);
    m[12] = (tm->scaleS - (ssSin + ssCos)) * tm->origW << 3;
    m[13] = (stSin - stCos - tm->scaleT + 0x2000) * tm->origH << 3;
    m[4] = (ssSin * FX_GetDivResult()) >> 12;
}
