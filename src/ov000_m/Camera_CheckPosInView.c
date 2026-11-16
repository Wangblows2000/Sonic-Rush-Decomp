#pragma opt_lifetimes off
#include <nitro/types.h>

typedef struct {
    u32 flags;
    s32 x;
    s32 y;
    u8 pad0C[0x1e - 0x0c];
    s16 w;
    s16 h;
    u8 pad22[0x64 - 0x22];
} UnkStruct_023425f4;

extern UnkStruct_023425f4 gCamera[];
UnkStruct_023425f4 *Camera_GetBySwapState(void);

u8 Camera_CheckPosInView(s32 x, s32 y) {
    UnkStruct_023425f4 *p;
    BOOL hit;
    BOOL h0;
    u8 res;
    u8 i;

    if (gCamera[2].flags & 1) {
        p = Camera_GetBySwapState();
        if (x >= p->x - 0x10000 && x <= p->x + (p->w << 8) + 0x10000 &&
            y >= p->y - 0x10000 && y <= p->y + p->h * 0x1d0 + 0x10000)
            h0 = TRUE;
        else
            h0 = FALSE;
        if (!h0)
            return 0;
        gCamera[0].flags |= 0x4000000;
        return 1;
    }
    res = 0;
    for (i = 0; i < 2; i++) {
        p = &gCamera[i];
        if (x >= p->x - 0x10000 && x <= p->x + (p->w << 8) + 0x10000 &&
            y >= p->y - 0x10000 && y <= p->y + p->h * 0xc0 + 0x10000)
            hit = TRUE;
        else
            hit = FALSE;
        if (hit) {
            p->flags |= 0x4000000;
            res |= 1 << i;
        }
    }
    return res;
}
