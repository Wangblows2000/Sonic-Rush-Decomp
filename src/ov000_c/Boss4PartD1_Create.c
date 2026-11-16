#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x48 - 0x24];
    UnkStruct_Vec unk48;
    u8 pad54[0x138 - 0x54];
} UnkStruct_022df094_Sub;

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_022df094_Sub unk20C;
    u8 pad344[0x34C - 0x344];
    u16 unk34C;
    u8 pad34E[0x3C8 - 0x34E];
} UnkStruct_022df094;

typedef struct {
    u8 pad0[0xBAC];
    UnkStruct_Vec unkBAC;
    u8 padBB8[0xBDC - 0xBB8];
    s32 unkBDC;
} UnkStruct_02369b2c;

extern UnkStruct_02369b2c *gBoss4;
extern void *data_ov000_02369b30[];

void Boss4PartD1_Destructor(void);
void Boss4PartD1_Draw(void);
void Boss4PartD1_State_Unk_ov000_022dc568(void);
void Boss4PartD1_Collide(void);
void MI_CpuFill8(void *p, s32 v, u32 n);
void Model_Init(void *s, s32 a);
void Model_SetResource(void *s, void *res, s32 idx);
void func_ov000_022db7ec(UnkStruct_Vec *v, UnkStruct_022df094 *w);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void func_ov000_022dc2ec(UnkStruct_022df094 *w);

UnkStruct_022df094 *Boss4PartD1_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_02369b2c *boss = gBoss4;
    UnkStruct_022df094 *w;
    UnkStruct_022df094_Sub *sub;

    if (boss->unkBDC >= 1)
        return NULL;
    boss->unkBDC++;
    w = Task_Create(StageObj_Main, Boss4PartD1_Destructor, 0, 0x504, 2, sizeof(UnkStruct_022df094))->work;
    MI_CpuFill8(w, 0, sizeof(UnkStruct_022df094));
    MapObject_Init(w, (s32)p, b, c);
    w->base.unk70 = Boss4PartD1_Draw;
    w->base.unk68 = Boss4PartD1_State_Unk_ov000_022dc568;
    w->base.unk78 = Boss4PartD1_Collide;
    w->base.unk54 |= 0x10;
    w->base.unk58 |= 0xa300;
    sub = &w->unk20C;
    Model_Init(sub, 0);
    Model_SetResource(sub, data_ov000_02369b30[5], 0);
    sub->unk18 = sub->unk1C = sub->unk20 = 0x34cc;
    sub->unk48 = boss->unkBAC;
    func_ov000_022db7ec(&sub->unk48, w);
    w->unk34C = 0;
    Sound_PlaySeqArc(0, 0, 0x8a);
    func_ov000_022dc2ec(w);
    return w;
}
