#include <nitro/types.h>

typedef struct UnkStruct_022d43a8_a {
    u8 pad[0xb];
    u8 unkB;
    u8 pad0[0x24 - 0xc];
    s32 unk24[9];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad1[0x90 - 0x54];
    u8 unk90[0xe4 - 0x90];
    s32 *unkE4;
    u8 padE8[0xf8 - 0xe8];
    s32 *unkF8;
    u8 padFC[0x10c - 0xfc];
    u8 unk10C;
    u8 pad2[0x114 - 0x10d];
    s32 unk114;
    u8 pad3[0x12c - 0x118];
    s16 unk12C;
    u8 pad4[0x138 - 0x12e];
} UnkStruct_022d43a8_a;

typedef struct UnkStruct_022d43a8_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x2c - 0xc];
} UnkStruct_022d43a8_h;

typedef struct UnkStruct_022d43a8_q {
    u8 pad[8];
    u16 unk8;
    u8 unkA;
    u8 unkB;
} UnkStruct_022d43a8_q;

typedef struct UnkStruct_022d43a8_o {
    u8 pad[0x54];
    u32 unk54;
    u8 pad0[0x5c - 0x58];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void *unk68;
    u8 pad2[0x134 - 0x6c];
    UnkStruct_022d43a8_q unk134;
} UnkStruct_022d43a8_o;

typedef struct UnkStruct_022d43a8 UnkStruct_022d43a8;
struct UnkStruct_022d43a8 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad0[0x5c - 0x18];
    u16 unk5C;
    u8 pad0b[0x68 - 0x5e];
    void (*func)(UnkStruct_022d43a8 *s);
    u8 pad1[0x20c - 0x6c];
    u32 unk20C;
    u8 pad2[0xa9c - 0x210];
    UnkStruct_022d43a8_a unkA9C;
    UnkStruct_022d43a8_a unkBD4;
    UnkStruct_022d43a8_a unkD0C;
    u8 pad3[0xf7c - 0xe44];
    s32 unkF7C;
    u8 pad3b[0xf84 - 0xf80];
    UnkStruct_022d43a8_h unkF84[10];
    u8 pad4[0x12f4 - 0x113c];
    s32 unk12F4;
    s32 unk12F8;
    s16 unk12FC;
    s16 unk12FE;
    u16 unk1300;
    u16 unk1302;
    u16 unk1304;
    u8 pad5[0x131c - 0x1306];
    UnkStruct_022d43a8_o *unk131C;
    u8 pad5b[0x139c - 0x1320];
    s32 unk139C;
    s32 unk13A0;
    s32 unk13A4;
    u8 pad5c[0x1450 - 0x13a8];
    s32 unk1450;
    u8 pad5d[0x1458 - 0x1454];
    s32 unk1458;
    s32 unk145C;
    s32 unk1460;
    s32 unk1464;
    s32 unk1468;
    s16 unk146C;
    u8 pad7[0x1470 - 0x146e];
    s32 unk1470;
    s32 unk1474;
};

void func_02031a68(void *p, s32 a, s32 b, s32 c, s32 d);
void func_ov000_022cd1b0(u32 flags);

void func_ov000_022d4378(UnkStruct_022d43a8 *s);

void func_ov000_022d43a8(UnkStruct_022d43a8 *s) {
    s->unk12F4--;
    if (s->unk12F4 != 0)
        return;
    s->func = func_ov000_022d4378;
    s->unkA9C.unk10C &= ~4;
    s->unkBD4.unk10C &= ~4;
    s->unkD0C.unk10C &= ~4;
}
