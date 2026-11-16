#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
    u8 pad8[4];
    s16 x1;
    s16 y1;
    s16 x2;
    s16 y2;
} UnkBox_0206d4c8;

s32 ObjRect_HitCenterY(UnkBox_0206d4c8 *a, UnkBox_0206d4c8 *b) {
    u8 maxIdx;
    s32 max;
    u8 k;
    s32 min;
    u8 minIdx;
    u8 i;
    s32 h;
    s32 v[4];

    k = 0;
    v[0] = a->y + a->y1;
    maxIdx = 0;
    v[1] = a->y + a->y2;
    v[2] = b->y + b->y1;
    v[3] = b->y + b->y2;
    max = v[0];
    if (v[1] > max) {
        max = v[1];
        maxIdx = 1;
    }
    if (v[2] > max) {
        max = v[2];
        maxIdx = 2;
    }
    if (v[3] > max)
        maxIdx = 3;
    min = v[0];
    minIdx = 0;
    if (v[1] < min) {
        min = v[1];
        minIdx = 1;
    }
    if (v[2] < min) {
        min = v[2];
        minIdx = 2;
    }
    if (v[3] < min)
        minIdx = 3;
    for (i = 0;; i++) {
        if (i != maxIdx && i != minIdx) {
            v[k] = v[i];
            if (k != 0)
                break;
            k++;
        }
    }
    h = (v[0] - v[1]) >> 1;
    if (h < 0)
        h = -h;
    return (v[0] > v[1] ? h + v[1] : h + v[0]) << 8;
}
