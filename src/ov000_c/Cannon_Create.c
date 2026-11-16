#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_Spr sprA;
    UnkStruct_Spr sprB;
    UnkStruct_Spr sprC;
    UnkStruct_Spr sprD;
} UnkStruct_023047f0;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366730[], data_ov000_02369e5c[], data_ov000_02366748[], data_ov000_02366760[];

void Cannon_Destructor(void);
void Cannon_Draw(void);
void Cannon_OnHit(void);
void func_ov000_023046c0(UnkStruct_023047f0 *w);

UnkStruct_023047f0 *Cannon_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_023047f0 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, Cannon_Destructor, 0, 0x500, 2, sizeof(UnkStruct_023047f0));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_023047f0));
    MapObject_Init(w, (s32)p, b, c);
    StageObj_Load2dBAC(w, data_ov000_02366730, 0xffff, data_ov000_02369e5c, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x2b);
    StageObj_SetAnimation(w, 0);
    w->base.spr0.unk46 = 0x14;
    w->base.spr1.unk46 = 0x14;
    ObjAction_Load2dBAC(&w->sprC, &w->sprD, data_ov000_02366748, 0xffff, data_ov000_02369e5c, data_ov000_02369ba0);
    AnimatorSprite_SetAnimation(&w->sprC, 1);
    AnimatorSprite_SetAnimation(&w->sprD, 1);
    w->sprC.unk46 = 0x15;
    w->sprD.unk46 = 0x15;
    w->sprC.unk44 = w->base.spr0.unk44;
    w->sprD.unk44 = w->base.spr0.unk44;
    w->sprC.unk2C |= 0x200;
    w->sprD.unk2C |= 0x200;
    ObjAction_Load2dBAC(&w->sprA, &w->sprB, data_ov000_02366760, 0xffff, data_ov000_02369e5c, data_ov000_02369ba0);
    AnimatorSprite_SetAnimation(&w->sprA, 2);
    AnimatorSprite_SetAnimation(&w->sprB, 2);
    w->sprA.unk46 = 0x16;
    w->sprB.unk46 = 0x16;
    w->sprA.unk44 = w->base.spr0.unk44;
    w->sprB.unk44 = w->base.spr0.unk44;
    w->base.unk70 = Cannon_Draw;
    ObjRect_SetAtkDefFlags(&w->base.coll0, 0, -16);
    w->base.coll0.unk1C = Cannon_OnHit;
    w->base.coll0.unkA |= 0x800;
    w->base.unk58 |= 0x2100;
    func_ov000_023046c0(w);
    return w;
}
