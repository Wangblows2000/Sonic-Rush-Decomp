#include <nitro/types.h>

typedef struct UnkStruct_022c61ec_2 {
    u8 pad[0x228];
    s16 unk228;
} UnkStruct_022c61ec_2;

typedef struct UnkStruct_022c61ec UnkStruct_022c61ec;
struct UnkStruct_022c61ec {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x2e - 0x14];
    u16 unk2E;
    u8 pad1[0x68 - 0x30];
    void (*func)(UnkStruct_022c61ec *s);
    u8 pad2[0x2bc - 0x6c];
    u16 unk2BC;
    u8 pad3[2];
    u16 unk2C0;
    u16 unk2C2;
    u8 pad4[0x2cc - 0x2c4];
    u16 unk2CC;
    s16 unk2CE;
};

extern UnkStruct_022c61ec_2 *gBoss1;

void Boss1_SetCameraSide(s16 a);
s32 FX_Div(s32 a, s32 b);
void Boss1Hammer_State_SlideToEdge(UnkStruct_022c61ec *s);

void Boss1Hammer_State_TurnAtEdge(UnkStruct_022c61ec *s) {
    s32 v;
    if (gBoss1->unk228 > 0)
        s->unk2CE = -0xc00;
    else
        s->unk2CE = 0xc00;
    Boss1_SetCameraSide(-gBoss1->unk228);
    s->unk2C0 = 0;
    v = (s16)s->unk2E;
    if (v < 0)
        v = -v;
    s->unk2C2 = FX_Div(v << 4, 0x78000);
    s->unk2C2 >>= 1;
    s->func = Boss1Hammer_State_SlideToEdge;
    s->func(s);
}
