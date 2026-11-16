#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
} UnkStruct_022ff920;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366584[];
extern u8 data_ov000_02369e0c[];

void MapObject_Destructor();

UnkStruct_022ff920 *EggFlapperLaser_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_022ff920 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0x500, 2, sizeof(UnkStruct_022ff920));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_022ff920));
    MapObject_Init(w, (s32)p, b, c);
    StageObj_Load2dBAC(w, data_ov000_02366584, 2, data_ov000_02369e0c, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x29);
    StageObj_SetAnimation(w, 4);
    w->base.unk5C |= 4;
    StageObj_SetOAMOrder(w, 0x15);
    if (!(w->base.unk1E0->unk4 & 1))
        w->base.unk5C &= ~1;
    w->base.unk30 = 0x600;
    w->base.unk58 |= 0x110;
    w->base.unk5C |= 0x100;
    return w;
}
