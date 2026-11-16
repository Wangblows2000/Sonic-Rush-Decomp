#include <nitro/types.h>

typedef struct UnkStruct_022d8408_ctl2 {
    u8 pad[4];
    u16 unk4;
} UnkStruct_022d8408_ctl2;

typedef struct UnkStruct_022d8408_ctl {
    s32 unk0;
    u8 pad[4];
    UnkStruct_022d8408_ctl2 *unk8;
} UnkStruct_022d8408_ctl;

typedef struct UnkStruct_022d8408_a {
    u8 pad[0x24];
    u8 unk24[0x24];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 padA[0xe4 - 0x54];
    struct UnkStruct_022d8408_ctl *unkE4;
    u8 padB[0x10c - 0xe8];
    u8 unk10C;
    u8 pad1[0x114 - 0x10d];
    s32 unk114;
    u8 pad2[0x12e - 0x118];
    u16 unk12E;
    u8 pad3[0x170 - 0x130];
} UnkStruct_022d8408_a;

typedef struct UnkStruct_022d8408 UnkStruct_022d8408;
struct UnkStruct_022d8408 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 padA[0x2e - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad0[0x5c - 0x32];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*func)(UnkStruct_022d8408 *s);
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
    u16 unk25C;
    u8 pad3[0x264 - 0x25e];
    u16 unk264;
    u8 pad4[0x2c0 - 0x266];
    s32 unk2C0;
    UnkStruct_022d8408_a unk2C4;
    void *unk434;
};

void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);

typedef struct UnkStruct_022d8408_r {
    u8 pad[0x210];
    u16 unk210;
} UnkStruct_022d8408_r;

typedef struct UnkStruct_022d8408_b {
    u8 pad[0x218];
    UnkStruct_022d8408_r *unk218;
    u8 pad1[0x222 - 0x21c];
    u16 unk222;
} UnkStruct_022d8408_b;

typedef struct UnkStruct_022d8408_4 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022d8408_4;

extern UnkStruct_022d8408_b *data_ov000_02369b14;
extern UnkStruct_022d8408_4 data_022c4560;
extern s16 data_ov000_023631fc[][9];
extern u16 data_ov000_023631d8[][9];
extern u8 data_ov000_023633e4[][9][16];

void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void func_ov000_022dac94(u16 *p, s32 a, s32 b);
void func_ov000_022daa78(void);
void func_ov000_022daa94(void);
void func_ov000_022d8398(UnkStruct_022d8408 *s);

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

static inline s32 GetIdx2(void) {
    switch (data_022c4560.unk10) {
    case 2:
    case 3:
        return 1;
    }
    return data_022c4560.unk14;
}

#define MAXSPD data_ov000_023631fc[GetIdx()][data_ov000_02369b14->unk222]
#define MAXSPD2 data_ov000_023631fc[GetIdx2()][data_ov000_02369b14->unk222]

void func_ov000_022d8408(UnkStruct_022d8408 *s) {
    if (s->unk5C & 1) {
        if (s->unk2E < 0x100)
            s->unk2E = 0x100;
        else
            s->unk2E += 8;
    } else {
        if (s->unk2E > -0x100)
            s->unk2E = -0x100;
        else
            s->unk2E -= 8;
    }
    s->unk2E = (s->unk2E < -MAXSPD) ? -MAXSPD2 : ((s->unk2E > MAXSPD2) ? MAXSPD2 : s->unk2E);
    s->unk2C4.unk114 = FX_Mul((s->unk2E << 4) < 0 ? -(s->unk2E << 4) : (s->unk2E << 4), 0x666);
    data_ov000_02369b14->unk218->unk210 -= s->unk2E;
    s->unk25C += (u16)(s->unk2E < 0 ? -s->unk2E : s->unk2E);
    if (s->unk25C > 0x6000) {
        s->unk25C -= 0x6000;
        Sound_PlaySeqArc(0, 0, 0x8e);
        s->unk264 = 0x20;
    }
    func_ov000_022dac94(&s->unk264, 1, 5);
    {
        u8 k = data_ov000_023633e4[GetIdx()][data_ov000_02369b14->unk222][s->unk258 >> 5];
        if (s->unk25A != k) {
            s->unk25A = k;
            if (s->unk25A != 0)
                func_ov000_022daa78();
            else
                func_ov000_022daa94();
        }
    }
    if (data_ov000_023631d8[GetIdx()][data_ov000_02369b14->unk222] < s->unk258++)
        s->func = func_ov000_022d8398;
}
