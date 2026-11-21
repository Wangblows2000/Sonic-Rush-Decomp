#include <nitro/types.h>

typedef struct UnkStruct_022d5cc4_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_022d5cc4_v;

typedef struct UnkStruct_022d5cc4 UnkStruct_022d5cc4;
struct UnkStruct_022d5cc4 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad0[0x54 - 0x18];
    u32 unk54;
    u8 pad1[0x5c - 0x58];
    u16 unk5C;
    u8 pad2[0x68 - 0x5e];
    void (*func)(UnkStruct_022d5cc4 *s);
    u8 pad3[0x13e - 0x6c];
    u16 unk13E;
    u8 pad4[0x16a - 0x140];
    u16 unk16A;
    u8 pad5[0x20c - 0x16c];
    u32 unk20C;
    u16 unk210;
    u8 pad6[0x214 - 0x212];
    UnkStruct_022d5cc4_v unk214;
    u16 unk220;
    u8 pad7[0x22a - 0x222];
    u16 unk22A;
    u16 unk22C;
    u16 unk22E;
    u8 pad8[0x234 - 0x230];
    u16 unk234;
    u8 pad9[0x2b4 - 0x236];
    u8 unk2B4[0x24];
    UnkStruct_022d5cc4_v unk2D8;
};

void Boss3PartCD_State_StartHittable(UnkStruct_022d5cc4 *s);
void Boss3PartCD_State_Hittable(UnkStruct_022d5cc4 *s);

void Boss3_SetBallHittable(UnkStruct_022d5cc4 *s) {
    s->unk13E |= 4;
    s->unk16A &= ~4;
    s->unk22A = 0;
    if (s->unk20C & 4)
        s->func = Boss3PartCD_State_StartHittable;
    else
        s->func = Boss3PartCD_State_Hittable;
}
