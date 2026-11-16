#include <nitro/types.h>

typedef struct UnkStruct_02302140_c {
    u8 pad0[4];
    s32 accel;
    s32 decel;
    s32 max;
} UnkStruct_02302140_c;

typedef struct UnkStruct_02302140_w {
    u8 pad0[8];
    UnkStruct_02302140_c *cfg;
} UnkStruct_02302140_w;

typedef struct UnkStruct_02302140_k {
    u8 pad0[0x18];
    u16 held;
} UnkStruct_02302140_k;

typedef struct UnkStruct_02302140 {
    u8 pad0[0x14];
    u32 flags;
    u8 pad18[0x3c - 0x18];
    s16 unk3C;
    u8 pad3E[0x50 - 0x3e];
    s32 x;
    s32 y;
    u8 pad58[4];
    s32 vx;
    s32 vy;
    u8 pad64[0x1dc - 0x64];
    UnkStruct_02302140_w *work;
} UnkStruct_02302140;

UnkStruct_02302140_k *ExtraBoss_GetWork(void);

void func_ov001_02302140(UnkStruct_02302140 *p) {
    UnkStruct_02302140_w *w = p->work;
    u16 held = ExtraBoss_GetWork()->held;
    s32 accel = w->cfg->accel;
    s32 decel = w->cfg->decel;
    s32 max = w->cfg->max;
    s32 s;

    p->vx = p->vy = 0;
    p->flags &= ~0x700;
    if (held & 0x10) {
        p->vx = accel;
    } else if (held & 0x20) {
        p->vx = -accel;
    } else {
        if (p->x > 0)
            p->vx = decel;
        else if (p->x < 0)
            p->vx = -decel;
        p->flags |= 0x100;
    }
    if (held & 0x40) {
        p->vy = -accel;
    } else if (held & 0x80) {
        p->vy = accel;
    } else {
        if (p->y > 0)
            p->vy = decel;
        else if (p->y < 0)
            p->vy = -decel;
        p->flags |= 0x200;
    }
    s = p->vx + p->x;
    if (s > max)
        p->vx = max - p->x;
    else if (s < -max)
        p->vx = -(max + p->x);
    s = p->vy + p->y;
    if (s > max)
        p->vy = max - p->y;
    else if (s < -max)
        p->vy = -(max + p->y);
    p->unk3C = p->y * 0x1555 / w->cfg->max;
}
