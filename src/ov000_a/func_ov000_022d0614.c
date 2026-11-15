#include <nitro/types.h>

typedef struct UnkStruct_022d0614_a {
    u8 pad[0xb];
    u8 unkB;
    u8 pad0[0x24 - 0xc];
    s32 unk24[9];
    u8 pad1[0x90 - 0x48];
    u8 unk90[0x10c - 0x90];
    u8 unk10C;
    u8 pad2[0x114 - 0x10d];
    s32 unk114;
    u8 pad3[0x138 - 0x118];
} UnkStruct_022d0614_a;

typedef struct UnkStruct_022d0614_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x2c - 0xc];
} UnkStruct_022d0614_h;

typedef struct UnkStruct_022d0614 UnkStruct_022d0614;
struct UnkStruct_022d0614 {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x68 - 0x14];
    void (*func)(UnkStruct_022d0614 *s);
    u8 pad1[0x20c - 0x6c];
    u32 unk20C;
    u8 pad2[0xa9c - 0x210];
    UnkStruct_022d0614_a unkA9C;
    UnkStruct_022d0614_a unkBD4;
    UnkStruct_022d0614_a unkD0C;
    u8 pad3[0xf7c - 0xe44];
    s32 unkF7C;
    u8 pad3b[0xf84 - 0xf80];
    UnkStruct_022d0614_h unkF84[10];
    u8 pad4[0x1304 - 0x113c];
    u16 unk1304;
    u8 pad5[0x1458 - 0x1306];
    s32 unk1458;
    u16 unk145C;
    u8 pad6[0x1460 - 0x145e];
    s32 unk1460;
    u8 pad7[0x1474 - 0x1464];
    s32 unk1474;
};

void func_02031a68(void *p, s32 a, s32 b, s32 c, s32 d);
void func_ov000_022cd1b0(u32 flags);

void func_02021c24(void *p);
void func_ov000_022d4654(s32 a);

void func_ov000_022d0614(UnkStruct_022d0614 *s) {
    if (!(s->unkA9C.unk10C & 2))
        return;
    func_02021c24(s->unkD0C.unk90);
    s->unk20C &= ~2;
    s->unk1304 = s->unk145C;
    s->unk10 = s->unk1460;
    func_ov000_022d4654(3);
    s->unkF84[8].unkA &= ~4;
    s->unkF84[8].unkA |= 0x1000;
}
