#include <nitro/types.h>

typedef struct UnkStruct_022d6568_a {
    u8 pad[0x10c];
    u8 unk10C;
    u8 pad1[0x114 - 0x10d];
    s32 unk114;
    u8 pad2[0x170 - 0x118];
} UnkStruct_022d6568_a;

typedef struct UnkStruct_022d6568 UnkStruct_022d6568;
struct UnkStruct_022d6568 {
    u8 pad[0x2e];
    s16 unk2E;
    u8 pad0[0x5c - 0x30];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*func)(UnkStruct_022d6568 *s);
    u8 pad2[0x258 - 0x6c];
    u16 unk258;
    u16 unk25A;
    u8 pad3[0x264 - 0x25c];
    u16 unk264;
    u8 pad4[0x2c4 - 0x266];
    UnkStruct_022d6568_a unk2C4;
    void *unk434;
};

void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);

extern u32 gRandSeed;

void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void func_020366f4(s32 a, s32 b, s32 c);
void Boss3PartCC_State_Unk_ov000_022d63f4(UnkStruct_022d6568 *s);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void Boss3PartCC_State_Unk_ov000_022d6568(UnkStruct_022d6568 *s) {
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 0x15, 0);
    s->unk2C4.unk10C &= ~4;
    s->unk2C4.unk114 = 0x1000;
    if (s->unk5C & 1)
        s->unk2E = 0x400;
    else
        s->unk2E = -0x400;
    Sound_PlaySeqArc(0, 0, 0x7b);
    func_020366f4(0, 0, (Rand() & 1) ? 0xce : 0xcf);
    s->unk258 = 0;
    s->unk25A = 0;
    s->unk264 = 0x40;
    s->func = Boss3PartCC_State_Unk_ov000_022d63f4;
}
