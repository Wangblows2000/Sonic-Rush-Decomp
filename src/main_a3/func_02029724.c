#include <nitro/types.h>

typedef struct { u8 unk0[0x18]; fx32 scaleS; fx32 scaleT; fx16 sinR; fx16 cosR; fx32 transS; fx32 transT; u16 origW; u16 origH; } UnkTexMtx_02029724;

void func_02029724(fx32 *m, UnkTexMtx_02029724 *tm)
{
    fx32 a;
    fx32 b;
    m[0] = tm->scaleS;
    m[5] = tm->scaleT;
    m[1] = 0;
    a = (-tm->origW << 11) - tm->transS * tm->origW;
    b = tm->transT * tm->origH + (-tm->origH << 11);
    m[12] = (fx32)(((s64)tm->scaleS * a) >> 8) + (tm->origW << 15);
    m[13] = (fx32)(((s64)tm->scaleT * b) >> 8) + (tm->origH << 15);
    m[4] = 0;
}
