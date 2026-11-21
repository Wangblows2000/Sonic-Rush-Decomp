#include <nitro/types.h>

typedef struct {
    fx16 x, y, z;
} UnkVec16_02002360;

#define REG_DIVCNT (*(vu16 *)0x04000280)
#define REG_DIV_NUMER (*(volatile u64 *)0x04000290)
#define REG_DIV_DENOM (*(volatile u64 *)0x04000298)
#define REG_DIV_RESULT (*(volatile s64 *)0x040002a0)
#define REG_SQRTCNT (*(vu16 *)0x040002b0)
#define REG_SQRT_RESULT (*(vu32 *)0x040002b4)
#define REG_SQRT_PARAM (*(volatile u64 *)0x040002b8)

void VEC_Fx16Normalize(const UnkVec16_02002360 *pSrc, UnkVec16_02002360 *pDst)
{
    s64 t;
    fx32 sqrt;

    t = pSrc->x * pSrc->x;
    t += pSrc->y * pSrc->y;
    t += pSrc->z * pSrc->z;
    REG_DIVCNT = 2;
    REG_DIV_NUMER = 0x0100000000000000ULL;
    REG_DIV_DENOM = (u64)t;
    REG_SQRTCNT = 1;
    REG_SQRT_PARAM = (u64)(t * 4);
    while (REG_SQRTCNT & 0x8000) {
    }
    sqrt = (fx32)REG_SQRT_RESULT;
    while (REG_DIVCNT & 0x8000) {
    }
    t = REG_DIV_RESULT;
    t = t * sqrt;
    pDst->x = (fx16)((t * pSrc->x + (1LL << 44)) >> 45);
    pDst->y = (fx16)((t * pSrc->y + (1LL << 44)) >> 45);
    pDst->z = (fx16)((t * pSrc->z + (1LL << 44)) >> 45);
}
