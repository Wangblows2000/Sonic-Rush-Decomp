#include <nitro/types.h>

typedef struct UnkStruct_022cac48_3 {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x54 - 0x14];
    u32 unk54;
    u8 pad1[0x33c - 0x58];
    s32 unk33C;
} UnkStruct_022cac48_3;

typedef struct UnkStruct_022cac48_4 {
    u8 pad[0x54];
    u32 unk54;
    u8 pad1[0x216 - 0x58];
    u16 unk216;
    u8 pad2[0x242 - 0x218];
    u16 unk242;
    u8 pad3[0x29a - 0x244];
    u16 unk29A;
    u8 pad4[0x420 - 0x29c];
    s32 unk420;
} UnkStruct_022cac48_4;

typedef struct UnkStruct_022cac48 UnkStruct_022cac48;
struct UnkStruct_022cac48 {
    u8 pad[0x54];
    u32 unk54;
    u8 pad0[0x68 - 0x58];
    void (*func)(UnkStruct_022cac48 *s);
    u8 pad1[0x210 - 0x6c];
    UnkStruct_022cac48_3 *unk210;
    UnkStruct_022cac48_4 *unk214;
    u8 pad3[0x350 - 0x218];
    s32 unk350;
};

typedef struct UnkStruct_022cac48_pl {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad1[0x3b2 - 0x18];
    u16 unk3B2;
} UnkStruct_022cac48_pl;

typedef struct UnkStruct_022cac48_5 {
    s32 unk0;
    u8 pad[0x18 - 4];
    u32 unk18;
} UnkStruct_022cac48_5;

typedef struct UnkStruct_022cac48_6 {
    u8 pad[4];
    s32 unk4;
} UnkStruct_022cac48_6;

extern UnkStruct_022cac48_6 data_ov000_02369ac0;
extern UnkStruct_022cac48_5 data_022c4560;
extern UnkStruct_022cac48_pl *data_ov000_0236ac60;

void func_020446b0(s32 a);
void func_0206c7d8(void);
void func_ov000_022cc35c(s32 a, s32 b);
void func_ov000_02347c44(UnkStruct_022cac48_pl *p);
void func_ov000_0234c4a4(UnkStruct_022cac48_pl *p, s32 a);
u32 func_ov000_0235b0a8(void);
void func_ov000_0235b08c(u32 a);
void func_0201bfbc(s32 a);
void func_ov000_022cab48(UnkStruct_022cac48 *s);

void func_ov000_022cac48(UnkStruct_022cac48 *s) {
    UnkStruct_022cac48_3 *p = s->unk210;
    data_ov000_02369ac0.unk4 = 6;
    s->unk214->unk216 &= ~4;
    s->unk214->unk29A &= ~4;
    s->unk214->unk242 &= ~4;
    func_020446b0(0);
    s->unk54 |= 0x20;
    s->unk210->unk54 |= 0x20;
    s->unk214->unk54 |= 0x20;
    s->unk350 = 0;
    s->unk210->unk33C = 0;
    s->unk214->unk420 = 0;
    func_0206c7d8();
    data_022c4560.unk18 &= ~1;
    p->unk10 = 0xe100;
    func_ov000_022cc35c(data_022c4560.unk0, 0);
    func_ov000_02347c44(data_ov000_0236ac60);
    func_ov000_0234c4a4(data_ov000_0236ac60, 0x30);
    data_ov000_0236ac60->unk3B2 = 0;
    data_ov000_0236ac60->unk10 = 0xe100;
    data_ov000_0236ac60->unk14 = 0x1a800;
    func_ov000_0235b08c(func_ov000_0235b0a8() | 1);
    func_0201bfbc(0);
    s->func = func_ov000_022cab48;
}
