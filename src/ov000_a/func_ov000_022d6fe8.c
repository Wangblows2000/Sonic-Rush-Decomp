#include <nitro/types.h>

typedef struct UnkStruct_022d6fe8_ctl2 {
    u8 pad[4];
    u16 unk4;
} UnkStruct_022d6fe8_ctl2;

typedef struct UnkStruct_022d6fe8_ctl {
    s32 unk0;
    u8 pad[4];
    UnkStruct_022d6fe8_ctl2 *unk8;
} UnkStruct_022d6fe8_ctl;

typedef struct UnkStruct_022d6fe8_a {
    u8 pad[0x48];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 padA[0xe4 - 0x54];
    struct UnkStruct_022d6fe8_ctl *unkE4;
    u8 padB[0x10c - 0xe8];
    u8 unk10C;
    u8 pad1[0x114 - 0x10d];
    s32 unk114;
    u8 pad2[0x170 - 0x118];
} UnkStruct_022d6fe8_a;

typedef struct UnkStruct_022d6fe8 UnkStruct_022d6fe8;
struct UnkStruct_022d6fe8 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 padA[0x2e - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad0[0x5c - 0x32];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*func)(UnkStruct_022d6fe8 *s);
    u8 pad2[0x258 - 0x6c];
    u16 unk258;
    u16 unk25A;
    u8 pad3[0x264 - 0x25c];
    u16 unk264;
    u8 pad4[0x2c0 - 0x266];
    s32 unk2C0;
    UnkStruct_022d6fe8_a unk2C4;
    void *unk434;
};

void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);

BOOL func_ov000_022d8c64(UnkStruct_022d6fe8 *s);
void func_ov000_022d66cc(UnkStruct_022d6fe8 *s);
void func_ov000_022d6f20(UnkStruct_022d6fe8 *s);

void func_ov000_022d6fe8(UnkStruct_022d6fe8 *s) {
    if (func_ov000_022d8c64(s)) {
        s->func = func_ov000_022d66cc;
        return;
    }
    if (s->unk2C4.unk10C & 2)
        s->func = func_ov000_022d6f20;
}
