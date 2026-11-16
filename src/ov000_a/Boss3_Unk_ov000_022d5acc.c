#include <nitro/types.h>

typedef struct UnkStruct_022d5acc_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_022d5acc_v;

typedef struct UnkStruct_022d5acc UnkStruct_022d5acc;
struct UnkStruct_022d5acc {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad0[0x54 - 0x18];
    u32 unk54;
    u8 pad1[0x5c - 0x58];
    u16 unk5C;
    u8 pad2[0x68 - 0x5e];
    void (*func)(UnkStruct_022d5acc *s);
    u8 pad3[0x13e - 0x6c];
    u16 unk13E;
    u8 pad4[0x16a - 0x140];
    u16 unk16A;
    u8 pad5[0x20c - 0x16c];
    u32 unk20C;
    u16 unk210;
    u8 pad6[0x214 - 0x212];
    UnkStruct_022d5acc_v unk214;
    u16 unk220;
    u8 pad7[0x22a - 0x222];
    u16 unk22A;
    u16 unk22C;
    u16 unk22E;
    u8 pad8[0x234 - 0x230];
    u16 unk234;
    u8 pad9[0x2b4 - 0x236];
    u8 unk2B4[0x24];
    UnkStruct_022d5acc_v unk2D8;
};

void Boss3_Unk_ov000_022dad70(s32 a, s32 b);
void Boss3PartCD_State_Unk_ov000_022d5ab0(UnkStruct_022d5acc *s);
void Boss3PartCD_State_Unk_ov000_022d5aa4(UnkStruct_022d5acc *s);
extern s32 gGameState;

void Boss3_Unk_ov000_022d5acc(UnkStruct_022d5acc *s) {
    Boss3_Unk_ov000_022dad70(gGameState, 0);
    s->unk13E &= ~4;
    s->unk16A &= ~4;
    s->unk20C &= ~0x10;
    if (!(s->unk20C & 4))
        s->func = Boss3PartCD_State_Unk_ov000_022d5ab0;
    else
        s->func = Boss3PartCD_State_Unk_ov000_022d5aa4;
}
