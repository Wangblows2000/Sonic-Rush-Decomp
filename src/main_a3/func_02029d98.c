#include <nitro/types.h>

typedef struct { u8 unk0[0x18]; fx32 scaleS; fx32 scaleT; fx16 sinR; fx16 cosR; fx32 transS; fx32 transT; u16 origW; u16 origH; } UnkTexMtx_02029d98;

#define FX_MUL_02029d98(a, b) ((fx32)(((s64)(a) * (b)) >> 12))

void func_02029d98(fx32 *m, UnkTexMtx_02029d98 *tm)
{
    fx32 ts;
    fx32 tt;
    m[0] = tm->scaleS;
    m[5] = tm->scaleT;
    m[1] = 0;
    ts = FX_MUL_02029d98(tm->transS, tm->scaleS);
    tt = FX_MUL_02029d98(-tm->transT, tm->scaleT);
    m[12] = tm->origW * -ts << 4;
    m[13] = -tm->origH * (tm->scaleT + tt - 0x1000) << 4;
    m[4] = 0;
}
