#include <ov000_b/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_Spr s0;
    UnkStruct_Spr s1;
    UnkStruct_Spr s2;
    UnkStruct_Spr s3;
    u8 pad32C[0x3D0 - 0x32C];
} UnkStruct_0231cb18;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02367224[];
extern u8 data_ov000_0236a388[];
extern u8 data_ov000_0236723c[];
extern u8 data_ov000_0236a390[];
extern u8 data_ov000_02367254[];
extern u8 data_ov000_0236a3a0[];

void func_ov000_0231caac(void);
void func_ov000_0231c9ac(void);
void func_ov000_0231c3ac(void);
void func_ov000_0231c36c(void);
void StageObj_SetAnimation(void *w, s32 a);
void func_02068e40(void *w, s32 a, s32 b, s32 c, s32 d);
void func_ov000_0231c99c(UnkStruct_0231cb18 *w);

UnkStruct_0231cb18 *func_ov000_0231cb18(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_0231cb18 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, func_ov000_0231caac, 0, 0xf6, 2, sizeof(UnkStruct_0231cb18));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0231cb18));
    func_ov000_022fba50(w, a, b, c);
    StageObj_Load2dBAC(w, data_ov000_02367224, 0x11, data_ov000_0236a388, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x18);
    w->base.spr0.unk46 = 0x14;
    w->base.spr1.unk46 = 0x14;
    w->base.unk70 = func_ov000_0231c9ac;
    StageObj_SetAnimation(w, 0);

    ObjAction_Load2dBAC(&w->s0, &w->s1, data_ov000_0236723c, 0, data_ov000_0236a388, data_ov000_02369ba0);
    AnimatorSprite_SetAnimation(&w->s0, 3);
    AnimatorSprite_SetAnimation(&w->s1, 3);
    w->s0.unk46 = 0x14;
    w->s1.unk46 = 0x14;
    w->s0.unk44 = w->base.spr0.unk44;
    w->s1.unk44 = w->base.spr0.unk44;
    func_02069e68(&w->s0, &w->s1, 1, data_ov000_0236a390);

    ObjAction_Load2dBAC(&w->s2, &w->s3, data_ov000_02367254, 0, data_ov000_0236a388, data_ov000_02369ba0);
    AnimatorSprite_SetAnimation(&w->s2, 2);
    AnimatorSprite_SetAnimation(&w->s3, 2);
    w->s2.unk46 = 0x13;
    w->s3.unk46 = 0x13;
    w->s2.unk44 = w->base.spr0.unk44;
    w->s3.unk44 = w->base.spr0.unk44;
    func_02069e68(&w->s2, &w->s3, 4, data_ov000_0236a3a0);

    ObjRect_SetAtkDefFlags(&w->base.coll0, 0, -0x10);
    w->base.coll0.unk1C = func_ov000_0231c3ac;
    w->base.coll0.unkA |= 0x800;
    ObjRect_SetAtkDefFlags(&w->base.coll1, 0, -0x10);
    w->base.coll1.unk1C = func_ov000_0231c36c;
    w->base.coll1.unkA |= 0x800;
    w->base.coll1.unk28 = 0x33;
    func_02068e40(w, -0xc, -0xc, 0xc, 0xc);
    w->base.unk58 |= 0x2100;
    func_ov000_0231c99c(w);
    return w;
}
