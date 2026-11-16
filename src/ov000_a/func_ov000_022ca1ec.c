#include <nitro/types.h>

typedef struct UnkStruct_022ca1ec_2 {
    u8 pad[0xa];
    u8 unkA;
    u8 unkB;
    u8 unkC;
    u8 pad1[0x18 - 0xd];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
} UnkStruct_022ca1ec_2;

typedef struct UnkStruct_022ca1ec UnkStruct_022ca1ec;
struct UnkStruct_022ca1ec {
    u8 pad[0xc];
    s32 unkC;
    u8 pad0[0x68 - 0x10];
    void (*func)(UnkStruct_022ca1ec *s);
    u8 pad1[0x16a - 0x6c];
    u16 unk16A;
    u8 pad2[0x20c - 0x16c];
    UnkStruct_022ca1ec_2 unk20C;
    u8 pad3[0x398 - 0x20c - sizeof(UnkStruct_022ca1ec_2)];
    s32 unk398;
    s32 unk39C;
    s32 unk3A0;
    s32 unk3A4;
    s32 unk3A8;
};

void Model_Release(UnkStruct_022ca1ec_2 **p);
void Model_Init(UnkStruct_022ca1ec_2 *p, s32 a);
void Model_SetResource(UnkStruct_022ca1ec_2 *p, s32 a, s32 b);
void Model_SetAnimation(UnkStruct_022ca1ec_2 *p, s32 a, s32 b, s32 c, s32 d);
s32 func_02025e8c(s32 a);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void func_ov000_022ca1a8(UnkStruct_022ca1ec *s);

void func_ov000_022ca1ec(UnkStruct_022ca1ec *s) {
    UnkStruct_022ca1ec_2 *a = &s->unk20C;
    Model_Release(&a);
    Model_Init(a, 0);
    Model_SetResource(a, s->unk398, 0);
    Model_SetAnimation(a, 0, s->unk39C, 0, 0);
    Model_SetAnimation(a, 1, s->unk3A0, 0, 0);
    Model_SetAnimation(a, 3, s->unk3A4, 0, 0);
    Model_SetAnimation(a, 2, s->unk3A8, 0, func_02025e8c(s->unk398));
    a->unkA = 0x14;
    a->unkB = 3;
    a->unkC = 0;
    a->unk18 = a->unk1C = a->unk20 = 0x34cc;
    Sound_PlaySeqArc(0, 0, 0x9d);
    s->unkC = 0;
    s->unk16A |= 4;
    s->func = func_ov000_022ca1a8;
}
