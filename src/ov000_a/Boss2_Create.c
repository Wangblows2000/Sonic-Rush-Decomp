#include <nitro/types.h>

typedef struct UnkStruct_022d4e54_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x14 - 0xc];
    s16 unk14;
    s16 unk16;
    void *unk18;
    void (*unk1C)(void);
    u32 unk20;
    u8 pad2[0x28 - 0x24];
    u8 unk28;
    u8 pad3[0x2c - 0x29];
} UnkStruct_022d4e54_h;

typedef struct UnkStruct_022d4e54_a {
    u8 pad[8];
    u16 unk8;
    u8 unkA;
    u8 unkB;
    u8 unkC;
    u8 pad1[0x18 - 0xd];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad2[0x48 - 0x24];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad3[0x90 - 0x54];
    u8 unk90[0x10c - 0x90];
    u8 unk10C;
    u8 pad4[0x10f - 0x10d];
    u8 unk10F;
    u8 pad5[0x138 - 0x110];
} UnkStruct_022d4e54_a;

typedef struct UnkStruct_022d4e54 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad0[0x54 - 0x1c];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*unk68)(void);
    u8 pad1b[0x70 - 0x6c];
    void (*unk70)(void);
    u8 pad1c[0x78 - 0x74];
    s32 unk78;
    u8 pad2[0x134 - 0x7c];
    UnkStruct_022d4e54_h unk134;
    UnkStruct_022d4e54_h unk160;
    u8 pad3[0x20c - 0x18c];
    u32 unk20C;
    s32 unk210;
    UnkStruct_022d4e54_a unk214[11];
    void *unkF7C;
    void *unkF80;
    UnkStruct_022d4e54_h unkF84[10];
    UnkStruct_022d4e54_h unk113C[10];
    u8 pad4[0x12fe - 0x12f4];
    s16 unk12FE;
    u16 unk1300;
    u16 unk1302;
    u8 pad5[0x1424 - 0x1304];
    u16 *unk1424;
    u16 *unk1428;
    u16 *unk142C;
    u16 *unk1430;
    u16 *unk1434;
    u16 *unk1438;
    u16 *unk143C;
    u16 *unk1440;
    u16 *unk1444;
    u16 *unk1448;
    u8 pad6[0x1450 - 0x144c];
    u32 *unk1450;
    u32 *unk1454;
    u32 *unk1458;
} UnkStruct_022d4e54;

typedef struct UnkTask_022d4e54 {
    u8 pad[0x10];
    UnkStruct_022d4e54 *unk10;
} UnkTask_022d4e54;

typedef struct UnkStruct_022d4e54_7 {
    u8 pad[4];
    s32 unk4;
    u16 unk8;
    u16 unkA;
} UnkStruct_022d4e54_7;

typedef struct UnkStruct_022d4e54_r {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
} UnkStruct_022d4e54_r;

extern void *gZoneActArchive;
extern s32 gGameState;
extern UnkStruct_022d4e54 *gBoss2;
extern UnkStruct_022d4e54_7 gBossStatus;
extern u32 gRandSeed;
extern s32 data_ov000_02362f30[];
extern UnkStruct_022d4e54_r data_ov000_0236301c[];
extern void (*data_ov000_0236521c[])(void);
extern s32 gBossStage2Models[];
extern u8 data_ov000_02365350[];
extern char data_ov000_02365354[];
extern char data_ov000_0236536c[];
extern char data_ov000_02365384[];
extern char data_ov000_0236539c[];
extern char data_ov000_023653b4[];
extern char data_ov000_023653cc[];
extern char data_ov000_023653e0[];
extern char data_ov000_023653f8[];
extern u16 data_ov000_02362e28[][3];
extern u16 data_ov000_02362e1c[][3];
extern u16 data_ov000_02362e70[][4];
extern u16 data_ov000_02362e90[][4];
extern u16 data_ov000_02362dd8[];
extern u16 data_ov000_02362de8[];
extern u16 data_ov000_02362dd4[];
extern u16 data_ov000_02362de4[];
extern u16 data_ov000_02362dd0[];
extern u16 data_ov000_02362de0[];
extern u32 data_ov000_02365110[];
extern u32 data_ov000_023651c8[];
extern u32 data_ov000_023650fc[];
extern u32 data_ov000_02365108[];
extern u16 data_ov000_02362e04[];
extern u16 data_ov000_02362e0c[];
extern u16 data_ov000_02362dc8[];
extern u16 data_ov000_02362dc4[];

