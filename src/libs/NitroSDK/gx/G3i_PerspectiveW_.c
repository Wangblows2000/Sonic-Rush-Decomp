#include <nitro/types.h>

typedef struct {
    fx32 _[16];
} UnkMtx44_02004988;

s64 FX_GetDivResultFx64c(void);
fx32 FX_GetDivResult(void);
fx32 FX_Div(fx32 numer, fx32 denom);

#define REG_MTX_MODE (*(vu32 *)0x04000440)
#define REG_MTX_LOAD_4x4 (*(vu32 *)0x04000458)
#define REG_DIV_NUMER (*(volatile s64 *)0x04000290)
#define REG_DIV_DENOM (*(volatile s64 *)0x04000298)

static inline void func_02004988_InvAsyncImm(fx32 x)
{
    REG_DIV_NUMER = (s64)0x1000 << 32;
    REG_DIV_DENOM = (u32)x;
}

static inline fx32 func_02004988_Mul32x64c(fx32 a, s64 b)
{
    return (fx32)((b * a + 0x80000000) >> 32);
}

static inline fx32 func_02004988_Mul(fx32 a, fx32 b)
{
    return (fx32)(((s64)a * b + 0x800) >> 12);
}

static inline void func_02004988_DivAsyncImm(fx32 numer, fx32 denom)
{
    REG_DIV_NUMER = (s64)numer << 32;
    REG_DIV_DENOM = (u32)denom;
}

void G3i_PerspectiveW_(fx32 fovySin, fx32 fovyCos, fx32 aspect, fx32 n, fx32 f, fx32 scaleW, BOOL draw, UnkMtx44_02004988 *mtx)
{
    fx32 fovyCot;
    fx32 temp;
    s64 inv;
    fx32 x0, x1;
    vu32 *load;

    fovyCot = FX_Div(fovyCos, fovySin);
    if (scaleW != 0x1000) {
        fovyCot = (fovyCot * scaleW) / 0x1000;
    }
    func_02004988_DivAsyncImm(fovyCot, aspect);
    if (draw) {
        REG_MTX_MODE = 0;
        load = &REG_MTX_LOAD_4x4;
    }
    if (mtx != NULL) {
        mtx->_[1] = 0;
        mtx->_[2] = 0;
        mtx->_[3] = 0;
        mtx->_[4] = 0;
        mtx->_[6] = 0;
        mtx->_[7] = 0;
        mtx->_[8] = 0;
        mtx->_[9] = 0;
        mtx->_[11] = -scaleW;
        mtx->_[12] = 0;
        mtx->_[13] = 0;
        mtx->_[15] = 0;
    }
    temp = FX_GetDivResult();
    func_02004988_InvAsyncImm(n - f);
    if (draw) {
        *load = temp;
        *load = 0;
        *load = 0;
        *load = 0;
        *load = 0;
        *load = fovyCot;
        *load = 0;
        *load = 0;
        *load = 0;
        *load = 0;
    }
    if (mtx != NULL) {
        mtx->_[0] = temp;
        mtx->_[5] = fovyCot;
    }
    inv = FX_GetDivResultFx64c();
    if (scaleW != 0x1000) {
        inv = (inv * scaleW) / 0x1000;
    }
    x0 = func_02004988_Mul32x64c(f + n, inv);
    x1 = func_02004988_Mul32x64c(func_02004988_Mul(n << 1, f), inv);
    if (draw) {
        *load = x0;
        *load = -scaleW;
        *load = 0;
        *load = 0;
        *load = x1;
        *load = 0;
    }
    if (mtx != NULL) {
        mtx->_[10] = x0;
        mtx->_[14] = x1;
    }
}
