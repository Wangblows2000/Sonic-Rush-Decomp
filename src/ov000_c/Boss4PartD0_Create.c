#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x48 - 0x24];
    UnkStruct_Vec unk48;
    u8 pad54[0x138 - 0x54];
} UnkStruct_022df200_Sub;

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_022df200_Sub unk20C;
    UnkStruct_022df200_Sub unk344;
    u8 pad47C[0x484 - 0x47C];
    s32 unk484;
    u8 pad488[0x490 - 0x488];
} UnkStruct_022df200;

typedef struct {
    u8 pad0[0xAB0];
    void *unkAB0[17];
    u8 padAF4[0xB7C - 0xAF4];
    UnkStruct_Vec unkB7C;
} UnkStruct_02369b2c;

extern UnkStruct_02369b2c *gBoss4;

void Boss4PartD0_Destructor(void);
void Boss4PartD0_Draw(void);
void Boss4PartD0_State_Unk_ov000_022dcb18(void);
void Boss4PartD0_Collide(void);
void MI_CpuFill8(void *p, s32 v, u32 n);
void Model_Init(void *s, s32 a);
void Model_SetResource(void *s, void *res, s32 idx);
void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);
void func_ov000_022db7ec(UnkStruct_Vec *v, UnkStruct_022df200 *w);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void func_ov000_022dc82c(UnkStruct_022df200 *w);

UnkStruct_022df200 *Boss4PartD0_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_02369b2c *boss = gBoss4;
    UnkStruct_022df200 *w;
    UnkStruct_022df200_Sub *sub;

    w = Task_Create(StageObj_Main, Boss4PartD0_Destructor, 0, 0x503, 2, sizeof(UnkStruct_022df200))->work;
    MI_CpuFill8(w, 0, sizeof(UnkStruct_022df200));
    MapObject_Init(w, (s32)p, b, c);
    w->base.unk70 = Boss4PartD0_Draw;
    w->base.unk68 = Boss4PartD0_State_Unk_ov000_022dcb18;
    w->base.unk78 = Boss4PartD0_Collide;
    w->base.unk54 |= 0x10;
    w->base.unk58 |= 0xa300;
    sub = &w->unk20C;
    Model_Init(sub, 0);
    Model_SetResource(sub, boss->unkAB0[3], 0);
    sub->unk18 = sub->unk1C = sub->unk20 = 0x34cc;
    sub->unk48 = boss->unkB7C;
    func_ov000_022db7ec(&sub->unk48, w);
    sub = &w->unk344;
    Model_Init(sub, 0);
    Model_SetResource(sub, boss->unkAB0[9], 0);
    Model_SetAnimation(sub, 0, boss->unkAB0[10], 0, 0);
    Model_SetAnimation(sub, 1, boss->unkAB0[11], 0, 0);
    sub->unk18 = sub->unk1C = sub->unk20 = 0x34cc;
    sub->unk48 = boss->unkB7C;
    w->unk484 = boss->unkB7C.y + 0x200000;
    Sound_PlaySeqArc(0, 0, 0x89);
    func_ov000_022dc82c(w);
    return w;
}
