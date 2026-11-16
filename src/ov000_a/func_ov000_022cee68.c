#include <nitro/types.h>

typedef struct UnkStruct_022cee68_a {
    u8 pad[0x1c];
    s32 unk1C;
    u8 pad1[0x4c - 0x20];
    s32 unk4C;
    u8 pad2[0x10c - 0x50];
    u8 unk10C[4];
    u8 pad3[0x114 - 0x110];
    s32 unk114;
    u8 pad4[0x138 - 0x118];
} UnkStruct_022cee68_a;

typedef struct UnkStruct_022cee68_s {
    u8 pad[0x5bc];
    UnkStruct_022cee68_a unk5BC;
    u8 pad1[0x82c - 0x5bc - 0x138];
    UnkStruct_022cee68_a unk82C;
    u8 pad2[0xf7c - 0x82c - 0x138];
    s32 unkF7C;
    s32 unkF80;
    u8 pad3[0x1420 - 0xf84];
    UnkStruct_022cee68_a *unk1420;
} UnkStruct_022cee68_s;

typedef struct UnkStruct_022cee68 UnkStruct_022cee68;
struct UnkStruct_022cee68 {
    UnkStruct_022cee68_s *unk0;
    void (*func)(UnkStruct_022cee68 *t);
    s32 unk8;
    s32 unkC;
};

s32 FX_DivS32(s32 a, s32 b);
void Model_SetAnimation(void *p, s32 a, s32 b, s32 c, s32 d);

void func_ov000_022cee04(UnkStruct_022cee68 *t);
void func_ov000_022cd574(void);

void func_ov000_022cee68(UnkStruct_022cee68 *t) {
    UnkStruct_022cee68_a *a = t->unk0->unk1420;
    s32 k;
    t->unk8 += -0x1b33;
    if (t->unk8 < -0x66000)
        t->unk8 = -0x66000;
    k = (t->unk8 >> 12) + 0xd3;
    a->unk1C = FX_DivS32(k * 0x34cc, 0xd3);
    a->unk4C = -(k + 0x12c) << 12;
    t->unkC--;
    if (t->unkC <= 0) {
        UnkStruct_022cee68_s *s = t->unk0;
        UnkStruct_022cee68_a *b;
        t->func = func_ov000_022cee04;
        b = &s->unk82C;
        b->unk1C = 0x34cc;
        Model_SetAnimation(b, 0, s->unkF7C, 2, 0);
        b->unk10C[0] &= ~4;
        Model_SetAnimation(b, 3, s->unkF80, 2, 0);
        b->unk10C[3] &= ~4;
        b->unk114 = 0x1000;
        s->unk1420 = b;
    } else if (t->unkC == 0xf) {
        func_ov000_022cd574();
    }
}
