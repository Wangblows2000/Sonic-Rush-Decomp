#include <nitro/types.h>

typedef struct {
    fx32 x, y, z;
} UnkVec_VEC_Mag;

#define REG_SQRTCNT (*(vu16 *)0x040002b0)
#define REG_SQRT_RESULT (*(vu32 *)0x040002b4)
#define REG_SQRT_PARAM (*(volatile u64 *)0x040002b8)

fx32 VEC_Mag(const UnkVec_VEC_Mag *pSrc)
{
    s64 t;

    t = (s64)pSrc->x * pSrc->x;
    t += (s64)pSrc->y * pSrc->y;
    t += (s64)pSrc->z * pSrc->z;
    REG_SQRTCNT = 1;
    REG_SQRT_PARAM = (u64)(t * 4);
    while (REG_SQRTCNT & 0x8000) {
    }
    return (fx32)(((s32)REG_SQRT_RESULT + 1) >> 1);
}
