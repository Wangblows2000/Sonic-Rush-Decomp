#include <nitro/types.h>

typedef struct UnkStruct_022c87d8_3 {
    u8 pad[0x304];
    s32 unk304;
    u8 pad1[4];
    u8 unk30C[0x418 - 0x30c];
    u8 unk418;
} UnkStruct_022c87d8_3;

typedef struct UnkStruct_022c87d8_2 {
    u8 pad[0x214];
    UnkStruct_022c87d8_3 *unk214;
} UnkStruct_022c87d8_2;

typedef struct UnkStruct_022c9c24 UnkStruct_022c9c24;
struct UnkStruct_022c9c24 {
    u8 pad[0x68];
    void (*func)(UnkStruct_022c9c24 *s);
    u8 pad1[0x20c - 0x6c];
    u16 unk20C;
    u8 pad2[0x220 - 0x20e];
    s32 unk220;
    u8 pad3[4];
    u8 unk228[0x334 - 0x228];
    u8 unk334;
};

extern UnkStruct_022c87d8_2 *gBoss1;

void Boss1_SetPartAnimation(void *p, s32 a, s32 b, s32 c, s32 d);
void Boss1Neck_State_Unk_ov000_022c9b00(UnkStruct_022c9c24 *s);

void Boss1Neck_State_Unk_ov000_022c9c24(UnkStruct_022c9c24 *s) {
    UnkStruct_022c87d8_3 *p = gBoss1->unk214;
    Boss1_SetPartAnimation(s->unk228, s->unk220, 0xf, 0x1000, 0);
    s->unk334 |= 1;
    Boss1_SetPartAnimation(p->unk30C, p->unk304, 2, 0x1000, 0);
    p->unk418 |= 1;
    s->unk20C = 0;
    s->func = Boss1Neck_State_Unk_ov000_022c9b00;
    s->func(s);
}
