#pragma opt_lifetimes off
#include <nitro/types.h>

typedef struct {
    u32 flags;
    s32 x;
    s32 y;
    u8 pad0C[0x1e - 0x0c];
    s16 w;
    s16 h;
    u8 pad22[0x34 - 0x22];
    u32 unk34;
    u8 pad38[0x64 - 0x38];
} UnkStruct_02342480;

extern UnkStruct_02342480 data_ov000_0236aad8[];
UnkStruct_02342480 *func_ov000_0233b62c(void);

u8 func_ov000_02342480(s32 x, s32 y) {
    UnkStruct_02342480 *p;
    BOOL hit;
    BOOL h0;
    u8 res;
    u8 i;

    if (data_ov000_0236aad8[2].flags & 1) {
        p = func_ov000_0233b62c();
        if (x >= p->x - 0x10000 && x <= p->x + (p->w << 8) + 0x10000 &&
            y >= p->y - 0x10000 && y <= p->y + p->h * 0x1d0 + 0x10000)
            h0 = TRUE;
        else
            h0 = FALSE;
        if (!h0)
            return 0;
        data_ov000_0236aad8[0].unk34 |= 0x20;
        return 1;
    }
    res = 0;
    for (i = 0; i < 2; i++) {
        p = &data_ov000_0236aad8[i];
        if (x >= p->x - 0x10000 && x <= p->x + (p->w << 8) + 0x10000 &&
            y >= p->y - 0x10000 && y <= p->y + p->h * 0xc0 + 0x10000)
            hit = TRUE;
        else
            hit = FALSE;
        if (hit) {
            p->unk34 |= 0x20;
            res |= 1 << i;
        }
    }
    return res;
}
