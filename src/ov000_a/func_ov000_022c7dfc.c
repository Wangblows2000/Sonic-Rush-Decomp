#include <nitro/types.h>

typedef struct UnkStruct_022c7dfc_5 {
    s32 unk0;
} UnkStruct_022c7dfc_5;

typedef struct UnkStruct_022c7dfc_3 {
    u8 pad[0x304];
    s32 unk304;
    u8 pad1[4];
    u8 unk30C[0x418 - 0x30c];
    u8 unk418;
} UnkStruct_022c7dfc_3;

typedef struct UnkStruct_022c7dfc_2 {
    u8 pad[0x214];
    UnkStruct_022c7dfc_3 *unk214;
} UnkStruct_022c7dfc_2;

typedef struct UnkStruct_022c7dfc UnkStruct_022c7dfc;
struct UnkStruct_022c7dfc {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad0[0x68 - 0x1c];
    void (*func)(UnkStruct_022c7dfc *s);
    u8 pad1[0x20c - 0x6c];
    u16 unk20C;
    u8 pad2[0x220 - 0x20e];
    s32 unk220;
    s32 unk224;
    u8 unk228[0x30c - 0x228];
    UnkStruct_022c7dfc_5 *unk30C;
    u8 pad4[0x334 - 0x310];
    u8 unk334;
};

extern UnkStruct_022c7dfc_2 *gBoss1;

void Boss1_SetPartAnimation(void *p, s32 a, s32 b, s32 c, s32 d);
void Boss1_UpdateCameraShake(void *p, s32 a, s32 b);

void Boss1Neck_State_Unk_ov000_022c7d54(UnkStruct_022c7dfc *s);

void func_ov000_022c7dfc(UnkStruct_022c7dfc *s) {
    Boss1_SetPartAnimation(s->unk228, s->unk224, 1, 0x1000, 0);
    s->unk10 = -0x7c00;
    s->unk14 = 0x20000;
    s->unk18 = -0x16800;
    s->unk20C = 0;
    s->func = Boss1Neck_State_Unk_ov000_022c7d54;
}
