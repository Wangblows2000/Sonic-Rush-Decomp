#include <nitro/types.h>

typedef struct UnkStruct_022ce474_h {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    u16 unkA;
    u8 pad[0x2c - 0xc];
} UnkStruct_022ce474_h;

typedef struct UnkStruct_022ce474 {
    u8 pad[0x10];
    s32 unk10;
    u8 padb[4];
    s32 unk18;
    u8 padc[0x5c - 0x1c];
    u16 unk5C;
    u8 pad0[0xf84 - 0x5e];
    UnkStruct_022ce474_h unkF84[4];
    UnkStruct_022ce474_h unk1034;
    u8 pad1[0x113c - 0x1060];
    UnkStruct_022ce474_h unk113C[4];
    u8 pad2[0x1304 - 0x11ec];
    u16 unk1304;
    u8 pad3[0x134c - 0x1306];
    s32 unk134C;
    u8 pad3b[0x139c - 0x1350];
    s32 unk139C;
    s32 unk13A0;
    s32 unk13A4;
    u8 pad4[0x144c - 0x13a8];
    s32 unk144C;
} UnkStruct_022ce474;

typedef struct UnkStruct_022ce474_g {
    u8 pad[0x1444];
    u16 *unk1444;
    u16 *unk1448;
} UnkStruct_022ce474_g;

typedef struct UnkStruct_022ce474_2 {
    u8 pad[0x24];
    s32 unk24;
} UnkStruct_022ce474_2;

extern UnkStruct_022ce474_g *gBoss2;
extern UnkStruct_022ce474_2 gBossStage2Models;
extern u16 data_ov000_02362ddc[];
extern u32 data_ov000_0236510c[];
extern u16 data_ov000_02362dcc[];

s32 FX_Sqrt(s32 a);
void ObjRect_Register(UnkStruct_022ce474_h *h);
void Boss_LoadModelPalettes(s32 a, u32 *p1, u16 *p2, u16 *p3, u16 n);

void Boss2_UpdateBodyHitboxes(UnkStruct_022ce474 *s) {
    s32 i;
    UnkStruct_022ce474_h *h1;
    UnkStruct_022ce474_h *h2;
    s16 x;
    s16 xb;
    s16 y;
    s16 z;
    x = s->unk10 >> 8;
    y = -(s->unk134C >> 12);
    z = s->unk18 >> 8;
    if (x < 0x366)
        xb = x + 0x6cd;
    else
        xb = x - 0x6cd;
    for (i = 0; i < 2; i++) {
        h1 = &s->unkF84[data_ov000_02362ddc[i]];
        h2 = &s->unk113C[data_ov000_02362ddc[i]];
        if (h1->unkA & 4) {
            h1->unk0 = x;
            h2->unk0 = xb;
            h1->unk4 = h2->unk4 = y;
            h1->unk8 = h2->unk8 = z;
            h1->unkA &= ~3;
            h2->unkA &= ~3;
            if (!(s->unk5C & 1)) {
                h1->unkA |= 1;
                h2->unkA |= 1;
            }
            ObjRect_Register(h1);
            ObjRect_Register(h2);
        }
    }
    if (s->unk144C != 0) {
        Boss_LoadModelPalettes(gBossStage2Models.unk24, data_ov000_0236510c, gBoss2->unk1444, data_ov000_02362dcc, 1);
        s->unk144C = 0;
    }
}
