#include <nitro/types.h>

typedef struct { u8 unk0[0x18]; fx32 scaleS; fx32 scaleT; fx16 sinR; fx16 cosR; fx32 transS; fx32 transT; u16 origW; u16 origH; } UnkTexMtx_02029d38;

void texmtxCalc_flagRS_xsi(fx32 *m, UnkTexMtx_02029d38 *tm)
{
    fx32 ts;
    fx32 tt;
    m[0] = 0x1000;
    m[5] = 0x1000;
    m[1] = 0;
    ts = -tm->transS;
    tt = -tm->transT;
    m[12] = tm->origW * ts << 4;
    m[13] = -tm->origH * tt << 4;
    m[4] = 0;
}
