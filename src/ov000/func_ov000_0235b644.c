#include <nitro/types.h>

typedef struct UnkStruct_0235b644 {
    s32 x;
    s32 y;
    s16 vx;
    s16 vy;
    u8 pad[6];
    u16 flags;
} UnkStruct_0235b644;

s32 func_02066a9c(s32 x, s32 y, u8 flag, s32 dir, s32 a, s32 b);

void func_ov000_0235b644(UnkStruct_0235b644 *s) {
    s32 r = 0;
    u8 flag;
    s16 vy = s->vy;

    if (s->flags & 0x800)
        return;
    flag = (s->flags & 0x10) ? 1 : 0;

    if (vy > 0) {
        r = func_02066a9c(s->x >> 8, s->y >> 8, flag, 2, 0, 0);
        if (r < 0) {
            if (s->flags & 0x8000)
                s->y -= r << 8;
            else
                s->y += r << 8;
        }
    } else if (vy < 0) {
        r = func_02066a9c(s->x >> 8, s->y >> 8, flag, 3, 0, 0);
        if (r < 0) {
            if (s->flags & 0x8000)
                s->y += r << 8;
            else
                s->y -= r << 8;
        }
    }
    if (r < 0) {
        s->vy -= s->vy >> 2;
        s->vy = -s->vy;
    }

    r = 0;
    if (s->vx > 0) {
        r = func_02066a9c(s->x >> 8, s->y >> 8, flag, 0, 0, 0);
        if (r < 0)
            s->x += r << 8;
    } else if (s->vx < 0) {
        r = func_02066a9c(s->x >> 8, s->y >> 8, flag, 1, 0, 0);
        if (r < 0)
            s->x -= r << 8;
    }
    if (r >= 0)
        return;
    s->vx -= s->vx >> 2;
    s->vx = -s->vx;
}
