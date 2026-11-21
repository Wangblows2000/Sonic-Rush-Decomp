#include <nitro/types.h>

typedef struct {
    u8 pad0[0x48];
    s32 unk48;
} UnkStruct_0231f794_Sub;

typedef struct {
    u8 pad0[0x20C];
    UnkStruct_0231f794_Sub unk20C;
} UnkStruct_0231f794_Obj;

typedef struct {
    u8 pad0[0x134];
    UnkStruct_0231f794_Sub unk134;
} UnkStruct_0231f794_Obj2;

typedef struct {
    u8 pad0[0x210];
    s32 unk210;
    UnkStruct_0231f794_Obj *unk214;
    u8 pad218[0x384 - 0x218];
    s32 unk384;
} UnkStruct_0231f794_Ctx;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 padC[0x18 - 0xC];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x3C - 0x24];
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    u8 pad48[0x54 - 0x48];
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    u8 pad60[0x78 - 0x60];
    s32 unk78;
    u8 pad7C[0x84 - 0x7C];
    s32 unk84;
    u8 pad88[0x8E - 0x88];
    s16 unk8E;
} UnkStruct_0236a48c;

extern UnkStruct_0231f794_Ctx *gBoss7;
extern UnkStruct_0231f794_Obj2 *gPlayers;
extern UnkStruct_0236a48c data_ov000_0236a48c;

s32 FX_Div(s32 a, s32 b);

#define FX_MUL(a, b) ((s32)(((s64)(a) * (b) + 0x800) >> 12))
#define ABS(x) ((x) < 0 ? -(x) : (x))

void StageZoneScript_Zone7Boss(void) {
    UnkStruct_0231f794_Ctx *ctx;
    UnkStruct_0231f794_Sub *a;
    UnkStruct_0231f794_Sub *b;
    UnkStruct_0236a48c *p;
    s32 t;

    a = &gPlayers->unk134;
    ctx = gBoss7;
    b = &ctx->unk214->unk20C;
    p = &data_ov000_0236a48c;

    switch (ctx->unk210) {
    case 0:
        break;
    case 1:
        p->unk3C = p->unk40 = p->unk44 = 0;
        p->unk54 = (a->unk48 + b->unk48) / 2;
        p->unk58 = -0x1d8000;
        p->unk5C = 0;
        p->unk0 = p->unk4 = p->unk8 = 0;
        p->unk18 = (a->unk48 + b->unk48) / 2;
        p->unk1C = -0x1d0000;
        p->unk20 = ABS(a->unk48 - b->unk48);
        if (p->unk20 < 0x96000)
            p->unk20 = 0x96000;
        if (p->unk20 > 0xc8000)
            p->unk20 = 0xc8000;
        t = FX_MUL(FX_Div(p->unk20 - 0x96000, 0x32000), 0x30000);
        p->unk58 += t;
        p->unk1C += t;
        break;
    case 2:
        p->unk3C = p->unk40 = p->unk44 = 0;
        p->unk54 = 0xa0000;
        p->unk58 = -0x1d8000;
        p->unk5C = 0;
        p->unk0 = p->unk4 = p->unk8 = 0;
        p->unk18 = 0xa0000;
        p->unk1C = -0x1d0000;
        p->unk20 = 0xc8000;
        t = FX_MUL(FX_Div(p->unk20 - 0x96000, 0x32000), 0x30000);
        p->unk58 += t;
        p->unk1C += t;
        break;
    case 3:
        p->unk3C = p->unk40 = p->unk44 = 0;
        p->unk54 = 0xa0000;
        p->unk58 = -0x1d8000;
        p->unk5C = 0;
        p->unk78 = ABS(a->unk48 - b->unk48);
        if (p->unk78 < 0x50000)
            p->unk78 = 0x50000;
        if (p->unk78 > 0xc8000)
            p->unk78 = 0xc8000;
        p->unk58 += FX_MUL(FX_Div(0x78000 - (p->unk78 - 0x50000), 0x78000), -0x10000);
        p->unk84 = 0x4000;
        p->unk84 += FX_MUL(FX_Div(0x78000 - (p->unk78 - 0x50000), 0x78000), 0xc000);
        p->unk8E = (-FX_Div(ctx->unk384 << 12, 0x4000000) * 0x2000) >> 12;
        break;
    }
}
