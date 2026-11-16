#include <nitro/types.h>

typedef struct UnkStruct_022ccc38_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x18 - 0xc];
    void *unk18;
    void (*unk1C)(void);
    u32 unk20;
    u8 pad2[0x28 - 0x24];
    u8 unk28;
    u8 pad3[0x2c - 0x29];
} UnkStruct_022ccc38_h;

typedef struct UnkStruct_022ccc38_a {
    u8 pad[8];
    u16 unk8;
    u8 unkA;
    u8 unkB;
    u8 pad1[0x18 - 0xc];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad2[0x48 - 0x24];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad3[0x90 - 0x54];
    u8 unk90[4];
} UnkStruct_022ccc38_a;

typedef struct UnkStruct_022ccc38 {
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
    void (*unk78)(void);
    u8 pad2[0x21c - 0x7c];
    s32 unk21C;
    void *unk220;
    void *unk224;
    UnkStruct_022ccc38_a unk228;
} UnkStruct_022ccc38;

typedef struct UnkTask_022ccc38 {
    u8 pad[0x10];
    UnkStruct_022ccc38 *unk10;
} UnkTask_022ccc38;

typedef struct UnkStruct_022ccc38_6 {
    u8 pad[4];
    s32 unk4;
    s32 unk8;
} UnkStruct_022ccc38_6;

extern void *gZoneActArchive;
extern u8 data_ov000_0236505c[];
extern UnkStruct_022ccc38_6 data_ov000_02369ad0;

typedef struct UnkStruct_022ccc38_g {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
} UnkStruct_022ccc38_g;

extern UnkStruct_022ccc38_g *gBoss1;

UnkTask_022ccc38 *Task_Create(void (*fn)(void), void (*fn2)(void), s32 b, s32 c, s32 d, s32 e);
void StageObj_Main(void);
void Boss1Neck_Destructor(void);
void Boss1Neck_State_Idle(void);
void Boss1Neck_Draw(void);
void Boss1Neck_NodeCallback_NeckHam(void);
void Boss1Hammer_State_Idle(void);
void Boss1Hammer_Draw(void);
void Boss1Hammer_Collide(void);
void Boss1Hammer_OnHit_Damage(void);
void Boss1Hammer_OnHit_PushPlayer(void);
void Boss1Hammer_NodeCallback_Hammer(void);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void NNS_FndMountArchive(void *ctx, void *a, void *b);
void *NNS_FndGetArchiveFileByIndex(void *ctx, u32 id);
void NNS_FndUnmountArchive(void *ctx);
BOOL func_ov000_022cc6d4(void);
void MapObject_Init(UnkStruct_022ccc38 *s, s32 a, s32 x, s32 y);
void ObjRect_SetBox3D(UnkStruct_022ccc38_h *h, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void ObjRect_SetAtkDefFlags(UnkStruct_022ccc38_h *h, s32 a, s32 b);
void Model_Init(void *p, s32 a);
void Model_SetResource(void *p, s32 a, s32 b);
void NNS_G3dRenderObjSetCallBack(void *p, void (*fn)(void), s32 a, s32 b, s32 c);

static inline void MI_CpuFill16(void *dest, u16 data, u32 size) {
    MIi_CpuClear16(*(volatile u16 *)&data, dest, size);
}

UnkStruct_022ccc38 *Boss1Neck_Create(s32 a, s32 x, s32 y) {
    u32 ctx[0x5c / 4];
    UnkStruct_022ccc38 *s;
    UnkStruct_022ccc38_g *g = gBoss1;
    UnkStruct_022ccc38_a *p;
    s = Task_Create(StageObj_Main, Boss1Neck_Destructor, 0, 0x501, 2, 0x360)->unk10;
    MI_CpuFill16(s, 0, 0x360);
    s->unk68 = Boss1Neck_State_Idle;
    NNS_FndMountArchive(ctx, data_ov000_0236505c, gZoneActArchive);
    if (func_ov000_022cc6d4()) {
        s->unk220 = NNS_FndGetArchiveFileByIndex(ctx, 8);
        s->unk224 = NNS_FndGetArchiveFileByIndex(ctx, 9);
    } else {
        s->unk220 = NNS_FndGetArchiveFileByIndex(ctx, 8);
    }
    NNS_FndUnmountArchive(ctx);
    s->unk21C = data_ov000_02369ad0.unk4;
    MapObject_Init(s, a, x, y);
    s->unk70 = Boss1Neck_Draw;
    s->unk54 |= 0x12;
    s->unk58 |= 0x8300;
    s->unk5C |= 0x20;
    s->unk10 = g->unk10;
    s->unk14 = g->unk14;
    s->unk18 = g->unk18 - 0x8e00;
    p = &s->unk228;
    Model_Init(p, 0);
    Model_SetResource(p, s->unk21C, 0);
    NNS_G3dRenderObjSetCallBack(p->unk90, Boss1Neck_NodeCallback_NeckHam, 0, 6, 3);
    p->unk48 = g->unk10 << 4;
    p->unk4C = -(g->unk14 << 4);
    p->unk50 = (g->unk18 - 0x8e00) << 4;
    p->unk18 = p->unk1C = p->unk20 = 0x34cc;
    p->unkA = 4;
    return s;
}
