#include <nitro/types.h>

typedef struct UnkStruct_022f1550_w {
    u8 pad0[0x13c];
    s32 x;
    s32 y;
} UnkStruct_022f1550_w;

typedef struct UnkStruct_022f1550_p {
    u8 pad0[0xb8];
    u32 flagsB8;
    u8 padBC[0x1dc - 0xbc];
    UnkStruct_022f1550_w *work;
} UnkStruct_022f1550_p;

typedef struct UnkStruct_022f1550 {
    u8 pad0[0x18];
    s32 x;
    s32 y;
    u8 pad20[0x74 - 0x20];
    void (*state)(struct UnkStruct_022f1550 *p);
    u8 pad78[4];
    UnkStruct_022f1550_p *parent;
    u8 pad80[0x128 - 0x80];
    u32 flags128;
    u8 pad12C[0x1d0 - 0x12c];
    s32 fromX;
    s32 fromY;
    s16 t;
} UnkStruct_022f1550;

void ExtraBossObj_State_HandCatchReturn(UnkStruct_022f1550 *p);

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

void ExtraBossObj_State_HandCatchFlyOut(UnkStruct_022f1550 *p) {
    UnkStruct_022f1550_p *q = p->parent;
    UnkStruct_022f1550_w *w = q->work;

    p->t += 0x5b;
    if (p->t >= 0x1000) {
        p->t = 0x1000;
        p->state = ExtraBossObj_State_HandCatchReturn;
        q->flagsB8 &= ~0x200;
        p->flags128 |= 0x200;
    }
    p->x = Approach(p->t, w->x, p->fromX);
    p->y = Approach(p->t, -w->y, p->fromY);
}
