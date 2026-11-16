#include <nitro/types.h>

typedef struct UnkStruct_022d73cc_ctl2 {
    u8 pad[4];
    u16 unk4;
} UnkStruct_022d73cc_ctl2;

typedef struct UnkStruct_022d73cc_ctl {
    s32 unk0;
    u8 pad[4];
    UnkStruct_022d73cc_ctl2 *unk8;
} UnkStruct_022d73cc_ctl;

typedef struct UnkStruct_022d73cc_a {
    u8 pad[0x24];
    u8 unk24[0x24];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 padA[0xe4 - 0x54];
    struct UnkStruct_022d73cc_ctl *unkE4;
    u8 padB[0x10c - 0xe8];
    u8 unk10C;
    u8 pad1[0x114 - 0x10d];
    s32 unk114;
    u8 pad2[0x170 - 0x118];
} UnkStruct_022d73cc_a;

typedef struct UnkStruct_022d73cc UnkStruct_022d73cc;
struct UnkStruct_022d73cc {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 padA[0x2e - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad0[0x5c - 0x32];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*func)(UnkStruct_022d73cc *s);
    u8 pad2[0x13e - 0x6c];
    u16 unk13E;
    u8 pad2a[0x16a - 0x140];
    u16 unk16A;
    u8 pad2c[0x218 - 0x16c];
    s32 unk218;
    s32 unk21C;
    s32 unk220;
    u16 unk224;
    u8 pad2b[0x258 - 0x226];
    u16 unk258;
    u16 unk25A;
    u8 pad3[0x264 - 0x25c];
    u16 unk264;
    u8 pad4[0x2c0 - 0x266];
    s32 unk2C0;
    UnkStruct_022d73cc_a unk2C4;
    void *unk434;
};

void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);

void func_ov000_022c5e1c(s32 a, s32 b, s32 x, s32 z, s32 *out);
void func_ov000_022daa78(void);
void BossEffect_CreateJump(s32 a, s32 b, s32 c, s32 d);
void func_0201c070(s32 a, s32 b);
void func_0201bf1c(s32 a);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void func_ov000_022d7384(UnkStruct_022d73cc *s);

void func_ov000_022d73cc(UnkStruct_022d73cc *s) {
    func_ov000_022c5e1c(0xf42f8, 0x5fe435, s->unk218, s->unk220, &s->unk10);
    s->unk10 >>= 4;
    s->unk13E |= 4;
    s->unk16A |= 4;
    func_ov000_022daa78();
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 0x11, 0);
    s->unk2C4.unk10C &= ~4;
    s->unk2C4.unk114 = 0x1000;
    BossEffect_CreateJump(1, s->unk2C4.unk48 >> 4, -s->unk2C4.unk4C >> 4, s->unk2C4.unk50 >> 4);
    func_0201c070(s->unk2C0, 0x20);
    func_0201bf1c(s->unk2C0);
    Sound_PlaySeqArc(0, 0, 0x90);
    s->unk264 = 0x40;
    s->func = func_ov000_022d7384;
}
