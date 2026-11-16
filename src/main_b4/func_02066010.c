#include <nitro/types.h>

typedef struct {
    u8 pad0[0x58];
    u32 flags;
} UnkPlayer_02066010;

s32 func_02066638(s32 x, s32 y, u8 layer, u8 dir, u8 *angle, u8 *attr);
s32 func_02067670(UnkPlayer_02066010 *p, s32 x, s32 y, u8 layer, u8 dir, u8 *angle, u8 *attr);

s32 func_02066010(UnkPlayer_02066010 *p, s32 x, s32 y, u8 layer, u8 dir, u8 *angle, u8 *attr) {
    s32 d = 0x20;
    s32 d2;
    if (!(p->flags & 0x1000))
        d = func_02066638(x, y, layer, dir, angle, attr);
    if (!(p->flags & 0x200)) {
        d2 = func_02067670(p, x, y, layer, dir, angle, attr);
        if (d > d2)
            d = d2;
    }
    return d;
}
