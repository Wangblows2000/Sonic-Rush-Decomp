#include <nitro/types.h>

typedef struct UnkStruct_022d6644_a {
    u8 pad[0x10c];
    u8 unk10C;
    u8 pad1[0x114 - 0x10d];
    s32 unk114;
    u8 pad2[0x170 - 0x118];
} UnkStruct_022d6644_a;

typedef struct UnkStruct_022d6644 UnkStruct_022d6644;
struct UnkStruct_022d6644 {
    u8 pad[0x14];
    s32 unk14;
    u8 padA[0x2e - 0x18];
    s16 unk2E;
    u16 unk30;
    u8 pad0[0x5c - 0x32];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*func)(UnkStruct_022d6644 *s);
    u8 pad2[0x258 - 0x6c];
    u16 unk258;
    u16 unk25A;
    u8 pad3[0x264 - 0x25c];
    u16 unk264;
    u8 pad4[0x2c0 - 0x266];
    s32 unk2C0;
    UnkStruct_022d6644_a unk2C4;
    void *unk434;
};

void func_02031a68(void *p, s32 a, void *b, s32 c, s32 d);

void func_0201c070(s32 a, s32 b);
void func_0201bf1c(s32 a);
void func_ov000_022d6568(UnkStruct_022d6644 *s);

void func_ov000_022d6644(UnkStruct_022d6644 *s) {
    s->unk30 = 0;
    s->unk14 = 0x20000;
    func_0201c070(s->unk2C0, 0x20);
    func_0201bf1c(s->unk2C0);
    s->func = func_ov000_022d6568;
}
