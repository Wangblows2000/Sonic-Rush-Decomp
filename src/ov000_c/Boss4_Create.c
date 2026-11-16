#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[8];
    u16 unk8;
    u8 unkA;
    u8 unkB;
    u8 unkC;
    u8 padD[0x18 - 0xD];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x48 - 0x24];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad54[0x90 - 0x54];
    u8 unk90[0x10E - 0x90];
    u8 unk10E;
    u8 pad10F[0x138 - 0x10F];
} UnkStruct_022df55c_Sub;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x228 - 0x20C];
    UnkStruct_022df55c_Sub unk228[7];
    void *unkAB0[17];
    u8 padAF4[0xBE0 - 0xAF4];
    s32 unkBE0;
    u8 padBE4[4];
    s32 unkBE8;
    u8 padBEC[0xBF4 - 0xBEC];
} UnkStruct_022df55c;

typedef struct {
    u32 unk0;
    s32 unk4;
    u16 unk8;
    u16 unkA;
} UnkStruct_02369ac0;

extern UnkStruct_022df55c *gBoss4;
extern UnkStruct_02369ac0 gBossStatus;
extern s16 data_ov000_02363580[];
extern void *data_ov000_02369b30[];
extern void *gZoneActArchive;
extern u8 data_ov000_02365778[], data_ov000_0236577c[];
extern u8 data_ov000_02365780[], data_ov000_02365794[], data_ov000_023657a8[], data_ov000_023657c4[];

void Boss4_Destructor(void);
void Boss4_Draw_Unk_ov000_022dec18(void);
void Boss4_State_WaitForStart(void);
void Boss4_NodeCallback_Rods(void);
void Boss4_NodeCallback_ArmGun(void);
void Boss4_NodeCallback_BodyExit(void);
void MI_CpuFill8(void *p, s32 v, u32 n);
u16 Boss_GetStartHealth(void);
void NNS_FndMountArchive(void *f, void *path, void *arc);
void *NNS_FndGetArchiveFileByIndex(void *f, s32 i);
void NNS_G3dResDefaultSetup(void *p);
void *NNS_FndGetArchiveFileByName(void *a);
void NNS_FndUnmountArchive(void *f);
void Model_Init(void *s, s32 a);
void Model_SetResource(void *s, void *res, s32 idx);
s32 NNS_G3dGetTex(void *p);
void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);
void NNS_G3dRenderObjSetCallBack(void *p, void (*fn)(void), s32 a, s32 b, s32 c);
void Boss4_Unk_ov000_022dbc4c(s32 a);
void Boss4_Unk_ov000_022dbb7c(s32 a);
void Boss4_Unk_ov000_022dba50(s32 a);
void Boss4_Unk_ov000_022dbab4(s32 a);
void Boss4_Unk_ov000_022db9ec(s32 a);

