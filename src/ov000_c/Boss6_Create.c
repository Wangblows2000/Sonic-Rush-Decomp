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
    u8 unk90[0x138 - 0x90];
} UnkStruct_022e6c0c_Sub;

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    UnkStruct_022e6c0c_Sub unk210[9];
    void *unkD08;
    void *unkD0C[4];
    void *unkD1C[4];
    void *unkD2C[4];
    void *unkD3C[4];
    u16 unkD4C;
    u16 unkD4E;
    u8 padD50[4];
    s32 unkD54;
    s32 unkD58;
    u8 padD5C[0xF46 - 0xD5C];
    u16 unkF46;
    u16 unkF48;
    u8 padF4A[0x10F0 - 0xF4A];
    void *unk10F0;
    void *unk10F4;
    void *unk10F8;
    void *unk10FC;
    void *unk1100;
    void *unk1104;
    void *unk1108;
    void *unk110C;
    void *unk1110;
    u8 pad1114[0x1130 - 0x1114];
} UnkStruct_022e6c0c;

typedef struct {
    u32 unk0;
    s32 unk4;
    u16 unk8;
    u16 unkA;
} UnkStruct_02369ac0;

typedef struct {
    s32 unk0;
} UnkStruct_022c4560;

extern UnkStruct_022e6c0c *gBoss6;
extern UnkStruct_02369ac0 gBossStatus;
extern void *data_ov000_02369b50[];
extern void *gZoneActArchive;
extern UnkStruct_022c4560 gGameState;
extern u8 data_ov000_02365968[];
extern s16 data_ov000_02363864[4][4];
extern u8 data_ov000_0236596c[], data_ov000_02365980[], data_ov000_02365994[], data_ov000_023659b0[];
extern u16 data_ov000_0236370c[], data_ov000_02363714[], data_ov000_023636f8[], data_ov000_02363708[];
extern u16 data_ov000_02363700[], data_ov000_02363704[], data_ov000_023636fc[];
extern u8 data_ov000_02363884[][16], data_ov000_023638a4[][16];
extern u8 data_ov000_023658e4[], data_ov000_0236377c[], data_ov000_023657e8[], data_ov000_023636e8[];
extern u8 data_ov000_023657ec[], data_ov000_023636f0[], data_ov000_023657e4[], data_ov000_023636ec[];

void Boss6_Destructor(void);
void Boss6_Draw(void);
void Boss6_State_WaitForStart(void);
void Boss6_Unk_ov000_022e62fc(void);
u16 Boss_GetStartHealth(void);
void NNS_FndMountArchive(void *f, void *path, void *arc);
void *NNS_FndGetArchiveFileByIndex(void *f, s32 i);
void NNS_G3dResDefaultSetup(void *p);
void *NNS_FndGetArchiveFileByName(void *a);
void NNS_FndUnmountArchive(void *f);
void Model_Init(void *s, s32 a);
void Model_SetResource(void *s, void *res, s32 idx);
u8 *NNS_G3dGetMdlSet(void *p);
void NNS_G3dMdlSetMdlAlphaAll(void *p, s32 a);
void NNS_G3dRenderObjSetCallBack(void *p, void (*fn)(void), s32 a, s32 b, s32 c);
void Boss6_Unk_ov000_022e671c(UnkStruct_022e6c0c *w);
void Boss_LoadModelPalettes(void *a, void *b, void *c, void *d, s32 e);

