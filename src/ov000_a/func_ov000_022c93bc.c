#include <nitro/types.h>

typedef struct UnkStruct_022c93bc_6 {
    u8 pad[4];
    u16 unk4;
} UnkStruct_022c93bc_6;

typedef struct UnkStruct_022c93bc_5 {
    s32 unk0;
    u8 pad[4];
    UnkStruct_022c93bc_6 *unk8;
} UnkStruct_022c93bc_5;

typedef struct UnkStruct_022c93bc_3 {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x304 - 0x14];
    s32 unk304;
    u8 pad1[4];
    u8 unk30C[0x3f0 - 0x30c];
    UnkStruct_022c93bc_5 *unk3F0;
    u8 pad1b[0x418 - 0x3f4];
    u8 unk418;
    u8 pad2[0x420 - 0x419];
    s32 unk420;
} UnkStruct_022c93bc_3;

typedef struct UnkStruct_022c93bc_2 {
    u8 pad[0x20c];
    u32 unk20C;
    u8 padA[4];
    UnkStruct_022c93bc_3 *unk214;
    u8 padB[0x228 - 0x218];
    s16 unk228;
    u16 unk22A;
} UnkStruct_022c93bc_2;

typedef struct UnkStruct_022c93bc UnkStruct_022c93bc;
struct UnkStruct_022c93bc {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad0[0x2e - 0x1c];
    u16 unk2E;
    u8 pad0b[0x68 - 0x30];
    void (*func)(UnkStruct_022c93bc *s);
    u8 pad1[0x20c - 0x6c];
    u16 unk20C;
    u16 unk20E;
    u16 unk210;
    u8 pad2b[0x220 - 0x212];
    s32 unk220;
    s32 unk224;
    u8 unk228[0x30c - 0x228];
    UnkStruct_022c93bc_5 *unk30C;
    u8 pad4[0x334 - 0x310];
    u8 unk334;
    u8 pad5[0x33c - 0x335];
    s32 unk33C;
};

extern UnkStruct_022c93bc_2 *gBoss1;

void func_ov000_022cc00c(void *p, s32 a, s32 b, s32 c, s32 d);
void func_ov000_022cc06c(void *p, s32 a, s32 b);

typedef struct UnkStruct_022c93bc_cam {
    u8 pad[0x48];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad0[0x64 - 0x54];
    s32 unk64;
    s32 unk68;
    u8 pad1[0x78 - 0x6c];
    u16 unk78;
    u8 pad2[0x84 - 0x7a];
    void *unk84;
    void *unk88;
    void *unk8C;
    u8 pad3[0x94 - 0x90];
    s32 unk94;
    u8 pad4[0xb4 - 0x98];
    u16 unkB4;
    u8 pad5[0xdc - 0xb6];
    s32 unkDC;
} UnkStruct_022c93bc_cam;

extern UnkStruct_022c93bc_cam data_ov000_0236a444;

BOOL func_ov000_022cc6d4(void);
void Boss1Neck_State_Unk_ov000_022c9294(UnkStruct_022c93bc *s);

void func_ov000_022c93bc(UnkStruct_022c93bc *s) {
    UnkStruct_022c93bc_3 *p = gBoss1->unk214;
    if (!func_ov000_022cc6d4())
        s->unk2E = 0;
    func_ov000_022cc00c(s->unk228, s->unk220, 0x14, 0x1000, 0);
    func_ov000_022cc00c(p->unk30C, p->unk304, 7, 0x1000, 0);
    data_ov000_0236a444.unk64 = -0x210000;
    data_ov000_0236a444.unk94 = 0x180000;
    data_ov000_0236a444.unk68 -= 0x50000;
    s->unk20C = 0;
    s->func = Boss1Neck_State_Unk_ov000_022c9294;
    s->func(s);
}
