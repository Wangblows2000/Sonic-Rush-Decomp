#include <nitro/types.h>

typedef struct {
    u8 pad0[0x34];
    s32 max;
} UnkStruct_022ecdcc;

#pragma thumb on
s32 Records_NextPageStart(UnkStruct_022ecdcc *p, s32 n) {
    s32 r;

    r = n % 16;
    if (r == 0) {
        n += 16;
        if (n > p->max) n = p->max;
    } else {
        n += 16 - r;
        if (n > p->max) n = p->max;
    }
    return n;
}
#pragma thumb reset
