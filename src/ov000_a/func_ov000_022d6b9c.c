#include <nitro/types.h>

typedef struct UnkStruct_022d6b9c_a {
    u8 pad[0x48];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 padA[0x10c - 0x54];
    u8 unk10C;
    u8 pad1[0x114 - 0x10d];
    s32 unk114;
    u8 pad2[0x170 - 0x118];
} UnkStruct_022d6b9c_a;

typedef struct UnkStruct_022d6b9c UnkStruct_022d6b9c;
struct UnkStruct_022d6b9c {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 padA[0x2e - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad0[0x5c - 0x32];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*func)(UnkStruct_022d6b9c *s);
    u8 pad2[0x258 - 0x6c];
    u16 unk258;
    u16 unk25A;
    u8 pad3[0x264 - 0x25c];
    u16 unk264;
    u8 pad4[0x2c0 - 0x266];
    s32 unk2C0;
    UnkStruct_022d6b9c_a unk2C4;
    void *unk434;
};

void func_02031a68(void *p, s32 a, void *b, s32 c, s32 d);

void func_ov000_022d6b0c(UnkStruct_022d6b9c *s);

void func_ov000_022d6b9c(UnkStruct_022d6b9c *s) {
    if (!(s->unk2C4.unk10C & 2))
        return;
    func_02031a68(&s->unk2C4, 0, s->unk434, 7, 0);
    s->unk2C4.unk10C |= 4;
    s->unk2C4.unk114 = 0x1000;
    s->unk30 = -0xa00;
    s->func = func_ov000_022d6b0c;
}
