#include <ov000_b/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_Spr s0;
    UnkStruct_Spr s1;
    UnkStruct_Spr s2;
    UnkStruct_Spr s3;
} UnkStruct_02317ac8;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366fd8[];
extern u8 data_ov000_0236a264[];
extern u8 data_ov000_02366ff0[];
extern u8 data_ov000_02367008[];

void Swing_Destructor(void);
void Swing_Draw(void);
void Swing_OnHit(void);
BOOL func_ov000_02345758(s32 x, s32 y);
void func_ov000_0231797c(UnkStruct_02317ac8 *w);

UnkStruct_02317ac8 *Swing_Create(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_02317ac8 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, Swing_Destructor, 0, 0x500, 2, sizeof(UnkStruct_02317ac8));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02317ac8));
    MapObject_Init(w, a, b, c);
    StageObj_Load2dBAC(w, data_ov000_02366fd8, 2, data_ov000_0236a264, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x2c);
    w->base.spr0.unk46 = 0x14;
    w->base.spr1.unk46 = 0x14;
    w->base.unk70 = Swing_Draw;

    ObjAction_Load2dBAC(&w->s0, &w->s1, data_ov000_02366ff0, 0xb, data_ov000_0236a264, data_ov000_02369ba0);
    AnimatorSprite_SetAnimation(&w->s0, 1);
    AnimatorSprite_SetAnimation(&w->s1, 1);
    w->s0.unk46 = 9;
    w->s1.unk46 = 9;
    w->s0.unk2C &= ~0x60000;
    w->s1.unk2C &= ~0x60000;
    w->s0.unk2C |= 0x20000;
    w->s1.unk2C |= 0x20000;
    w->s0.unk44 = w->base.spr0.unk44;
    w->s1.unk44 = w->base.spr0.unk44;
    w->s0.unk2C |= 0x200;
    w->s1.unk2C |= 0x200;

    ObjAction_Load2dBAC(&w->s2, &w->s3, data_ov000_02367008, 0xa, data_ov000_0236a264, data_ov000_02369ba0);
    AnimatorSprite_SetAnimation(&w->s2, 2);
    AnimatorSprite_SetAnimation(&w->s3, 2);
    w->s2.unk46 = 0x15;
    w->s3.unk46 = 0x15;
    w->s2.unk44 = w->base.spr0.unk44;
    w->s3.unk44 = w->base.spr0.unk44;
    w->s2.unk2C |= 0x200;
    w->s3.unk2C |= 0x200;

    ObjRect_SetBox2D(&w->base.coll0, -4, 0x30, 4, 0x38);
    ObjRect_SetAtkDefFlags(&w->base.coll0, 0, -0x10);
    w->base.coll0.unk1C = Swing_OnHit;
    w->base.coll0.unkA |= 0x800;
    w->base.coll0.unk18 = w;
    ObjRect_SetBox2D(&w->base.coll1, -4, -0x38, 4, -0x30);
    ObjRect_SetAtkDefFlags(&w->base.coll1, 0, -0x10);
    w->base.coll1.unk1C = Swing_OnHit;
    w->base.coll1.unkA |= 0x800;
    w->base.coll1.unk18 = w;
    w->base.unk58 |= 0x2100;
    w->base.unk5C |= 0x100;
    if (func_ov000_02345758(w->base.unk10, w->base.unk14))
        w->base.unk5 = 0x80;
    func_ov000_0231797c(w);
    return w;
}
