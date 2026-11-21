#include <nitro/types.h>

typedef struct UnkStruct_022e1078_h {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    u16 unkA;
    u8 pad[0x2c - 0xc];
} UnkStruct_022e1078_h;

typedef struct UnkStruct_022e1078 {
    u8 pad[0xd5e];
    u16 unkD5E;
    UnkStruct_022e1078_h unkD60[11];
    u8 pad1[0xf4c - 0xd60 - 11 * 0x2c];
    s16 unkF4C;
    u8 pad2[0x1068 - 0xf4e];
    s32 unk1068;
    s32 unk106C;
    u8 pad3[0x10bc - 0x1070];
    s32 unk10BC;
    s32 unk10C0;
    u8 pad4[0x1114 - 0x10c4];
    s32 unk1114;
} UnkStruct_022e1078;

typedef struct UnkStruct_022e1078_g {
    u8 pad[0x110c];
    s32 unk110C;
    s32 unk1110;
} UnkStruct_022e1078_g;

typedef struct UnkStruct_022e1078_2 {
    u8 pad[0x18];
    s32 unk18;
} UnkStruct_022e1078_2;

extern UnkStruct_022e1078_g *gBoss6;
extern UnkStruct_022e1078_2 data_ov000_02369b50;
extern s32 data_ov000_02363750[];
extern s32 data_ov000_02363720[];
extern u32 data_ov000_023657e0[];
extern u16 data_ov000_023636f4[];

void ObjRect_Register(UnkStruct_022e1078_h *h);
void Boss_LoadModelPalettes(s32 a, u32 *p1, s32 p2, u16 *p3, u16 n);

void Boss6_RegisterHitboxes3x2(UnkStruct_022e1078 *s) {
    s32 i;
    UnkStruct_022e1078_h *h;
    s16 x1;
    s16 y1;
    s16 x2;
    s16 y2;

    x1 = s->unk10BC >> 12;
    y1 = -(s->unk10C0 >> 12);
    x2 = s->unk1068 >> 12;
    y2 = -(s->unk106C >> 12);
    for (i = 0; i < 3; i++) {
        h = &s->unkD60[data_ov000_02363750[i]];
        if (h->unkA & 4) {
            h->unk0 = x1;
            h->unk4 = y1;
            h->unkA &= ~3;
            if ((s->unkD5E == 0 && s->unkF4C != 0) || (s->unkD5E != 0 && s->unkF4C == 0))
                h->unkA |= 1;
            ObjRect_Register(h);
        }
    }
    for (i = 0; i < 3; i++) {
        h = &s->unkD60[data_ov000_02363720[i]];
        if (h->unkA & 4) {
            h->unk0 = x2;
            h->unk4 = y2;
            h->unkA &= ~3;
            if ((s->unkD5E == 0 && s->unkF4C != 0) || (s->unkD5E != 0 && s->unkF4C == 0))
                h->unkA |= 1;
            ObjRect_Register(h);
        }
    }
    if (s->unkD60[1].unkA & 4) {
        s->unk1114++;
        if (s->unk1114 == 3) {
            Boss_LoadModelPalettes(data_ov000_02369b50.unk18, data_ov000_023657e0, gBoss6->unk1110, data_ov000_023636f4, 1);
        } else if (s->unk1114 == 7) {
            Boss_LoadModelPalettes(data_ov000_02369b50.unk18, data_ov000_023657e0, gBoss6->unk110C, data_ov000_023636f4, 1);
            s->unk1114 = 0;
        }
    } else if (s->unk1114 != 0) {
        Boss_LoadModelPalettes(data_ov000_02369b50.unk18, data_ov000_023657e0, gBoss6->unk110C, data_ov000_023636f4, 1);
        s->unk1114 = 0;
    }
}
