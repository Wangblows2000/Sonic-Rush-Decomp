#include <nitro/types.h>

typedef struct UnkStruct_022f1438_w {
    u8 pad0[0x14];
    s16 unk14;
    u8 pad16[0x13c - 0x16];
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_022f1438_w;

typedef struct UnkStruct_022f1438_p {
    u8 pad0[0x1dc];
    UnkStruct_022f1438_w *work;
} UnkStruct_022f1438_p;

typedef struct UnkStruct_022f1438 {
    u8 pad0[0x18];
    s32 x;
    s32 y;
    s32 z;
    u8 pad24[0x74 - 0x24];
    void (*state)(struct UnkStruct_022f1438 *p);
    u8 pad78[4];
    UnkStruct_022f1438_p *parent;
    u8 pad80[0x1d0 - 0x80];
    s32 fromX;
    s32 fromY;
    s16 t;
} UnkStruct_022f1438;

void ExtraBossObj_State_None(UnkStruct_022f1438 *p);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

static inline s32 Approach(s32 t, s32 target, s32 v) {
    s32 i = 2;
    do {
        v = target + FX_Mul(v - target, t);
    } while (i-- != 0);
    return v;
}

void ExtraBossObj_State_HandCatchReturn(UnkStruct_022f1438 *p) {
    UnkStruct_022f1438_w *w = p->parent->work;

    p->t -= 0x16;
    if (p->t <= 0) {
        p->t = 0;
        p->x = w->x;
        p->y = -w->y;
        p->z = w->z;
        w->unk14 = 0;
        p->state = ExtraBossObj_State_None;
    }
    p->x = Approach(p->t, w->x, p->fromX);
    p->y = Approach(p->t, -w->y, p->fromY);
}
