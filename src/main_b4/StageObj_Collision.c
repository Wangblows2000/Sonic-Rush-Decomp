#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
    u8 pad18[4];
    s32 px;
    s32 py;
    u8 pad24[0x34];
    u32 flags;
    u8 pad5c[2];
    u16 hit;
    u16 prevHit;
} UnkObj_0206bd5c;

void func_02065f98(UnkObj_0206bd5c *o);

#define ABS_0206bd5c(v) ((v) < 0 ? -(v) : (v))

void StageObj_Collision(UnkObj_0206bd5c *o) {
    s32 savePx;
    s32 savePy;
    s32 tx;
    s32 ty;
    s32 oldy;
    u16 step;
    u16 hit;
    u32 fl;
    s32 oldx;

    savePx = o->px;
    savePy = o->py;
    hit = 0;
    tx = o->x;
    ty = o->y;
    o->prevHit = o->hit;
    o->hit = 0;
    fl = 0;
    step = 0x800;
    if (o->flags & 0x100)
        goto end;
    if (o->flags & 0x200000) {
        if (!(o->flags & 1))
            step >>= 1;
        if (ABS_0206bd5c(o->x - o->px) > step || ABS_0206bd5c(o->y - o->py) > step) {
            o->x = o->px;
            o->y = o->py;
            for (;;) {
                if (ABS_0206bd5c(o->x - tx) > step) {
                    o->px = o->x;
                    if (tx > o->px)
                        o->x = o->px + step;
                    else
                        o->x = o->px - step;
                } else {
                    o->x = tx;
                }
                if (ABS_0206bd5c(o->y - ty) > step) {
                    o->py = o->y;
                    if (ty > o->py)
                        o->y = o->py + step;
                    else
                        o->y = o->py - step;
                } else {
                    o->y = ty;
                }
                oldx = o->x;
                if (oldx == tx && o->y == ty)
                    break;
                oldy = o->y;
                func_02065f98(o);
                if (oldx != o->x)
                    tx = o->x;
                hit |= o->hit;
                fl |= o->flags & 0xf;
                if (oldy != o->y)
                    ty = o->y;
            }
        }
    }
    func_02065f98(o);
    o->hit = hit | o->hit;
    o->flags |= fl;
end:
    o->px = savePx;
    o->py = savePy;
}
