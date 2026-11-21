#include <nitro/types.h>

typedef struct UnkStruct_022d6b0c_a {
    u8 pad[0x48];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 padA[0x10c - 0x54];
    u8 unk10C;
    u8 pad1[0x114 - 0x10d];
    s32 unk114;
    u8 pad2[0x170 - 0x118];
} UnkStruct_022d6b0c_a;

typedef struct UnkStruct_022d6b0c UnkStruct_022d6b0c;
struct UnkStruct_022d6b0c {
    u8 pad[0x14];
    s32 unk14;
    u8 padA[0x2e - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad0[0x5c - 0x32];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*func)(UnkStruct_022d6b0c *s);
    u8 pad2[0x258 - 0x6c];
    u16 unk258;
    u16 unk25A;
    u8 pad3[0x264 - 0x25c];
    u16 unk264;
    u8 pad4[0x2c0 - 0x266];
    s32 unk2C0;
    UnkStruct_022d6b0c_a unk2C4;
    void *unk434;
};

void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);

typedef struct UnkStruct_022d6b0c_c {
    u8 pad[0x30];
    u16 unk30;
    u8 pad1[0x6c - 0x32];
    u16 unk6C;
} UnkStruct_022d6b0c_c;

extern UnkStruct_022d6b0c_c data_ov000_0236a48c;

void Boss3_ResetCamera(void);
void Boss3PartCC_State_JumpFallStart(UnkStruct_022d6b0c *s);

void Boss3PartCC_State_JumpRise(UnkStruct_022d6b0c *s) {
    UnkStruct_022d6b0c_c *c;
    s->unk30 += 0x60;
    if (s->unk30 < 0)
        return;
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 8, 0);
    s->unk2C4.unk10C &= ~4;
    s->unk2C4.unk114 = 0x1000;
    c = (UnkStruct_022d6b0c_c *)(u32)&data_ov000_0236a48c;
    Boss3_ResetCamera();
    c->unk30 = 0x199;
    c->unk6C = 0xcc;
    s->func = Boss3PartCC_State_JumpFallStart;
}
