#include <nitro/types.h>

typedef struct UnkStruct_022fb074_o {
    u8 pad0[0x1c];
    s32 unk1C;
} UnkStruct_022fb074_o;

typedef struct UnkStruct_022fb074 {
    u8 pad0[0x14];
    u32 flags;
    s32 x;
    s32 y;
    u8 pad20[0x84 - 0x20];
    UnkStruct_022fb074_o *unk84;
    u8 pad88[0x104 - 0x88];
    s32 unk104;
    u8 pad108[0x1d0 - 0x108];
    s16 t;
} UnkStruct_022fb074;

BOOL ExtraBossObj_IsOutOfArena(s32 x, s32 y, s32 w, s32 h);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

static inline s32 Lerp(s32 t, s32 from, s32 to) {
    s32 v;
    s32 i;
    i = 1;
    v = from;
    do {
        v += FX_Mul(to - v, t);
    } while (i-- != 0);
    return v;
}

void func_ov001_022fb074(UnkStruct_022fb074 *p) {
    if (p->t < 0x1000) {
        p->t += 0x333;
        p->unk84->unk1C = Lerp(p->t, 0x34cc, 0x6998);
        p->unk104 = Lerp(p->t, 0x8000, 0x10000);
    }
    if (ExtraBossObj_IsOutOfArena(p->x, p->y, 0x30000, 0x30000))
        p->flags |= 8;
}
