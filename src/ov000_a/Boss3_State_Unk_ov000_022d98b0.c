#include <nitro/types.h>

typedef struct UnkStruct_022d98b0_a {
    u8 pad[0x48];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad1[0x10c - 0x54];
    u8 unk10C;
    u8 pad2[0x114 - 0x10d];
    s32 unk114;
    u8 pad3[0x170 - 0x118];
} UnkStruct_022d98b0_a;

typedef struct UnkStruct_022d98b0_brl {
    u8 pad[0x54];
    u32 unk54;
    u8 pad1[0x2c4 - 0x58];
    UnkStruct_022d98b0_a unk2C4;
    void *unk434;
} UnkStruct_022d98b0_brl;

typedef struct UnkStruct_022d98b0_rock {
    u8 pad[0x5c];
    u16 unk5C;
} UnkStruct_022d98b0_rock;

typedef struct UnkStruct_022d98b0 UnkStruct_022d98b0;
struct UnkStruct_022d98b0 {
    u8 pad[0x54];
    u32 unk54;
    u8 pad1[0x68 - 0x58];
    void (*func)(UnkStruct_022d98b0 *s);
    u8 pad2[0x214 - 0x6c];
    UnkStruct_022d98b0_brl *unk214;
    UnkStruct_022d98b0_rock *unk218;
    u8 pad3[0x224 - 0x21c];
    u16 unk224;
    u8 pad4[0x350 - 0x226];
    s32 unk350;
};

typedef struct UnkStruct_022d98b0_c {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 pad[0x18 - 0xc];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad1[0x30 - 0x24];
    u16 unk30;
    u8 pad2[0x3c - 0x32];
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    u8 pad3[0x6c - 0x60];
    u16 unk6C;
    u8 pad4[0x78 - 0x6e];
    s32 unk78;
    u8 pad5[0x80 - 0x7c];
    u16 unk80;
    u8 pad6[0x84 - 0x82];
    s32 unk84;
    u8 pad7[0x8c - 0x88];
    u16 unk8C;
    u16 unk8E;
    u8 pad8[0x92 - 0x90];
    u16 unk92;
} UnkStruct_022d98b0_c;

typedef struct UnkStruct_022d98b0_d {
    u8 pad[0xdc];
    s32 unkDC;
} UnkStruct_022d98b0_d;

typedef struct UnkStruct_022d98b0_q {
    u8 pad[0x13e];
    u8 unk13E;
    u8 unk13F;
    u8 unk140;
    u8 pad1[0x248 - 0x141];
    s32 unk248;
} UnkStruct_022d98b0_q;

typedef struct UnkStruct_022d98b0_g {
    u8 pad[8];
    u16 unk8;
    u16 unkA;
    u8 pad1[0x48 - 0xc];
    s16 unk48;
} UnkStruct_022d98b0_g;

extern UnkStruct_022d98b0_g gDisplayMain;

void func_ov000_022d9714(UnkStruct_022d98b0 *s);

void Boss3_State_Unk_ov000_022d98b0(UnkStruct_022d98b0 *s) {
    if (gDisplayMain.unk48 > 0)
        gDisplayMain.unk48 -= 2;
    if (s->unk224++ > 0x1e)
        s->func = func_ov000_022d9714;
}
