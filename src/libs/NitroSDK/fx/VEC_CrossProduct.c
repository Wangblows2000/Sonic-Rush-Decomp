#include <nitro/types.h>

typedef struct {
    fx32 x, y, z;
} UnkVec_0200265c;

void VEC_CrossProduct(const UnkVec_0200265c *a, const UnkVec_0200265c *b, UnkVec_0200265c *axb)
{
    fx32 x, y, z;

    x = (fx32)(((s64)a->y * b->z - (s64)a->z * b->y + 0x800) >> 12);
    y = (fx32)(((s64)a->z * b->x - (s64)a->x * b->z + 0x800) >> 12);
    z = (fx32)(((s64)a->x * b->y - (s64)a->y * b->x + 0x800) >> 12);
    axb->x = x;
    axb->y = y;
    axb->z = z;
}
