#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
    u8 pad8[2];
    u16 flags;
    s16 x1;
    s16 y1;
    s16 x2;
    s16 y2;
} UnkBox_0206d714;

BOOL func_0206d714(UnkBox_0206d714 *b, s32 px, s32 py) {
    s32 left;
    s32 top;
    u16 w;
    u16 h;

    if ((b->flags & 4) && !(b->flags & 0x1000)) {
        if (b->flags & 1)
            left = b->x - b->x2;
        else
            left = b->x + b->x1;
        if (b->flags & 2)
            top = b->y - b->y2;
        else
            top = b->y + b->y1;
        w = b->x2 - b->x1;
        h = b->y2 - b->y1;
        if (left <= px && left + w >= px && top <= py && top + h >= py)
            return TRUE;
    }
    return FALSE;
}
