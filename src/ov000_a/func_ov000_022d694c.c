#include <nitro/types.h>

typedef struct UnkStruct_022d694c_a {
    u8 pad[0x10c];
    u8 unk10C;
    u8 pad1[0x114 - 0x10d];
    s32 unk114;
    u8 pad2[0x170 - 0x118];
} UnkStruct_022d694c_a;

typedef struct UnkStruct_022d694c UnkStruct_022d694c;
struct UnkStruct_022d694c {
    u8 pad[0x14];
    s32 unk14;
    u8 padA[0x2e - 0x18];
    s16 unk2E;
    u16 unk30;
    u8 pad0[0x5c - 0x32];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*func)(UnkStruct_022d694c *s);
    u8 pad2[0x258 - 0x6c];
    u16 unk258;
    u16 unk25A;
    u8 pad3[0x264 - 0x25c];
    u16 unk264;
    u8 pad4[0x2c0 - 0x266];
    s32 unk2C0;
    UnkStruct_022d694c_a unk2C4;
    void *unk434;
};

void func_02031a68(void *p, s32 a, void *b, s32 c, s32 d);

void func_ov000_022dac94(u16 *p, s32 a, s32 b);
void func_ov000_022d68f0(UnkStruct_022d694c *s);

void func_ov000_022d694c(UnkStruct_022d694c *s) {
    func_ov000_022dac94(&s->unk264, 2, 6);
    if (!(s->unk2C4.unk10C & 2))
        return;
    if (s->unk264 != 0)
        return;
    s->unk5C ^= 1;
    s->func = func_ov000_022d68f0;
    s->func(s);
}
