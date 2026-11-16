#include <nitro/types.h>

typedef struct UnkStruct_022c8ca4_6 {
    u8 pad[4];
    u16 unk4;
} UnkStruct_022c8ca4_6;

typedef struct UnkStruct_022c8ca4_5 {
    s32 unk0;
    u8 pad[4];
    UnkStruct_022c8ca4_6 *unk8;
} UnkStruct_022c8ca4_5;

typedef struct UnkStruct_022c8ca4_3 {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x304 - 0x14];
    s32 unk304;
    u8 pad1[4];
    u8 unk30C[0x418 - 0x30c];
    u8 unk418;
    u8 pad2[0x420 - 0x419];
    s32 unk420;
} UnkStruct_022c8ca4_3;

typedef struct UnkStruct_022c8ca4_2 {
    u8 pad[0x20c];
    u32 unk20C;
    u8 padA[4];
    UnkStruct_022c8ca4_3 *unk214;
    u8 padB[0x228 - 0x218];
    s16 unk228;
} UnkStruct_022c8ca4_2;

typedef struct UnkStruct_022c8ca4 UnkStruct_022c8ca4;
struct UnkStruct_022c8ca4 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad0[0x2e - 0x1c];
    u16 unk2E;
    u8 pad0b[0x68 - 0x30];
    void (*func)(UnkStruct_022c8ca4 *s);
    u8 pad1[0x20c - 0x6c];
    u16 unk20C;
    u8 pad2[0x210 - 0x20e];
    u16 unk210;
    u8 pad2b[0x220 - 0x212];
    s32 unk220;
    s32 unk224;
    u8 unk228[0x30c - 0x228];
    UnkStruct_022c8ca4_5 *unk30C;
    u8 pad4[0x334 - 0x310];
    u8 unk334;
    u8 pad5[0x33c - 0x335];
    s32 unk33C;
};

extern UnkStruct_022c8ca4_2 *gBoss1;

void Boss1_SetPartAnimation(void *p, s32 a, s32 b, s32 c, s32 d);
void Boss1_UpdateCameraShake(void *p, s32 a, s32 b);

void func_ov000_022c9f7c(UnkStruct_022c8ca4 *s, s32 x);
void func_ov000_022c8c50(UnkStruct_022c8ca4 *s);

void Boss1Neck_State_Unk_ov000_022c8ca4(UnkStruct_022c8ca4 *s) {
    if (gBoss1->unk228 > 0)
        func_ov000_022c9f7c(s, 0x1aa00);
    else
        func_ov000_022c9f7c(s, 0x1800);
    s->unk20C++;
    if ((s16)s->unk2E != 0)
        s->unk20C = 0;
    if (s->unk20C > 10)
        s->func = func_ov000_022c8c50;
}
