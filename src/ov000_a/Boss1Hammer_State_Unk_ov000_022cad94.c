#include <nitro/types.h>

typedef struct UnkStruct_022cad94_3 {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x54 - 0x14];
    u32 unk54;
    u8 pad1[0x220 - 0x58];
    s32 unk220;
    u8 pad1b[4];
    u8 unk228[0x334 - 0x228];
    u8 unk334;
    u8 pad1c[0x33c - 0x335];
    s32 unk33C;
} UnkStruct_022cad94_3;

typedef struct UnkStruct_022cad94_4 {
    u8 pad[0x54];
    u32 unk54;
    u8 pad1[0x216 - 0x58];
    u16 unk216;
    u8 pad2[0x242 - 0x218];
    u16 unk242;
    u8 pad3[0x29a - 0x244];
    u16 unk29A;
    u8 pad4[0x304 - 0x29c];
    s32 unk304;
    u8 pad4b[4];
    u8 unk30C[0x420 - 0x30c];
    s32 unk420;
} UnkStruct_022cad94_4;

typedef struct UnkStruct_022cad94 UnkStruct_022cad94;
struct UnkStruct_022cad94 {
    u8 pad[0x54];
    u32 unk54;
    u8 pad0[0x68 - 0x58];
    void (*func)(UnkStruct_022cad94 *s);
    u8 pad1[0x210 - 0x6c];
    UnkStruct_022cad94_3 *unk210;
    UnkStruct_022cad94_4 *unk214;
    u8 pad2[0x220 - 0x218];
    u16 unk220;
    u16 unk222;
    u8 pad3[0x350 - 0x224];
    s32 unk350;
};

typedef struct UnkStruct_022cad94_pl {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad1[0x3b2 - 0x18];
    u16 unk3B2;
} UnkStruct_022cad94_pl;

typedef struct UnkStruct_022cad94_5 {
    s32 unk0;
    u8 pad[0x18 - 4];
    u32 unk18;
} UnkStruct_022cad94_5;

typedef struct UnkStruct_022cad94_6 {
    u8 pad[4];
    s32 unk4;
} UnkStruct_022cad94_6;

extern UnkStruct_022cad94_5 data_022c4560;

void func_ov000_022cc3e8(s32 a, s32 b);
void func_ov000_022cbfc8(void);

void Boss1Hammer_State_Unk_ov000_022cad94(UnkStruct_022cad94 *s) {
    if (s->unk222 < 0x2d || (s->unk220 & 0x8000)) {
        s->unk222++;
        s->unk220++;
        if (!(s->unk220 & 1)) {
            if (s->unk220 & 0x8000)
                func_ov000_022cc3e8(data_022c4560.unk0, 0);
            else
                func_ov000_022cc3e8(data_022c4560.unk0, 1);
            s->unk220 &= ~1;
            s->unk220 ^= 0x8000;
        }
    }
    if (s->unk210->unk334 & 2)
        func_ov000_022cbfc8();
}
