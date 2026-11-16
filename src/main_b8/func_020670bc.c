#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
} UnkObj_020670bc;

typedef struct {
    UnkObj_020670bc *obj;
} UnkEntry_020670bc;

typedef s32 (*UnkFunc_020670bc)(UnkEntry_020670bc *e, s32 x, s32 y, u32 flags, u8 *outA, u8 *outB);

void func_02067c6c(UnkEntry_020670bc *e, s16 *ox, s16 *oy);
s32 func_02066bc0(UnkEntry_020670bc *e, s32 x, s32 y, u32 flags, u8 *outA, u8 *outB);
s32 func_02066e3c(UnkEntry_020670bc *e, s32 x, s32 y, u32 flags, u8 *outA, u8 *outB);

static inline s32 Calc_020670bc(s32 h, s32 step, s32 sub) {
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

s32 func_020670bc(UnkEntry_020670bc *e, s32 x, s32 y, u32 flags, u8 dir, u8 *outA, u8 *outB) {
    s32 sa;
    s32 sb;
    s16 ox;
    UnkFunc_020670bc fn;
    s32 sub;
    s32 off;
    s16 oy;
    s32 step;
    s32 h;

    off = 0;
    ox = 0;
    oy = 0;
    step = 8;
    func_02067c6c(e, &ox, &oy);
    if (outA != NULL)
        sa = *outA;
    if (outB != NULL)
        sb = *outB;
    if (dir & 1)
        step = -8;
    if (dir & 2) {
        sub = (s8)((y - ((e->obj->y >> 8) + oy)) & 7);
        fn = func_02066bc0;
        x ^= y;
        y ^= x;
        x ^= y;
        h = func_02066bc0(e, x, y, flags, outA, outB);
    } else {
        fn = func_02066e3c;
        h = func_02066e3c(e, x, y, flags, outA, outB);
        sub = (s8)((x - ((e->obj->x >> 8) + ox)) & 7);
    }
    if (h == 0) {
        off += step;
        h = fn(e, x + off, y, flags, outA, outB);
        if (h == 0) {
            h = fn(e, x + (off + step), y, flags, outA, outB);
            if (h == 0) {
                if (outA != NULL)
                    *outA = sa;
                if (outB != NULL)
                    *outB = sb;
                return (step > 0 ? 8 - sub : sub + 1) + 16;
            }
            if (h == 8)
                return (step > 0 ? -(sub + 1) : sub - 8) + 16;
            return Calc_020670bc(h, step, sub) + 16;
        }
        if (h == 8)
            return (step > 0 ? -(sub + 1) : sub - 8) + 8;
        return Calc_020670bc(h, step, sub) + 8;
    } else if (h == 8) {
        if (outA != NULL)
            sa = *outA;
        if (outB != NULL)
            sb = *outB;
        off -= step;
        h = fn(e, x + off, y, flags, outA, outB);
        if (h == 8) {
            if (outA != NULL)
                sa = *outA;
            if (outB != NULL)
                sb = *outB;
            h = fn(e, x + (off - step), y, flags, outA, outB);
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
            return Calc_020670bc(h, step, sub) - 16;
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
        return Calc_020670bc(h, step, sub) - 8;
    }
    return Calc_020670bc(h, step, sub);
}
