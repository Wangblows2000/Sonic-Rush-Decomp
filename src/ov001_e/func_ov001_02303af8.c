#include <nitro/types.h>

typedef struct UnkStruct_02303af8_n {
    s32 x;
    s32 y;
    s32 ax;
    s32 ay;
    s32 vx;
    s32 vy;
    s32 unk18;
    s32 unk1C;
    u8 pad20[8];
    struct UnkStruct_02303af8_n *next;
} UnkStruct_02303af8_n;

typedef struct UnkStruct_02303af8 {
    u8 pad0[0x90];
    void (*func)(void);
    u8 pad94[0xb0 - 0x94];
    UnkStruct_02303af8_n *listB0;
} UnkStruct_02303af8;

extern UnkStruct_02303af8 *data_ov001_0231f548;
extern u32 gRandSeed;
extern s16 data_0206e470[];

void func_ov001_02303934(void);
void func_ov001_0230319c(UnkStruct_02303af8_n *n);
void func_ov001_0230328c(UnkStruct_02303af8_n *n);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return gRandSeed >> 16;
}

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_ov001_02303af8(void) {
    UnkStruct_02303af8_n *n;
    UnkStruct_02303af8_n *next;
    u16 ang;

    data_ov001_0231f548->func = func_ov001_02303934;
    next = data_ov001_0231f548->listB0;
    while (next != NULL) {
        n = next;
        next = n->next;
        ang = (Rand() & 0x3fff) + 0xa000;
        n->ax = 0;
        n->ay = 0x100;
        n->vx = FX_Mul(data_0206e470[((ang & 0xffff) >> 4) * 2 + 1], 0x2000);
        n->vy = FX_Mul(data_0206e470[((ang & 0xffff) >> 4) * 2], 0x2000);
        n->unk18 = 2;
        n->unk1C = 1;
        func_ov001_0230319c(n);
        func_ov001_0230328c(n);
    }
}
