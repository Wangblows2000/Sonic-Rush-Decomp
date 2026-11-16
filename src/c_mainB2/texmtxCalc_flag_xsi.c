#include <nitro/types.h>

typedef s64 fx64;

typedef struct {
    u32 flag;
    u32 prmMatColor0;
    u32 prmMatColor1;
    u32 prmPolygonAttr;
    u32 prmTexImage;
    u32 prmTexPltt;
    fx32 scaleS, scaleT;
    fx16 sinR, cosR;
    fx32 transS, transT;
    u16 origWidth, origHeight;
} UnkMatAnmResult_02029f0c;

void FX_DivAsync(fx32 a, fx32 b);
fx32 FX_GetDivResult(void);

void texmtxCalc_flag_xsi(fx32 *m, const UnkMatAnmResult_02029f0c *anm)
{
    fx32 ss_sin, ss_cos;
    fx32 st_sin, st_cos;
    fx32 tmpW, tmpH;
    fx64 tmp;
    fx32 p, q;

    tmpW = (s32)anm->origWidth << 12;
    tmpH = (s32)anm->origHeight << 12;
    FX_DivAsync(tmpH, tmpW);

    ss_sin = (fx32)((fx64)anm->scaleS * anm->sinR >> 12);
    ss_cos = (fx32)((fx64)anm->scaleS * anm->cosR >> 12);
    st_sin = (fx32)((fx64)anm->scaleT * anm->sinR >> 12);
    st_cos = (fx32)((fx64)anm->scaleT * anm->cosR >> 12);

    m[0] = ss_cos;
    m[5] = st_cos;

    m[1] = st_sin * FX_GetDivResult() >> 12;
    FX_DivAsync(tmpW, tmpH);

    tmp = ((fx64)anm->transS * anm->cosR + (fx64)anm->transT * anm->sinR) >> 12;
    p = (fx32)((tmp * anm->scaleS) >> 12);
    tmp = ((fx64)anm->transS * anm->sinR - (fx64)anm->transT * anm->cosR) >> 12;
    q = (fx32)((tmp * anm->scaleT) >> 12);

    m[12] = (anm->origWidth * (ss_sin - p)) << 4;
    m[13] = (-anm->origHeight * (st_cos + q - 0x1000)) << 4;

    m[4] = -ss_sin * FX_GetDivResult() >> 12;
}
