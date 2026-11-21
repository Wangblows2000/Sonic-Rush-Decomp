#include <nitro/types.h>

typedef struct { u8 unk0[0x20]; fx16 sinR; fx16 cosR; u8 unk24[8]; u16 origW; u16 origH; } UnkTexMtx_02028760;

void FX_DivAsync(fx32 a, fx32 b);
fx32 FX_GetDivResult(void);

void texmtxCalc_flagTS_maya(fx32 *m, UnkTexMtx_02028760 *tm) {
    fx32 w = tm->origW << 12;
    fx32 h = tm->origH << 12;
    FX_DivAsync(h, w);
    m[0] = tm->cosR;
    m[5] = tm->cosR;
    m[1] = (-tm->sinR * FX_GetDivResult()) >> 12;
    FX_DivAsync(w, h);
    m[12] = ((fx32)-(tm->sinR + tm->cosR) + 0x1000) * tm->origW << 3;
    m[13] = (tm->sinR - tm->cosR + 0x1000) * tm->origH << 3;
    m[4] = (tm->sinR * FX_GetDivResult()) >> 12;
}
