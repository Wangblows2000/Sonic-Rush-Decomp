#include <nitro/types.h>

typedef struct {
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
} UnkRect_02066638;

typedef s32 (*UnkFunc_02066638)(s32 x, s32 y, u32 flags, u8 *outA, u8 *outB);

extern UnkRect_02066638 data_022c4dcc;

s32 MapCollision_GetTileHeight(s32 x, s32 y, u32 flags, u8 *outA, u8 *outB);
s32 MapCollision_GetTileWidth(s32 x, s32 y, u32 flags, u8 *outA, u8 *outB);

static inline s32 Calc_02066638(s32 h, s32 step, s32 sub) {
    if (h > 0) {
        if (step > 0)
            return h - (sub + 1);
        else
            return 8 - sub;
    } else {
        if (step > 0)
            return -(sub + 1);
        else
            return h + sub;
    }
}

s32 MapCollision_ProbeDistanceLong(s32 x, s32 y, u32 flags, u32 dir, u8 *outA, u8 *outB) {
    s32 sa;
    s32 sb;
    UnkFunc_02066638 fn;
    s32 sub;
    s32 off;
    s32 step;
    s32 h;
    s32 d;

    off = 0;
    step = 8;
    if (flags & 0x20) {
        switch (dir) {
        case 2:
            d = data_022c4dcc.bottom - y;
            break;
        case 3:
            d = y - data_022c4dcc.top;
            break;
        case 1:
            d = x - data_022c4dcc.left;
            break;
        case 0:
            d = data_022c4dcc.right - x;
            break;
        }
        if (d < -0x1f)
            d = -0x1f;
        else if (d > 0x1f)
            d = 0x1f;
        return d;
    }
    if (outA != NULL)
        sa = *outA;
    if (outB != NULL)
        sb = *outB;
    if (dir & 1)
        step = -8;
    if (dir & 2) {
        sub = (s8)(y & 7);
        x ^= y;
        y ^= x;
        x ^= y;
        fn = MapCollision_GetTileHeight;
        h = MapCollision_GetTileHeight(x, y, flags, outA, outB);
    } else {
        fn = MapCollision_GetTileWidth;
        h = MapCollision_GetTileWidth(x, y, flags, outA, outB);
        sub = (s8)(x & 7);
    }
    if (h == 0) {
        off += step;
        h = fn(x + off, y, flags, outA, outB);
        if (h == 0) {
            h = fn(x + (off + step), y, flags, outA, outB);
            if (h == 0) {
                if (outA != NULL)
                    *outA = sa;
                if (outB != NULL)
                    *outB = sb;
                return (step > 0 ? 8 - sub : sub + 1) + 16;
            }
            if (h == 8)
                return (step > 0 ? -(sub + 1) : sub - 8) + 16;
            return Calc_02066638(h, step, sub) + 16;
        }
        if (h == 8)
            return (step > 0 ? -(sub + 1) : sub - 8) + 8;
        return Calc_02066638(h, step, sub) + 8;
    } else if (h == 8) {
        if (outA != NULL)
            sa = *outA;
        if (outB != NULL)
            sb = *outB;
        off -= step;
        h = fn(x + off, y, flags, outA, outB);
        if (h == 8) {
            if (outA != NULL)
                sa = *outA;
            if (outB != NULL)
                sb = *outB;
            h = fn(x + (off - step), y, flags, outA, outB);
            if (h == 0) {
                if (outA != NULL)
                    *outA = sa;
                if (outB != NULL)
                    *outB = sb;
                if (step > 0)
                    sub = 8 - (sub + 1);
                return sub - 16;
            }
            if (h == 8)
                return (step > 0 ? -(sub + 1) : sub - 8) - 16;
            return Calc_02066638(h, step, sub) - 16;
        }
        if (h == 0) {
            if (outA != NULL)
                *outA = sa;
            if (outB != NULL)
                *outB = sb;
            if (step > 0)
                sub = 8 - (sub + 1);
            return sub - 8;
        }
        return Calc_02066638(h, step, sub) - 8;
    }
    return Calc_02066638(h, step, sub);
}
