#include <nitro/types.h>

typedef struct {
    u8 pad0[0xa];
    u16 flags;
    s16 x1;
    s16 y1;
    s16 x2;
    s16 y2;
    s16 z1;
    s16 z2;
} UnkBox_0206e2d8;

#define XSWAP(a, b) { (a) ^= (b); (b) ^= (a); (a) ^= (b); }

void ObjRect_SetBox3D(UnkBox_0206e2d8 *b, s16 x1, s16 y1, s16 x2, s16 y2, s16 z1, s16 z2) {
    b->flags |= 4;
    b->x1 = x1;
    b->x2 = x2;
    b->y1 = y1;
    b->y2 = y2;
    b->z1 = z1;
    b->z2 = z2;
    if (b->x2 < b->x1)
        XSWAP(b->x1, b->x2);
    if (b->y2 < b->y1)
        XSWAP(b->y1, b->y2);
    if (b->z2 < b->z1)
        XSWAP(b->z1, b->z2);
}
