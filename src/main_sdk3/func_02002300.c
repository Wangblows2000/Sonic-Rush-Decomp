#include <nitro/types.h>

typedef struct {
    fx32 x, y, z;
} UnkVec_02002300;

void func_02002300(fx32 a, const UnkVec_02002300 *v1, const UnkVec_02002300 *v2, UnkVec_02002300 *pDest)
{
    pDest->x = (fx32)(((s64)a * v1->x) >> 12) + v2->x;
    pDest->y = (fx32)(((s64)a * v1->y) >> 12) + v2->y;
    pDest->z = (fx32)(((s64)a * v1->z) >> 12) + v2->z;
}