UnkStruct_022e6c0c *Boss6_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    u8 ctx[0x5c];
    UnkStruct_022e6c0c *w;
    UnkStruct_022e6c0c_Sub *sub;
    vu16 zero;
    vu16 zero2;
    s32 i;
    u8 *res;
    s32 idx;

    if (p->unk0 == 0xff && p->unk1 == 0xff)
        return NULL;
    w = Task_Create(StageObj_Main, Boss6_Destructor, 0, 0x500, 2, sizeof(UnkStruct_022e6c0c))->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_022e6c0c));
    gBoss6 = w;
    w->unk20C |= 1;
    w->unkD4E = Rand() & 3;
    zero2 = 0;
    MIi_CpuClear16(zero2, &gBossStatus, sizeof(UnkStruct_02369ac0));
    gBossStatus.unk4 = 0x1c;
    gBossStatus.unk8 = Boss_GetStartHealth();
    gBossStatus.unkA = Boss_GetStartHealth();
    MapObject_Init(w, (s32)p, b, c);
    w->base.unk70 = Boss6_Draw;
    w->base.unk68 = Boss6_State_WaitForStart;
    w->base.unk5C |= 0x20;
    w->base.unk54 |= 0x12;
    w->base.unk58 |= 0xa300;
    NNS_FndMountArchive(ctx, data_ov000_02365968, gZoneActArchive);
    w->unkD08 = NNS_FndGetArchiveFileByIndex(ctx, 8);
    for (i = 0; i < 4; i++) {
        if (data_ov000_02363864[i][0] != -1) {
            w->unkD0C[i] = NNS_FndGetArchiveFileByIndex(ctx, data_ov000_02363864[i][0]);
            NNS_G3dResDefaultSetup(w->unkD0C[i]);
        }
        if (data_ov000_02363864[i][1] != -1)
            w->unkD1C[i] = NNS_FndGetArchiveFileByIndex(ctx, data_ov000_02363864[i][1]);
        if (data_ov000_02363864[i][2] != -1)
            w->unkD2C[i] = NNS_FndGetArchiveFileByIndex(ctx, data_ov000_02363864[i][2]);
        if (data_ov000_02363864[i][3] != -1)
            w->unkD3C[i] = NNS_FndGetArchiveFileByIndex(ctx, data_ov000_02363864[i][3]);
    }
    NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByName(data_ov000_0236596c));
    NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByName(data_ov000_02365980));
    NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByName(data_ov000_02365994));
    NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByName(data_ov000_023659b0));
    NNS_FndUnmountArchive(ctx);
    sub = w->unk210;
    for (i = 0; i < 9; i++) {
        Model_Init(sub, 0);
        Model_SetResource(sub, data_ov000_02369b50[i], 0);
        sub->unk18 = sub->unk1C = sub->unk20 = 0x34cc;
        sub++;
    }
    sub = &w->unk210[0];
    sub->unk48 = 0x1a0000;
    sub->unk4C = -0x200000;
    sub->unk50 = 0;
    sub = &w->unk210[1];
    sub->unk48 = 0x1a0000;
    sub->unk4C = -0x200000;
    sub->unk50 = 0;
    sub = &w->unk210[2];
    sub->unk48 = 0x1a0000;
    sub->unk4C = -0x200000;
    sub->unk50 = 0;
    sub = &w->unk210[3];
    sub->unk48 = 0x1a0000;
    sub->unk4C = -0x200000;
    sub->unk50 = -0x32000;
    res = NNS_G3dGetMdlSet(data_ov000_02369b50[3]);
    NNS_G3dMdlSetMdlAlphaAll(res + *(u32 *)(res + *(u16 *)(res + 0xe) + 0xc), 0);
    sub = &w->unk210[4];
    sub->unk48 = 0xffdc20f0;
    sub->unk4C = -0x200000;
    sub->unk50 = 0x32000;
    res = NNS_G3dGetMdlSet(data_ov000_02369b50[4]);
    NNS_G3dMdlSetMdlAlphaAll(res + *(u32 *)(res + *(u16 *)(res + 0xe) + 0xc), 0);
    sub = &w->unk210[6];
    sub->unk48 = 0x1a0000;
    sub->unk4C = -0x200000;
    sub->unk50 = 0;
    NNS_G3dRenderObjSetCallBack(sub->unk90, Boss6_Unk_ov000_022e62fc, 0, 6, 3);
    sub->unkA = 4;
    sub = &w->unk210[7];
    sub->unk48 = 0;
    sub->unk4C = 0;
    sub->unk50 = 0;
    sub->unk8 = 0x1e;
    sub->unkA = 4;
    sub->unkB = 2;
    sub->unkC = 0xb;
    sub = &w->unk210[8];
    sub->unk48 = 0;
    sub->unk4C = 0;
    sub->unk50 = 0;
    sub->unk8 = 0x1e;
    sub->unkA = 4;
    sub->unkB = 2;
    sub->unkC = 0xb;
    Boss6_Unk_ov000_022e671c(w);
    w->unkF46 = 1;
    w->unkF48 = 0xffff;
    w->unkD54 = -0x17ff;
    w->unkD58 = 0x17ff;
    idx = gGameState.unk0;
    w->unk10F0 = &data_ov000_0236370c[idx];
    w->unk10F4 = data_ov000_02363884[idx];
    w->unk10F8 = data_ov000_023638a4[idx];
    w->unk10FC = &data_ov000_02363714[idx];
    w->unk1100 = &data_ov000_023636f8[idx];
    w->unk1104 = &data_ov000_02363708[idx];
    w->unk1108 = &data_ov000_02363700[idx];
    w->unk110C = &data_ov000_02363704[idx];
    w->unk1110 = &data_ov000_023636fc[idx];
    Boss_LoadModelPalettes(data_ov000_02369b50[6], data_ov000_023658e4, gBoss6->unk10F4, data_ov000_0236377c, 8);
    Boss_LoadModelPalettes(data_ov000_02369b50[7], data_ov000_023657e8, gBoss6->unk10FC, data_ov000_023636e8, 1);
    Boss_LoadModelPalettes(data_ov000_02369b50[8], data_ov000_023657ec, gBoss6->unk1104, data_ov000_023636f0, 1);
    Boss_LoadModelPalettes(data_ov000_02369b50[9], data_ov000_023657e4, gBoss6->unk10F0, data_ov000_023636ec, 1);
    return w;
}
