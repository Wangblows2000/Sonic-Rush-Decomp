#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x48 - 0x24];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad54[0x10C - 0x54];
    u8 unk10C;
    u8 pad10D[3];
    u8 unk110;
    u8 pad111[0x138 - 0x111];
} UnkStruct_022eee38_Sub;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x210 - 0x20C];
    s32 unk210;
    s32 unk214;
    u8 pad218[0x22E - 0x218];
    u16 unk22E;
    u8 pad230[0x23C - 0x230];
    void *unk23C;
    void *unk240;
    void *unk244;
    void *unk248;
    UnkStruct_022eee38_Sub unk24C;
    u8 pad384[4];
    s32 unk388;
    u8 pad38C[0x398 - 0x38C];
} UnkStruct_022eee38;

typedef struct {
    u32 unk0;
    s32 unk4;
    u16 unk8;
    u16 unkA;
} UnkStruct_02369ac0;

typedef struct {
    s32 unk0;
} UnkStruct_022c4560;

extern UnkStruct_022eee38 *gBoss7;
extern UnkStruct_02369ac0 gBossStatus;
extern void *data_ov000_02369b7c[];
extern void *gZoneActArchive;
extern UnkStruct_022c4560 gGameState;
extern u8 data_ov000_02365b30[], data_ov000_02365b34[];
extern u8 data_ov000_02365b38[], data_ov000_02365b4c[], data_ov000_02365b64[], data_ov000_02365b78[], data_ov000_02365b90[], data_ov000_02365ba8[], data_ov000_02365bc0[], data_ov000_02365bd4[], data_ov000_02365bf0[], data_ov000_02365c0c[], data_ov000_02365c28[], data_ov000_02365c3c[];

void Boss7_Destructor(void);
void Boss7FrameCounter_Main(void);
void Boss7_State_WaitForStart(void);
void Boss7_Draw(void);
u16 Boss_GetStartHealth(void);
void func_02021310(void *f, void *path, void *arc);
void *func_02021264(void *f, s32 i);
void NNS_G3dResDefaultSetup(void *p);
void *func_0202128c(void *a);
void func_020212e0(void *f);
void Model_Init(void *s, s32 a);
void Model_SetResource(void *s, void *res, s32 idx);
void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);
u8 *NNS_G3dGetMdlSet(void *p);
void NNSi_G3dModifyMatFlag(void *p, s32 a, s32 b);
s32 MapObject_Spawn(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i);

UnkStruct_022eee38 *Boss7_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    u32 ctx[0x17];
    u32 ctx2[0x17];
    UnkStruct_Task *task;
    UnkStruct_022eee38 *w;
    UnkStruct_022eee38_Sub *sub;
    u8 *res;
    vu16 zero;
    vu16 zero2;

    if (p->unk0 == 0xff && p->unk1 == 0xff)
        return NULL;
    task = Task_Create(StageObj_Main, Boss7_Destructor, 0, 0x500, 2, sizeof(UnkStruct_022eee38));
    Task_Create(Boss7FrameCounter_Main, 0, 0, 0x503, 2, 0);
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_022eee38));
    gBoss7 = w;
    w->base.unk68 = Boss7_State_WaitForStart;
    w->unk22E = Rand();
    w->unk388 = Sound_AllocHandle();
    zero2 = 0;
    MIi_CpuClear16(zero2, &gBossStatus, sizeof(UnkStruct_02369ac0));
    gBossStatus.unk4 = 0x24;
    gBossStatus.unk8 = Boss_GetStartHealth();
    gBossStatus.unkA = Boss_GetStartHealth();
    func_02021310(ctx, data_ov000_02365b30, gZoneActArchive);
    w->unk240 = func_02021264(ctx, 8);
    w->unk244 = func_02021264(ctx, 9);
    func_020212e0(ctx);
    w->unk23C = data_ov000_02369b7c[0];
    w->unk248 = data_ov000_02369b7c[1];
    MapObject_Init(w, (s32)p, b, c);
    w->base.unk70 = Boss7_Draw;
    w->base.unk54 |= 0x12;
    w->base.unk58 |= 0xa300;
    w->base.unk5C |= 0x20;
    w->unk210 = 1;
    w->base.unk10 = 0xa000;
    w->base.unk14 = 0x20000;
    w->base.unk18 = 0;
    res = NNS_G3dGetMdlSet(w->unk23C);
    NNSi_G3dModifyMatFlag(res + *(u32 *)(res + *(u16 *)(res + 0xe) + 0xc), 1, 0x40);
    res = NNS_G3dGetMdlSet(w->unk23C);
    NNSi_G3dModifyMatFlag(res + *(u32 *)(res + *(u16 *)(res + 0xe) + 0xc), 1, 0x80);
    res = NNS_G3dGetMdlSet(w->unk248);
    NNSi_G3dModifyMatFlag(res + *(u32 *)(res + *(u16 *)(res + 0xe) + 0xc), 1, 0x40);
    res = NNS_G3dGetMdlSet(w->unk248);
    NNSi_G3dModifyMatFlag(res + *(u32 *)(res + *(u16 *)(res + 0xe) + 0xc), 1, 0x80);
    sub = &w->unk24C;
    Model_Init(sub, 0);
    Model_SetResource(sub, w->unk23C, 0);
    Model_SetAnimation(sub, 0, w->unk240, 3, 0);
    Model_SetAnimation(sub, 4, w->unk244, gGameState.unk0 == 0 ? 4 : 3, 0);
    sub->unk10C |= 4;
    sub->unk110 |= 4;
    sub->unk48 = w->base.unk10 << 4;
    sub->unk4C = -(w->base.unk14 << 4);
    sub->unk50 = w->base.unk18 << 4;
    sub->unk18 = sub->unk1C = sub->unk20 = 0x34cc;
    func_02021310(ctx2, data_ov000_02365b34, gZoneActArchive);
    NNS_G3dResDefaultSetup(func_0202128c(data_ov000_02365b38));
    NNS_G3dResDefaultSetup(func_0202128c(data_ov000_02365b4c));
    NNS_G3dResDefaultSetup(func_0202128c(data_ov000_02365b64));
    NNS_G3dResDefaultSetup(func_0202128c(data_ov000_02365b78));
    NNS_G3dResDefaultSetup(func_0202128c(data_ov000_02365b90));
    NNS_G3dResDefaultSetup(func_0202128c(data_ov000_02365ba8));
    NNS_G3dResDefaultSetup(func_0202128c(data_ov000_02365bc0));
    NNS_G3dResDefaultSetup(func_0202128c(data_ov000_02365bd4));
    NNS_G3dResDefaultSetup(func_0202128c(data_ov000_02365bf0));
    NNS_G3dResDefaultSetup(func_0202128c(data_ov000_02365c0c));
    NNS_G3dResDefaultSetup(func_0202128c(data_ov000_02365c28));
    NNS_G3dResDefaultSetup(func_0202128c(data_ov000_02365c3c));
    func_020212e0(ctx2);
    w->unk214 = MapObject_Spawn(0xd5, b, c, 0, 0, 0, 0, 0, 0);
    return w;
}
