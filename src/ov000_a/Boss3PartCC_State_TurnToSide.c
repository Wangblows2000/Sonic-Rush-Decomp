#include <nitro/types.h>

typedef struct UnkStruct_022d7228_ctl2 {
    u8 pad[4];
    u16 unk4;
} UnkStruct_022d7228_ctl2;

typedef struct UnkStruct_022d7228_ctl {
    s32 unk0;
    u8 pad[4];
    UnkStruct_022d7228_ctl2 *unk8;
} UnkStruct_022d7228_ctl;

typedef struct UnkStruct_022d7228_a {
    u8 pad[0x24];
    u8 unk24[0x24];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 padA[0xe4 - 0x54];
    struct UnkStruct_022d7228_ctl *unkE4;
    u8 padB[0x10c - 0xe8];
    u8 unk10C;
    u8 pad1[0x114 - 0x10d];
    s32 unk114;
    u8 pad2[0x170 - 0x118];
} UnkStruct_022d7228_a;

typedef struct UnkStruct_022d7228 UnkStruct_022d7228;
struct UnkStruct_022d7228 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 padA[0x2e - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad0[0x5c - 0x32];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*func)(UnkStruct_022d7228 *s);
    u8 pad2[0x224 - 0x6c];
    u16 unk224;
    u8 pad2b[0x258 - 0x226];
    u16 unk258;
    u16 unk25A;
    u8 pad3[0x264 - 0x25c];
    u16 unk264;
    u8 pad4[0x2c0 - 0x266];
    s32 unk2C0;
    UnkStruct_022d7228_a unk2C4;
    void *unk434;
};

void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);

typedef struct UnkStruct_022d7228_b {
    u8 pad[0x20c];
    u32 unk20C;
} UnkStruct_022d7228_b;

typedef struct UnkStruct_022d7228_c {
    u8 pad[0x78];
    u16 unk78;
} UnkStruct_022d7228_c;

extern UnkStruct_022d7228_b *gBoss3;
extern UnkStruct_022d7228_c data_ov000_0236a444;
extern s16 data_0206e470[];

u16 Boss3_StepAngleToward(u16 a, u16 b, s32 c);
void MTX_RotY33_(void *m, s32 s, s32 c);
void Boss3PartCC_State_ReturnDone(UnkStruct_022d7228 *s);

void Boss3PartCC_State_TurnToSide(UnkStruct_022d7228 *s) {
    s->unk224 = Boss3_StepAngleToward(s->unk224, s->unk258, 0x200);
    MTX_RotY33_(s->unk2C4.unk24, data_0206e470[((s->unk224 & 0xffff) >> 4) * 2], data_0206e470[((s->unk224 & 0xffff) >> 4) * 2 + 1]);
    if (s->unk224 != s->unk258)
        return;
    gBoss3->unk20C &= ~2;
    data_ov000_0236a444.unk78 = 0x200;
    s->func = Boss3PartCC_State_ReturnDone;
}
