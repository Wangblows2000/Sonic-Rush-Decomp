#include <nitro/types.h>

typedef struct UnkStruct_022ca314 UnkStruct_022ca314;
struct UnkStruct_022ca314 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad0[0x68 - 0x1c];
    void (*func)(UnkStruct_022ca314 *s);
    u8 pad1[0x230 - 0x6c];
    u8 unk230[0x378 - 0x230];
    s32 unk378;
    s32 unk37C;
    s32 unk380;
    s32 unk384;
    s32 unk388;
    s32 unk38C;
    s16 unk390;
    s16 unk392;
};

extern s16 data_0206e470[];

void MTX_RotX33_(void *m, s32 s, s32 c);
void func_ov000_022ca1ec(UnkStruct_022ca314 *s);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_ov000_022ca314(UnkStruct_022ca314 *s) {
    u16 ang;
    u16 a2;
    s32 sn;
    s->unk392 += s->unk390;
    if (s->unk392 >= 0x1000) {
        s->unk392 = 0x1000;
        s->func = func_ov000_022ca1ec;
    }
    s->unk10 = (s->unk378 + FX_Mul(s->unk384 - s->unk378, s->unk392)) >> 4;
    s->unk14 = -(s->unk37C + FX_Mul(s->unk388 - s->unk37C, s->unk392)) >> 4;
    s->unk18 = (s->unk380 + FX_Mul(s->unk38C - s->unk380, s->unk392)) >> 4;
    ang = s->unk392 << 3;
    sn = data_0206e470[((ang & 0xffff) >> 4) * 2];
    a2 = ang - 0x4000;
    s->unk14 -= (s32)(((s64)sn * 0x12c000 + 0x800) >> 12) >> 4;
    MTX_RotX33_(s->unk230, data_0206e470[((a2 & 0xffff) >> 4) * 2], data_0206e470[((a2 & 0xffff) >> 4) * 2 + 1]);
}
