#include <nitro/types.h>

typedef struct { s32 x, y, z; } VecFx32;
void VEC_Add(const VecFx32 *a, const VecFx32 *b, VecFx32 *c) { c->x = a->x + b->x; c->y = a->y + b->y; c->z = a->z + b->z; }
