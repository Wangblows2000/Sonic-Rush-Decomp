#include <nitro/types.h>

typedef struct UnkStruct_022d0d64_a {
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
} UnkStruct_022d0d64_a;

typedef struct UnkStruct_022d0d64_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x2c - 0xc];
} UnkStruct_022d0d64_h;

typedef struct UnkStruct_022d0d64 UnkStruct_022d0d64;
struct UnkStruct_022d0d64 {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x68 - 0x14];
    void (*func)(UnkStruct_022d0d64 *s);
    u8 pad1[0x20c - 0x6c];
    u32 unk20C;
    u8 pad2[0xa9c - 0x210];
    UnkStruct_022d0d64_a unkA9C;
    UnkStruct_022d0d64_a unkBD4;
    UnkStruct_022d0d64_a unkD0C;
    u8 pad3[0xf7c - 0xe44];
    s32 unkF7C;
    u8 pad3b[0xf84 - 0xf80];
    UnkStruct_022d0d64_h unkF84[10];
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

typedef struct UnkStruct_022d0d64_7 {
    u8 pad[8];
    u16 unk8;
    u16 unkA;
} UnkStruct_022d0d64_7;

extern UnkStruct_022d0d64_7 data_ov000_02369ac0;

s32 func_0200225c(s32 a, s32 b);
s32 func_02002094(s32 a, s32 b);
void func_020366f4(s32 a, s32 b, s32 c);
void func_ov000_022d0c30(UnkStruct_022d0d64 *s);

void func_ov000_022d0d64(UnkStruct_022d0d64 *s) {
    s32 v;
    UnkStruct_022d0d64_a *y;
    s->unk12F4--;
    if (s->unk12F4 > 0)
        return;
    v = s->unk1464;
    s->func = func_ov000_022d0c30;
    y = &s->unkBD4;
    y->unk114 = func_02002094(data_ov000_02369ac0.unk8 * 0x1666 + (data_ov000_02369ac0.unkA - data_ov000_02369ac0.unk8) * 0x3000, data_ov000_02369ac0.unkA);
    if (v < 0)
        v = 0;
    s->unk12F4 = func_02002094(v * 300, 0x51a326);
    if (300 - (func_02002094(data_ov000_02369ac0.unk8 * 8 + (data_ov000_02369ac0.unkA - data_ov000_02369ac0.unk8) * 4, data_ov000_02369ac0.unkA) + (*y->unkE4 >> 12)) < s->unk12F4)
        s->unk12F4 = 300 - (func_02002094(data_ov000_02369ac0.unk8 * 8 + (data_ov000_02369ac0.unkA - data_ov000_02369ac0.unk8) * 4, data_ov000_02369ac0.unkA) + (*y->unkE4 >> 12));
    s->unk12F4 = func_0200225c(s->unk12F4, y->unk114);
    func_020366f4(0, 0, 0xd2);
}
