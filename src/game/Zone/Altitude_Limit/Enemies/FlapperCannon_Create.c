#include <game/Stage/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
} UnkStruct_022ff50c;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366544[];
extern u8 data_ov000_02369df4[];

void MapObject_Destructor();

UnkStruct_022ff50c *FlapperCannon_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_022ff50c *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0x500, 2, sizeof(UnkStruct_022ff50c));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_022ff50c));
    MapObject_Init(w, (s32)p, b, c);
    StageObj_Load2dBAC(w, data_ov000_02366544, 1, data_ov000_02369df4, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x29);
    StageObj_SetAnimation(w, 4);
    w->base.unk5C |= 4;
    StageObj_SetOAMOrder(w, 0x15);
    w->base.unk2E = -0x300;
    w->base.unk30 = 0x300;
    if (!(w->base.unk1E0->unk4 & 1)) {
        w->base.unk5C &= ~1;
        w->base.unk2E = -w->base.unk2E;
    }
    w->base.unk58 |= 0x110;
    w->base.unk5C |= 0x100;
    w->base.unk5C |= 4;
    return w;
}
