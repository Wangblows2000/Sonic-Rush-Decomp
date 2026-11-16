#include <nitro/types.h>

typedef struct UnkStruct_022d81ac_ctl2 {
    u8 pad[4];
    u16 unk4;
} UnkStruct_022d81ac_ctl2;

typedef struct UnkStruct_022d81ac_ctl {
    s32 unk0;
    u8 pad[4];
    UnkStruct_022d81ac_ctl2 *unk8;
} UnkStruct_022d81ac_ctl;

typedef struct UnkStruct_022d81ac_a {
    u8 pad[0x24];
    u8 unk24[0x24];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 padA[0xe4 - 0x54];
    struct UnkStruct_022d81ac_ctl *unkE4;
    u8 padB[0x10c - 0xe8];
    u8 unk10C;
    u8 pad1[0x114 - 0x10d];
    s32 unk114;
    u8 pad2[0x12e - 0x118];
    u16 unk12E;
    u8 pad3[0x170 - 0x130];
} UnkStruct_022d81ac_a;

typedef struct UnkStruct_022d81ac UnkStruct_022d81ac;
struct UnkStruct_022d81ac {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 padA[0x2e - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad0[0x5c - 0x32];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*func)(UnkStruct_022d81ac *s);
    u8 pad2[0x13e - 0x6c];
    u16 unk13E;
    u8 pad2a[0x16a - 0x140];
    u16 unk16A;
    u8 pad2c[0x20c - 0x16c];
    s32 unk20C;
    u8 pad2d[0x214 - 0x210];
    s32 unk214;
    s32 unk218;
    s32 unk21C;
    s32 unk220;
    u16 unk224;
    u8 pad2b[0x24c - 0x226];
    s32 unk24C;
    s32 unk250;
    s32 unk254;
    u16 unk258;
    u16 unk25A;
    u8 pad3[0x264 - 0x25c];
    u16 unk264;
    u8 pad4[0x2c0 - 0x266];
    s32 unk2C0;
    UnkStruct_022d81ac_a unk2C4;
    void *unk434;
};

void func_02031a68(void *p, s32 a, void *b, s32 c, s32 d);

typedef struct UnkStruct_022d81ac_b {
    u8 pad[0x20c];
    u32 unk20C;
    u8 pad1[0x222 - 0x210];
    u16 unk222;
} UnkStruct_022d81ac_b;

typedef struct UnkStruct_022d81ac_4 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022d81ac_4;

extern UnkStruct_022d81ac_b *data_ov000_02369b14;
extern UnkStruct_022d81ac_4 data_022c4560;
extern u32 data_0208fe70;
extern s16 data_0206e470[];
extern u16 data_ov000_023631d8[][9];

s32 func_0200225c(s32 a, s32 b);
void func_ov000_022d87c0(UnkStruct_022d81ac *s);
void func_ov000_022d8890(UnkStruct_022d81ac *s);
void func_ov000_022d8398(UnkStruct_022d81ac *s);

static inline u16 Rand(void) {
    data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
    return (u16)(data_0208fe70 >> 16);
}

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

static inline s32 GetIdx(void) {
    BOOL b = TRUE;
    if (data_022c4560.unk10 != 2 && data_022c4560.unk10 != 3)
        b = FALSE;
    if (b)
        return 1;
    return data_022c4560.unk14;
}

void func_ov000_022d81ac(UnkStruct_022d81ac *s) {
    UnkStruct_022d81ac_b *b = data_ov000_02369b14;
    u16 r;
    s32 m;
    s16 d;
    if (s->unk25A & 1) {
        r = Rand();
        m = FX_Mul(func_0200225c((0x3c - s->unk25A) << 12, 0x3c000), 0x8000);
        s->unk20C = FX_Mul(m, data_0206e470[((r & 0xffff) >> 4) * 2 + 1]);
        s->unk214 = FX_Mul(m, data_0206e470[((r & 0xffff) >> 4) * 2]);
    }
    if (s->unk25A++ <= 0x3c)
        return;
    d = data_ov000_023631d8[GetIdx()][data_ov000_02369b14->unk222] - s->unk258;
    s->unk2C4.unk10C &= ~1;
    if (d > 0x78) {
        if (b->unk20C & 8)
            s->func = func_ov000_022d87c0;
        else
            s->func = func_ov000_022d8890;
    } else {
        s->func = func_ov000_022d8398;
    }
}
