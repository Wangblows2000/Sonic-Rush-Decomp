#include <nitro/types.h>

typedef struct UnkStruct_022d12fc_a {
    u8 pad[0xb];
    u8 unkB;
    u8 pad0[0x24 - 0xc];
    s32 unk24[9];
    u8 pad1[0x90 - 0x48];
    u8 unk90[0xe4 - 0x90];
    s32 *unkE4;
    u8 padE8[0x10c - 0xe8];
    u8 unk10C;
    u8 pad2[0x114 - 0x10d];
    s32 unk114;
    u8 pad3[0x138 - 0x118];
} UnkStruct_022d12fc_a;

typedef struct UnkStruct_022d12fc_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x2c - 0xc];
} UnkStruct_022d12fc_h;

typedef struct UnkStruct_022d12fc_q {
    u8 pad[8];
    u16 unk8;
    u8 unkA;
    u8 unkB;
} UnkStruct_022d12fc_q;

typedef struct UnkStruct_022d12fc_o {
    u8 pad[0x54];
    u32 unk54;
    u8 pad0[0x5c - 0x58];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void *unk68;
    u8 pad2[0x134 - 0x6c];
    UnkStruct_022d12fc_q unk134;
} UnkStruct_022d12fc_o;

typedef struct UnkStruct_022d12fc UnkStruct_022d12fc;
struct UnkStruct_022d12fc {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x68 - 0x14];
    void (*func)(UnkStruct_022d12fc *s);
    u8 pad1[0x20c - 0x6c];
    u32 unk20C;
    u8 pad2[0xa9c - 0x210];
    UnkStruct_022d12fc_a unkA9C;
    UnkStruct_022d12fc_a unkBD4;
    UnkStruct_022d12fc_a unkD0C;
    u8 pad3[0xf7c - 0xe44];
    s32 unkF7C;
    u8 pad3b[0xf84 - 0xf80];
    UnkStruct_022d12fc_h unkF84[10];
    u8 pad4[0x12f4 - 0x113c];
    s32 unk12F4;
    u8 pad4b[0x1304 - 0x12f8];
    u16 unk1304;
    u8 pad5[0x131c - 0x1306];
    UnkStruct_022d12fc_o *unk131C;
    u8 pad5b[0x139c - 0x1320];
    s32 unk139C;
    s32 unk13A0;
    s32 unk13A4;
    u8 pad5c[0x1458 - 0x13a8];
    s32 unk1458;
    u16 unk145C;
    u8 pad6[0x1460 - 0x145e];
    s32 unk1460;
    s32 unk1464;
    u8 pad7[0x1474 - 0x1468];
    s32 unk1474;
};

void Model_SetAnimation(void *p, s32 a, s32 b, s32 c, s32 d);
void Boss2_SetPalettes(u32 flags);

extern UnkStruct_022d12fc *gBoss2;

void NNS_G3dRenderObjSetCallBack(void *p, void (*fn)(void), s32 a, s32 b, s32 c);
void Boss2_NodeCallback_Head(void);
void BossArena_ArcToCirclePos(s32 len, s32 b, s32 a, s32 *outX, s32 *outY, u16 *outAng);
void Boss2_SetTargetAngle(UnkStruct_022d12fc *s, s32 a);
void Boss2_State_Attack3Start(UnkStruct_022d12fc *s);

void Boss2_Action_Attack3(void) {
    UnkStruct_022d12fc *s = gBoss2;
    UnkStruct_022d12fc_a *x = &s->unkA9C;
    UnkStruct_022d12fc_a *y = &s->unkBD4;
    UnkStruct_022d12fc_a *z = &s->unkD0C;
    u16 ang;
    NNS_G3dRenderObjSetCallBack(z->unk90, Boss2_NodeCallback_Head, 0, 6, 3);
    if (s->unk1304) {
        Model_SetAnimation(x, 0, s->unkF7C, 0x17, 0);
        Model_SetAnimation(y, 0, s->unkF7C, 0x35, 0);
        Model_SetAnimation(z, 0, s->unkF7C, 0x53, 0);
    } else {
        Model_SetAnimation(x, 0, s->unkF7C, 0x11, 0);
        Model_SetAnimation(y, 0, s->unkF7C, 0x31, 0);
        Model_SetAnimation(z, 0, s->unkF7C, 0x4d, 0);
    }
    x->unk10C &= ~4;
    y->unk10C &= ~4;
    z->unk10C &= ~4;
    x->unk114 = 0x1000;
    y->unk114 = 0x1000;
    z->unk114 = 0x1000;
    s->func = Boss2_State_Attack3Start;
    s->unk145C = s->unk1304;
    s->unk1460 = s->unk10;
    BossArena_ArcToCirclePos(0x1152f0, 0x6cd988, s->unk10 << 4, NULL, NULL, &ang);
    s->unk1304 = ang + 0x4000;
    s->unk20C |= 2;
    s->unk10 = 0;
    s->unk20C &= ~0x40;
    s->unkF84[8].unkA |= 4;
    s->unkF84[8].unkA &= ~0x1000;
    Boss2_SetTargetAngle(s, 0x1c71000);
}
