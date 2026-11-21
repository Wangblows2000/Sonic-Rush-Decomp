#include <nitro/types.h>

typedef struct {
    u8 unk0[0x18];
    fx32 scaleS;
    fx32 scaleT;
    fx16 sinR;
    fx16 cosR;
    fx32 transS;
    fx32 transT;
    u16 origW;
    u16 origH;
} UnkTexMtx_02028958;

void texmtxCalc_flagR_maya(fx32 *m, UnkTexMtx_02028958 *tm)
{
    m[0] = tm->scaleS;
    m[5] = tm->scaleT;
    m[1] = 0;
    m[12] = -(fx32)(((s64)tm->scaleS * tm->transS) >> 8) * tm->origW;
    m[13] = ((0x2000 - tm->scaleT - tm->scaleT) * tm->origH << 3) + (fx32)(((s64)tm->scaleT * tm->transT) >> 8) * tm->origH;
    m[4] = 0;
}
