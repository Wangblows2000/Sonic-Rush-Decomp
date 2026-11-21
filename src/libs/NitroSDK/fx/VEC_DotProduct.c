#include <nitro/types.h>

typedef struct { s32 x, y, z; } VecFx32;
s32 VEC_DotProduct(const VecFx32 *a, const VecFx32 *b) { return (s32)(((s64)a->x * b->x + (s64)a->y * b->y + (s64)a->z * b->z + 0x800) >> 12); }
