#include <nitro/types.h>

typedef struct {
    u8 pad0[0x3C4];
    s32 unk3C4;
    s32 unk3C8;
    u8 pad3CC[0x424 - 0x3CC];
    s32 unk424;
    s32 unk428;
    u8 pad42C[0x438 - 0x42C];
    u16 unk438;
    u16 unk43A;
    s32 unk43C;
} UnkStruct_022f8df8;

extern s16 data_0206e470[];

s32 func_020027a4(s32 y, s32 x);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_ov000_022f8df8(UnkStruct_022f8df8 *s, s32 *x, s32 *y, u8 *z) {
    u16 a;
    u16 ang;
    u16 b;
    s32 idx;
    s32 px;
    s32 py;

    a = func_020027a4(s->unk3C8 - s->unk428, s->unk3C4 - s->unk424) + 0x8000;
    ang = a - (s->unk438 - s->unk43A);
    idx = (s32)(ang & 0xffff) >> 4;
    px = s->unk3C4 + FX_Mul(data_0206e470[(u32)((idx << 1) + 1)], s->unk43C);
    py = s->unk3C8 + FX_Mul(data_0206e470[(u32)(idx << 1)], s->unk43C);
    b = s->unk43A - ang;
    if (x != NULL)
        *x = px >> 4;
    if (y != NULL)
        *y = -py >> 4;
    if (z != NULL)
        *z = b >> 8;
}
