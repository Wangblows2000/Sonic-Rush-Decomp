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
} UnkBox_0206e3c0;

#define XSWAP(a, b) { (a) ^= (b); (b) ^= (a); (a) ^= (b); }

void func_0206e3c0(UnkBox_0206e3c0 *b, s16 x1, s16 y1, s16 x2, s16 y2) {
    b->flags |= 4;
    b->x1 = x1;
    b->x2 = x2;
    b->y1 = y1;
    b->y2 = y2;
    b->z1 = -8;
    b->z2 = 8;
    if (b->x2 < b->x1)
        XSWAP(b->x1, b->x2);
    if (b->y2 < b->y1)
        XSWAP(b->y1, b->y2);
}
