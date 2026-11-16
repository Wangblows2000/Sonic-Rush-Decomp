#include <nitro/types.h>

typedef struct UnkStruct_022db390_a {
    u8 pad[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad1[0x48 - 0x24];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad2[0x10f - 0x54];
    u8 unk10F;
    u8 pad3[0x138 - 0x110];
} UnkStruct_022db390_a;

typedef struct UnkStruct_022db390 {
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
    u8 pad2[0x214 - 0x74];
    void *unk214;
    u8 pad3[0x21e - 0x218];
    u16 unk21E;
    u8 pad4[0x22c - 0x220];
    s32 unk22C;
    void *unk230;
    void *unk234;
    s32 unk238;
    UnkStruct_022db390_a unk23C;
} UnkStruct_022db390;

typedef struct UnkTask_022db390 {
    u8 pad[0x10];
    UnkStruct_022db390 *unk10;
} UnkTask_022db390;

typedef struct UnkStruct_022db390_7 {
    u8 pad[8];
    u16 unk8;
    u16 unkA;
} UnkStruct_022db390_7;

typedef struct { u8 data[0x5c]; } UnkArc_022db390;

extern void *gZoneActArchive;
extern s32 gGameState;
extern UnkStruct_022db390 *gBoss3;
extern UnkStruct_022db390_7 gBossStatus;
extern u32 gRandSeed;
extern s32 data_ov000_02369b18[];
extern u8 data_ov000_0236554c[];
extern u8 data_ov000_02365550[];
extern char data_ov000_02365554[];
extern char data_ov000_02365568[];
extern char data_ov000_0236557c[];
extern char data_ov000_02365590[];
extern char data_ov000_023655a4[];
extern char data_ov000_023655b8[];
extern char data_ov000_023655cc[];

UnkTask_022db390 *Task_Create(void (*fn)(void), void (*fn2)(void), s32 b, s32 c, s32 d, s32 e);
void StageObj_Main(void);
void Boss3_Destructor(void);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void Boss3_State_WaitForStart(void);
u16 Boss_GetStartHealth(void);
void NNS_FndMountArchive(UnkArc_022db390 *ctx, void *a, void *b);
void *NNS_FndGetArchiveFileByIndex(UnkArc_022db390 *ctx, u32 id);
void NNS_FndUnmountArchive(UnkArc_022db390 *ctx);
void *NNS_FndGetArchiveFileByName(const char *path);
void NNS_G3dResDefaultSetup(void *p);
void MapObject_Init(UnkStruct_022db390 *s, u8 *a, s32 x, s32 y);
void Boss3_Draw_Model(void);
void Model_Init(void *p, s32 a);
void Model_SetResource(void *p, s32 a, s32 b);
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);
void *MapObject_Spawn(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i);
void Boss3_LoadPalettes(s32 k, BOOL f);
void Boss3_Unk_ov000_022dad70(s32 k, BOOL f);
void ScreenShake_Start(s32 a);

static inline void MI_CpuFill16(void *dest, u16 data, u32 size) {
    MIi_CpuClear16(*(volatile u16 *)&data, dest, size);
}

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

UnkStruct_022db390 *Boss3_Create(u8 *a, s32 x, s32 y) {
    UnkArc_022db390 arc1;
    UnkArc_022db390 arc2;
    UnkStruct_022db390 *s;
    UnkStruct_022db390_a *p;
    if (a[0] == 0xff && a[1] == 0xff)
        return NULL;
    s = Task_Create(StageObj_Main, Boss3_Destructor, 0, 0x500, 2, 0x374)->unk10;
    MI_CpuFill16(s, 0, 0x374);
    gBoss3 = s;
    s->unk68 = Boss3_State_WaitForStart;
    MI_CpuFill16(&gBossStatus, 0, 0xc);
    gBossStatus.unk8 = Boss_GetStartHealth();
    gBossStatus.unkA = Boss_GetStartHealth();
    NNS_FndMountArchive(&arc1, data_ov000_0236554c, gZoneActArchive);
    s->unk230 = NNS_FndGetArchiveFileByIndex(&arc1, 8);
    s->unk234 = NNS_FndGetArchiveFileByIndex(&arc1, 9);
    NNS_FndUnmountArchive(&arc1);
    s->unk22C = data_ov000_02369b18[0];
    s->unk238 = data_ov000_02369b18[4];
    MapObject_Init(s, a, x, y);
    s->unk70 = Boss3_Draw_Model;
    s->unk54 |= 0x12;
    s->unk58 |= 0xa300;
    s->unk5C |= 0x20;
    s->unk21E = Rand() & 3;
    s->unk10 = 0;
    s->unk14 = 0x20000;
    s->unk18 = 0;
    p = &s->unk23C;
    Model_Init(p, 0);
    Model_SetResource(p, s->unk22C, 0);
    Model_SetAnimation(p, 3, s->unk234, 0, 0);
    p->unk10F |= 4;
    p->unk48 = s->unk10 << 4;
    p->unk4C = -(s->unk14 << 4);
    p->unk50 = s->unk18 << 4;
    p->unk20 = 0x34cc;
    p->unk1C = p->unk20;
    p->unk18 = p->unk1C;
    s->unk214 = MapObject_Spawn(0xcc, x, 0, 0, 0, 0, 0, 0, 0);
    Boss3_LoadPalettes(gGameState, 0);
    Boss3_Unk_ov000_022dad70(gGameState, 0);
    NNS_FndMountArchive(&arc2, data_ov000_02365550, gZoneActArchive);
    NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByName(data_ov000_02365554));
    NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByName(data_ov000_02365568));
    NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByName(data_ov000_0236557c));
    NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByName(data_ov000_02365590));
    NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByName(data_ov000_023655a4));
    NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByName(data_ov000_023655b8));
    NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByName(data_ov000_023655cc));
    NNS_FndUnmountArchive(&arc2);
    ScreenShake_Start(0xb);
    return s;
}
