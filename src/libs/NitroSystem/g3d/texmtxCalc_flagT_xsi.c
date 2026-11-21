#include <nitro/types.h>

typedef struct { u8 unk0[0x18]; fx32 scaleS; fx32 scaleT; fx16 sinR; fx16 cosR; u8 unk24[8]; u16 origW; u16 origH; } UnkTexMtx_02029c60;

void FX_DivAsync(fx32 a, fx32 b);
fx32 FX_GetDivResult(void);

#define FX_MUL_02029c60(a, b) ((fx32)(((s64)(a) * (b)) >> 12))

void texmtxCalc_flagT_xsi(fx32 *m, UnkTexMtx_02029c60 *tm) {
    fx32 w;
    fx32 h;
    fx32 sinR;
    fx32 ssSin;
    fx32 scaleT;
    fx32 stCos;
    fx32 cosR;
    fx32 scaleS;
    w = tm->origW << 12;
    h = tm->origH << 12;
    FX_DivAsync(h, w);
    cosR = tm->cosR;
    scaleS = tm->scaleS;
    sinR = tm->sinR;
    scaleT = tm->scaleT;
    ssSin = FX_MUL_02029c60(scaleS, sinR);
    stCos = FX_MUL_02029c60(scaleT, cosR);
    m[0] = FX_MUL_02029c60(scaleS, cosR);
    m[5] = stCos;
    m[1] = (FX_MUL_02029c60(scaleT, sinR) * FX_GetDivResult()) >> 12;
    FX_DivAsync(w, h);
    m[12] = (tm->origW * ssSin) << 4;
    m[13] = (-tm->origH * (stCos - 0x1000)) << 4;
    m[4] = (-ssSin * FX_GetDivResult()) >> 12;
}
