#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x138 - 0x24];
} UnkStruct_022ee43c_Sub;

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_022ee43c_Sub unk20C;
    u32 unk344;
    u8 pad348[4];
    void *unk34C;
    void *unk350;
    void *unk354;
    void *unk358;
    void *unk35C;
    void *unk360;
    void *unk364;
} UnkStruct_022ee43c;

extern void *gZoneActArchive;
extern u8 data_ov000_02365ae4[];

void Boss7PartD7_Destructor(void);
void Boss7PartD7_State_Unk_ov000_022ebfac(void);
void Boss7PartD7_Draw(void);
void func_02021310(void *f, void *path, void *arc);
void *func_02021264(void *f, s32 i);
void func_020212e0(void *f);
void Model_Init(void *s, s32 a);
void Model_SetResource(void *s, void *res, s32 idx);

UnkStruct_022ee43c *Boss7PartD7_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    u32 ctx[0x17];
    UnkStruct_022ee43c *w;
    UnkStruct_022ee43c_Sub *sub;
    vu16 zero;

    w = Task_Create(StageObj_Main, Boss7PartD7_Destructor, 0, 0x502, 2, sizeof(UnkStruct_022ee43c))->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_022ee43c));
    w->base.unk68 = Boss7PartD7_State_Unk_ov000_022ebfac;
    w->unk344 = p->unk4;
    func_02021310(ctx, data_ov000_02365ae4, gZoneActArchive);
    if (p->unk4 == 0) {
        w->unk34C = func_02021264(ctx, 0x20);
        w->unk350 = func_02021264(ctx, 0x21);
        w->unk35C = func_02021264(ctx, 0x22);
        w->unk354 = func_02021264(ctx, 0x23);
        w->unk360 = func_02021264(ctx, 0x24);
        w->unk358 = func_02021264(ctx, 0x25);
        w->unk364 = func_02021264(ctx, 0x26);
    } else {
        w->unk34C = func_02021264(ctx, 0x27);
        w->unk350 = func_02021264(ctx, 0x28);
        w->unk35C = func_02021264(ctx, 0x29);
        w->unk354 = func_02021264(ctx, 0x2a);
        w->unk360 = func_02021264(ctx, 0x2b);
        w->unk358 = func_02021264(ctx, 0x2c);
        w->unk364 = func_02021264(ctx, 0x2d);
    }
    func_020212e0(ctx);
    MapObject_Init(w, (s32)p, b, c);
    w->base.unk70 = Boss7PartD7_Draw;
    w->base.unk54 |= 0x10;
    w->base.unk58 |= 0x3300;
    w->base.unk5C |= 0x20;
    sub = &w->unk20C;
    Model_Init(sub, 0);
    Model_SetResource(sub, w->unk34C, 0);
    sub->unk18 = sub->unk1C = sub->unk20 = 0x34cc;
    return w;
}
