#include <nitro/types.h>

typedef struct UnkStruct_022d0c30_a {
    u8 pad[0xb];
    u8 unkB;
    u8 pad0[0x24 - 0xc];
    s32 unk24[9];
    u8 pad1[0x90 - 0x48];
    u8 unk90[0xe4 - 0x90];
    s32 *unkE4;
    u8 padE8[0x10c - 0xe8];
    u8 unk10C;
    u8 pad2[0x114 - 0x10d];
    s32 unk114;
    u8 pad3[0x138 - 0x118];
} UnkStruct_022d0c30_a;

typedef struct UnkStruct_022d0c30_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x2c - 0xc];
} UnkStruct_022d0c30_h;

typedef struct UnkStruct_022d0c30 UnkStruct_022d0c30;
struct UnkStruct_022d0c30 {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x68 - 0x14];
    void (*func)(UnkStruct_022d0c30 *s);
    u8 pad1[0x20c - 0x6c];
    u32 unk20C;
    u8 pad2[0xa9c - 0x210];
    UnkStruct_022d0c30_a unkA9C;
    UnkStruct_022d0c30_a unkBD4;
    UnkStruct_022d0c30_a unkD0C;
    u8 pad3[0xf7c - 0xe44];
    s32 unkF7C;
    u8 pad3b[0xf84 - 0xf80];
    UnkStruct_022d0c30_h unkF84[10];
    u8 pad4[0x12f4 - 0x113c];
    s32 unk12F4;
    u8 pad4b[0x1304 - 0x12f8];
    u16 unk1304;
    u8 pad5[0x131c - 0x1306];
    u8 *unk131C;
    u8 pad5b[0x1458 - 0x1320];
    s32 unk1458;
    u16 unk145C;
    u8 pad6[0x1460 - 0x145e];
    s32 unk1460;
    s32 unk1464;
    u8 pad7[0x1474 - 0x1468];
    s32 unk1474;
};

void func_02031a68(void *p, s32 a, s32 b, s32 c, s32 d);
void func_ov000_022cd1b0(u32 flags);

void func_ov000_022d0864(UnkStruct_022d0c30 *s);

void func_ov000_022d0c30(UnkStruct_022d0c30 *s) {
    UnkStruct_022d0c30_a *x;
    UnkStruct_022d0c30_a *y;
    UnkStruct_022d0c30_a *z;
    s->unk12F4--;
    if (s->unk12F4 > 0 && !(s->unkBD4.unk10C & 2))
        return;
    s->func = func_ov000_022d0864;
    x = &s->unkA9C;
    y = &s->unkBD4;
    z = &s->unkD0C;
    if (s->unk145C) {
        func_02031a68(x, 0, s->unkF7C, 0x19, 0);
        func_02031a68(z, 0, s->unkF7C, 0x55, 0);
    } else {
        func_02031a68(x, 0, s->unkF7C, 0x13, 0);
        func_02031a68(z, 0, s->unkF7C, 0x4f, 0);
    }
    x->unk10C &= ~4;
    z->unk10C &= ~4;
    x->unk114 = z->unk114 = 0x1000;
    y->unk114 = 0;
    s->unk12F4 = 0;
    *(u32 *)(s->unk131C + 0x54) |= 4;
}
