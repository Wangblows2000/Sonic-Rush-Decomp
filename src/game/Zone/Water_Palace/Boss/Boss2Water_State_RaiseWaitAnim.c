#include <nitro/types.h>

typedef struct UnkStruct_022cefa0_a {
    u8 pad[0x1c];
    s32 unk1C;
    u8 pad1[0x4c - 0x20];
    s32 unk4C;
    u8 pad2[0x10c - 0x50];
    u8 unk10C[4];
    u8 pad3[0x114 - 0x110];
    s32 unk114;
    u8 pad4[0x138 - 0x118];
} UnkStruct_022cefa0_a;

typedef struct UnkStruct_022cefa0_s {
    u8 pad[0x5bc];
    UnkStruct_022cefa0_a unk5BC;
    u8 pad1[0x82c - 0x5bc - 0x138];
    UnkStruct_022cefa0_a unk82C;
    u8 pad2[0xf7c - 0x82c - 0x138];
    s32 unkF7C;
    s32 unkF80;
    u8 pad3[0x1420 - 0xf84];
    UnkStruct_022cefa0_a *unk1420;
} UnkStruct_022cefa0_s;

typedef struct UnkStruct_022cefa0 UnkStruct_022cefa0;
struct UnkStruct_022cefa0 {
    UnkStruct_022cefa0_s *unk0;
    void (*func)(UnkStruct_022cefa0 *t);
    s32 unk8;
    s32 unkC;
};

s32 FX_DivS32(s32 a, s32 b);
void Model_SetAnimation(void *p, s32 a, s32 b, s32 c, s32 d);

void Boss2Water_State_Raise(UnkStruct_022cefa0 *t);

void Boss2Water_State_RaiseWaitAnim(UnkStruct_022cefa0 *t) {
    UnkStruct_022cefa0_s *s = t->unk0;
    UnkStruct_022cefa0_a *a = s->unk1420;
    UnkStruct_022cefa0_a *b;
    s32 k;
    if (!(a->unk10C[0] & 2))
        return;
    if (!(a->unk10C[3] & 2))
        return;
    t->func = Boss2Water_State_Raise;
    b = &s->unk5BC;
    Model_SetAnimation(b, 3, s->unkF80, 0, 0);
    b->unk10C[3] |= 4;
    b->unk114 = 0x1000;
    k = (t->unk8 >> 12) + 0xd3;
    b->unk1C = FX_DivS32(k * 0x34cc, 0xd3);
    b->unk4C = -(k + 0x12c) << 12;
    s->unk1420 = b;
    t->unkC = 0x78;
}