UnkStruct_022df55c *Boss4_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    u32 ctx[0x17];
    u32 ctx2[0x17];
    UnkStruct_022df55c *w;
    s32 i;
    UnkStruct_022df55c_Sub *sub;
    vu16 zero;

    if (p->unk0 == 0xff && p->unk1 == 0xff)
        return NULL;
    w = Task_Create(StageObj_Main, Boss4_Destructor, 0, 0x500, 2, sizeof(UnkStruct_022df55c))->work;
    MI_CpuFill8(w, 0, sizeof(UnkStruct_022df55c));
    gBoss4 = w;
    zero = 0;
    MIi_CpuClear16(zero, &gBossStatus, sizeof(UnkStruct_02369ac0));
    gBossStatus.unk8 = Boss_GetStartHealth();
    gBossStatus.unkA = Boss_GetStartHealth();
    w->unkBE0 = 0;
    w->unkBE8 = data_ov000_02363580[gBossStatus.unk8 - 1];
    MapObject_Init(w, (s32)p, b, c);
    w->base.unk70 = Boss4_Draw_Unk_ov000_022dec18;
    w->base.unk68 = Boss4_State_WaitForStart;
    w->base.unk78 = 0;
    w->base.unk54 |= 0x12;
    w->base.unk58 |= 0xa300;
    w->base.unk5C |= 0x20;
    w->base.unk10 = 0;
    w->base.unk14 = 0x20000;
    w->base.unk18 = 0;
    NNS_FndMountArchive(ctx, data_ov000_02365778, gZoneActArchive);
    w->unkAB0[0] = NNS_FndGetArchiveFileByIndex(ctx, 8);
    w->unkAB0[1] = NNS_FndGetArchiveFileByIndex(ctx, 9);
    w->unkAB0[2] = NNS_FndGetArchiveFileByIndex(ctx, 10);
    NNS_FndUnmountArchive(ctx);
    sub = w->unk228;
    for (i = 0; i < 7; i++) {
        Model_Init(sub, 0);
        Model_SetResource(sub, data_ov000_02369b30[i], 0);
        sub->unk18 = sub->unk1C = sub->unk20 = 0x34cc;
        sub->unk48 = w->base.unk10 << 4;
        sub->unk4C = -(w->base.unk14 << 4);
        sub->unk50 = w->base.unk18 << 4;
        sub->unkA = 4;
        sub++;
    }
    Model_SetAnimation(&w->unk228[0], 2, w->unkAB0[2], 0, NNS_G3dGetTex(data_ov000_02369b30[0]));
    w->unk228[0].unk10E |= 4;
    sub = (UnkStruct_022df55c_Sub *)(u32)&w->unk228[4];
    w->unk228[4].unk4C = 0xffec6b72;
    NNS_G3dRenderObjSetCallBack(sub->unk90, Boss4_NodeCallback_Rods, 0, 6, 3);
    NNS_G3dRenderObjSetCallBack(w->unk228[2].unk90, Boss4_NodeCallback_ArmGun, 0, 6, 3);
    NNS_G3dRenderObjSetCallBack(w->unk228[1].unk90, Boss4_NodeCallback_BodyExit, 0, 6, 3);
    NNS_FndMountArchive(ctx2, data_ov000_0236577c, gZoneActArchive);
    w->unkAB0[3] = NNS_FndGetArchiveFileByIndex(ctx2, 0x51);
    w->unkAB0[4] = NNS_FndGetArchiveFileByIndex(ctx2, 0x52);
    w->unkAB0[5] = NNS_FndGetArchiveFileByIndex(ctx2, 0x53);
    w->unkAB0[6] = NNS_FndGetArchiveFileByIndex(ctx2, 0x54);
    w->unkAB0[7] = NNS_FndGetArchiveFileByIndex(ctx2, 0x55);
    w->unkAB0[8] = NNS_FndGetArchiveFileByIndex(ctx2, 0x56);
    w->unkAB0[9] = NNS_FndGetArchiveFileByIndex(ctx2, 0x57);
    w->unkAB0[10] = NNS_FndGetArchiveFileByIndex(ctx2, 0x59);
    w->unkAB0[11] = NNS_FndGetArchiveFileByIndex(ctx2, 0x58);
    w->unkAB0[12] = NNS_FndGetArchiveFileByIndex(ctx2, 0x5a);
    w->unkAB0[13] = NNS_FndGetArchiveFileByIndex(ctx2, 0x5b);
    w->unkAB0[14] = NNS_FndGetArchiveFileByIndex(ctx2, 0x5c);
    w->unkAB0[15] = NNS_FndGetArchiveFileByIndex(ctx2, 0x5e);
    w->unkAB0[16] = NNS_FndGetArchiveFileByIndex(ctx2, 0x5d);
    NNS_G3dResDefaultSetup(w->unkAB0[3]);
    NNS_G3dResDefaultSetup(w->unkAB0[4]);
    NNS_G3dResDefaultSetup(w->unkAB0[9]);
    NNS_G3dResDefaultSetup(w->unkAB0[12]);
    NNS_G3dResDefaultSetup(w->unkAB0[14]);
    NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByName(data_ov000_02365780));
    NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByName(data_ov000_02365794));
    NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByName(data_ov000_023657a8));
    NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByName(data_ov000_023657c4));
    NNS_FndUnmountArchive(ctx2);
    Boss4_Unk_ov000_022dbc4c(0);
    Boss4_Unk_ov000_022dbb7c(0);
    Boss4_Unk_ov000_022dba50(0);
    Boss4_Unk_ov000_022dbab4(0);
    Boss4_Unk_ov000_022db9ec(0);
    return w;
}
