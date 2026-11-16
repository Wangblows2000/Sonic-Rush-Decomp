#include <nitro/types.h>

typedef struct { u8 unk0[0x20]; fx16 sinR; fx16 cosR; fx32 transS; fx32 transT; u16 origW; u16 origH; } UnkTexMtx_02029e20;

void FX_DivAsync(fx32 a, fx32 b);
fx32 FX_GetDivResult(void);

void func_02029e20(fx32 *m, UnkTexMtx_02029e20 *tm) {
    fx32 w = tm->origW << 12;
    fx32 h = tm->origH << 12;
    FX_DivAsync(h, w);
    m[0] = tm->cosR;
    m[5] = tm->cosR;
    m[1] = (tm->sinR * FX_GetDivResult()) >> 12;
    FX_DivAsync(w, h);
    {
        fx32 tx = (fx32)(((s64)tm->transS * tm->cosR + (s64)tm->transT * tm->sinR) >> 12);
        fx32 ty = (fx32)(((s64)tm->transS * tm->sinR - (s64)tm->transT * tm->cosR) >> 12);
        m[12] = (tm->sinR - tx) * tm->origW << 4;
        m[13] = (tm->cosR + ty - 0x1000) * -tm->origH << 4;
    }
    m[4] = (-tm->sinR * FX_GetDivResult()) >> 12;
}