UnkTask_022d4e54 *Task_Create(void (*fn)(void), void (*fn2)(void *), s32 b, s32 c, s32 d, s32 e);
void StageObj_Main(void);
void Boss2_Destructor(void *t);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
u16 Boss_GetStartHealth(void);
void MapObject_Init(UnkStruct_022d4e54 *s, u8 *a, s32 x, s32 y);
void Boss2_Draw_Unk_ov000_022ce614(void);
void Boss2_State_WaitForStart(void);
void ObjRect_SetBox3D(UnkStruct_022d4e54_h *h, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void ObjRect_SetAtkDefFlags(UnkStruct_022d4e54_h *h, s32 a, s32 b);
void func_02021310(void *ctx, void *a, void *b);
void *func_02021264(void *ctx, u32 id);
void func_020212e0(void *ctx);
void *func_0202128c(const char *path);
void NNS_G3dResDefaultSetup(void *p);
void Model_Init(void *p, s32 a);
void Model_SetResource(void *p, s32 a, s32 b);
void func_02021c38(void *p, void (*fn)(void), s32 a, s32 b, s32 c);
void func_ov000_022d4c30(void);
void func_ov000_022d4bfc(void);
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);
void func_ov000_022c5d4c(s32 a, u32 *p1, u16 *p2, u16 *p3, u16 n);
u32 *Sound_AllocHandle(void);

static inline void MI_CpuFill16(void *dest, u16 data, u32 size) {
    MIi_CpuClear16(*(volatile u16 *)&data, dest, size);
}

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

