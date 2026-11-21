#include <nitro/types.h>

typedef struct {
    fx32 _[16];
} UnkMtx44_0200468c;

void FX_InvAsync(fx32 x);
s64 FX_GetDivResultFx64c(void);

#define REG_MTX_MODE (*(vu32 *)0x04000440)
#define REG_MTX_LOAD_4x4 (*(vu32 *)0x04000458)
#define REG_DIV_NUMER (*(volatile s64 *)0x04000290)
#define REG_DIV_DENOM (*(volatile s64 *)0x04000298)

static inline void func_0200468c_InvAsyncImm(fx32 x)
{
    REG_DIV_NUMER = (s64)0x1000 << 32;
    REG_DIV_DENOM = (u32)x;
}

static inline fx32 func_0200468c_Mul32x64c(fx32 a, s64 b)
{
    return (fx32)((b * a + 0x80000000) >> 32);
}

void G3i_OrthoW_(fx32 t, fx32 b, fx32 l, fx32 r, fx32 n, fx32 f, fx32 scaleW, BOOL draw, UnkMtx44_0200468c *mtx)
{
    s64 inv1, inv2, inv3;
    fx32 x0, x1, x2;
    vu32 *load;

    FX_InvAsync(r - l);
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
        mtx->_[11] = 0;
        mtx->_[15] = scaleW;
    }
    inv1 = FX_GetDivResultFx64c();
    func_0200468c_InvAsyncImm(t - b);
    if (scaleW != 0x1000) {
        inv1 = (inv1 * scaleW) / 0x1000;
    }
    x0 = func_0200468c_Mul32x64c(0x2000, inv1);
    if (draw) {
        *load = x0;
        *load = 0;
        *load = 0;
        *load = 0;
        *load = 0;
    }
    if (mtx != NULL) {
        mtx->_[0] = x0;
    }

    inv2 = FX_GetDivResultFx64c();
    func_0200468c_InvAsyncImm(n - f);
    if (scaleW != 0x1000) {
        inv2 = (inv2 * scaleW) / 0x1000;
    }
    x0 = func_0200468c_Mul32x64c(0x2000, inv2);
    if (draw) {
        *load = x0;
        *load = 0;
        *load = 0;
        *load = 0;
        *load = 0;
    }
    if (mtx != NULL) {
        mtx->_[5] = x0;
    }

    inv3 = FX_GetDivResultFx64c();
    if (scaleW != 0x1000) {
        inv3 = (inv3 * scaleW) / 0x1000;
    }
    x0 = func_0200468c_Mul32x64c(0x2000, inv3);
    if (draw) {
        *load = x0;
        *load = 0;
    }
    if (mtx != NULL) {
        mtx->_[10] = x0;
    }

    x0 = func_0200468c_Mul32x64c(-(r + l), inv1);
    x1 = func_0200468c_Mul32x64c(-(t + b), inv2);
    x2 = func_0200468c_Mul32x64c(f + n, inv3);
    if (draw) {
        *load = x0;
        *load = x1;
        *load = x2;
        *load = scaleW;
    }
    if (mtx != NULL) {
        mtx->_[12] = x0;
        mtx->_[13] = x1;
        mtx->_[14] = x2;
    }
}
