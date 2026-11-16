#include <nitro/types.h>

typedef struct {
    fx32 x, y, z;
} UnkVec_02002278;

#define REG_SQRTCNT (*(vu16 *)0x040002b0)
#define REG_SQRT_RESULT (*(vu32 *)0x040002b4)
#define REG_SQRT_PARAM (*(volatile u64 *)0x040002b8)

fx32 VEC_Distance(const UnkVec_02002278 *pPos1, const UnkVec_02002278 *pPos2)
{
    s64 t;
    fx32 diff;

    diff = pPos1->x - pPos2->x;
    t = (s64)diff * diff;
    diff = pPos1->y - pPos2->y;
    t += (s64)diff * diff;
    diff = pPos1->z - pPos2->z;
    t += (s64)diff * diff;
    REG_SQRTCNT = 1;
    REG_SQRT_PARAM = (u64)(t * 4);
    while (REG_SQRTCNT & 0x8000) {
    }
    return (fx32)(((s32)REG_SQRT_RESULT + 1) >> 1);
}