UnkStruct_022d4e54 *Boss2_Create(u8 *a, s32 x, s32 y) {
    s32 i;
    u8 ctx[0x5c];
    UnkStruct_022d4e54 *s;
    UnkStruct_022d4e54_h *h;
    UnkStruct_022d4e54_h *h2;
    UnkStruct_022d4e54_a *p;
    u32 k;
    if (a[0] == 0xff && a[1] == 0xff)
        return NULL;
    s = Task_Create(StageObj_Main, Boss2_Destructor, 0, 0x500, 2, 0x1478)->unk10;
    MI_CpuFill16(s, 0, 0x1478);
    gBoss2 = s;
    s->unk20C |= 9;
    s->unk12FE = Rand() & 1;
    MI_CpuFill16(&gBossStatus, 0, 0xc);
    gBossStatus.unk4 = 7;
    gBossStatus.unk8 = Boss_GetStartHealth();
    gBossStatus.unkA = Boss_GetStartHealth();
    MapObject_Init(s, a, x, y);
    s->unk70 = Boss2_Draw_Unk_ov000_022ce614;
    s->unk68 = Boss2_State_WaitForStart;
    s->unk5C |= 0x20;
    s->unk54 |= 0x12;
    s->unk58 |= 0xa300;
    s->unk10 = 0;
    s->unk14 = 0x20000;
    s->unk18 = 0;
    h = s->unkF84;
    h2 = s->unk113C;
    for (i = 0; i < 10; i++, h2++, h++) {
        switch (data_ov000_02362f30[i]) {
        case 0:
            *h = s->unk160;
            break;
        case 1:
            *h = s->unk160;
            h->unk20 |= 0x20;
            break;
        case 2:
            *h = s->unk134;
            break;
        case 3:
            ObjRect_SetAtkDefFlags(h, 0, -0x10);
            h->unkA |= 0xc0;
            h->unk28 = 0x12;
            break;
        }
        ObjRect_SetBox3D(h, data_ov000_0236301c[i].unk0, data_ov000_0236301c[i].unk2, data_ov000_0236301c[i].unk4, data_ov000_0236301c[i].unk6, -0x7d00, 0x7d00);
        h->unk18 = s;
        h->unkA &= ~4;
        h->unk1C = data_ov000_0236521c[i];
        *h2 = *h;
    }
    s->unk113C[1].unk14 = -0x10;
    s->unkF84[1].unk14 = s->unk113C[1].unk14;
    s->unk113C[1].unk16 = 0x23;
    s->unkF84[1].unk16 = s->unk113C[1].unk16;
    s->unk78 = 0;
    func_02021310(ctx, data_ov000_02365350, gZoneActArchive);
    s->unkF7C = func_02021264(ctx, 8);
    s->unkF80 = func_02021264(ctx, 9);
    NNS_G3dResDefaultSetup(func_0202128c(data_ov000_02365354));
    NNS_G3dResDefaultSetup(func_0202128c(data_ov000_0236536c));
    NNS_G3dResDefaultSetup(func_0202128c(data_ov000_02365384));
    NNS_G3dResDefaultSetup(func_0202128c(data_ov000_0236539c));
    NNS_G3dResDefaultSetup(func_0202128c(data_ov000_023653b4));
    NNS_G3dResDefaultSetup(func_0202128c(data_ov000_023653cc));
    NNS_G3dResDefaultSetup(func_0202128c(data_ov000_023653e0));
    NNS_G3dResDefaultSetup(func_0202128c(data_ov000_023653f8));
    func_020212e0(ctx);
    p = s->unk214;
    for (i = 0; i < 11; i++) {
        Model_Init(p, 0);
        Model_SetResource(p, gBossStage2Models[i], 0);
        p->unk20 = 0x34cc;
        p->unk1C = p->unk20;
        p->unk18 = p->unk1C;
        p++;
    }
    p = &s->unk214[0];
    p->unk48 = 0;
    p->unk4C = -0x200000;
    p->unk50 = 0;
    p = &s->unk214[7];
    p->unk48 = 0;
    p->unk4C = -0x200000;
    p->unk50 = 0;
    func_02021c38(p->unk90, func_ov000_022d4c30, 0, 6, 3);
    p->unkA = 4;
    p = &s->unk214[8];
    p->unk48 = 0;
    p->unk4C = -0x200000;
    p->unk50 = 0;
    p->unk8 = 0x1d;
    p->unkA = 4;
    p->unkB = 2;
    func_02021c38(p->unk90, func_ov000_022d4bfc, 0, 6, 3);
    p = &s->unk214[9];
    p->unk48 = 0;
    p->unk4C = 0;
    p->unk50 = 0;
    p->unk8 = 0x1b;
    p->unkA = 4;
    p->unkB = 2;
    p = &s->unk214[10];
    p->unk48 = 0;
    p->unk4C = 0;
    p->unk50 = 0;
    p->unk8 = 0x1c;
    p->unkA = 4;
    p->unkB = 2;
    p->unkC = 0xb;
    p = &s->unk214[1];
    Model_SetAnimation(p, 0, gBoss2->unkF7C, 0, 0);
    p->unk10C |= 4;
    p->unk48 = 0;
    p->unk4C = 0xffe66000;
    p->unk50 = 0x32000;
    p = &s->unk214[2];
    Model_SetAnimation(p, 3, gBoss2->unkF80, 3, 0);
    p->unk10F |= 4;
    p->unk48 = 0;
    p->unk4C = 0xffe66000;
    p->unk50 = 0;
    p = &s->unk214[4];
    Model_SetAnimation(p, 3, gBoss2->unkF80, 1, 0);
    Model_SetAnimation(p, 0, gBoss2->unkF7C, 1, 0);
    p->unk10F |= 4;
    p->unk48 = 0;
    p->unk4C = -0x200000;
    p->unk50 = 0;
    p = &s->unk214[3];
    Model_SetAnimation(p, 3, gBoss2->unkF80, 0, 0);
    p->unk10F |= 4;
    p->unk48 = 0;
    p->unk4C = -0x200000;
    p->unk50 = 0;
    p = &s->unk214[5];
    Model_SetAnimation(p, 3, gBoss2->unkF80, 2, 0);
    Model_SetAnimation(p, 0, gBoss2->unkF7C, 2, 0);
    p->unk10F |= 4;
    p->unk48 = 0;
    p->unk4C = -0x200000;
    p->unk50 = 0;
    s->unk1300 = 1;
    s->unk1302 = 0xffff;
    k = gGameState;
    s->unk1424 = data_ov000_02362e28[k];
    s->unk1428 = data_ov000_02362e1c[k];
    s->unk142C = data_ov000_02362e70[k];
    s->unk1430 = data_ov000_02362e90[k];
    s->unk1434 = &data_ov000_02362dd8[k];
    s->unk1438 = &data_ov000_02362de8[k];
    s->unk143C = &data_ov000_02362dd4[k];
    s->unk1440 = &data_ov000_02362de4[k];
    s->unk1444 = &data_ov000_02362dd0[k];
    s->unk1448 = &data_ov000_02362de0[k];
    func_ov000_022c5d4c(gBossStage2Models[7], data_ov000_02365110, gBoss2->unk1424, data_ov000_02362e04, 3);
    func_ov000_022c5d4c(gBossStage2Models[9], data_ov000_023651c8, gBoss2->unk142C, data_ov000_02362e0c, 4);
    func_ov000_022c5d4c(gBossStage2Models[8], data_ov000_023650fc, gBoss2->unk1434, data_ov000_02362dc8, 1);
    func_ov000_022c5d4c(gBossStage2Models[10], data_ov000_02365108, gBoss2->unk143C, data_ov000_02362dc4, 1);
    s->unk1450 = Sound_AllocHandle();
    s->unk1454 = Sound_AllocHandle();
    s->unk1458 = Sound_AllocHandle();
    return s;
}
