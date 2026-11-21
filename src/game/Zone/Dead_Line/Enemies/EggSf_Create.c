#include <game/Stage/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
} UnkStruct_022fc230;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366394[];
extern u8 data_ov000_02369dbc[];

void MapObject_Destructor();
void Enemy_Action_StartPopOnContact();

UnkStruct_022fc230 *EggSf_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_022fc230 *w;
    vu16 zero;
    s32 k;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0x500, 2, sizeof(UnkStruct_022fc230));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_022fc230));
    MapObject_Init(w, (s32)p, b, c);
    StageObj_Load2dBAC(w, data_ov000_02366394, 2, data_ov000_02369dbc, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x27);
    StageObj_SetOAMOrder(w, 0x15);
    if (!(w->base.unk1E0->unk4 & 1))
        w->base.unk5C &= ~1;
    w->base.unk2E = 0xa00;
    if (w->base.unk5C & 1)
        w->base.unk2E = -w->base.unk2E;
    k = w->base.unk1E0->unk4 & 0xc;
    if (k != 0)
        w->base.unk2E = (w->base.unk2E * (((k >> 2) << 7) + 0x100)) >> 8;
    w->base.unk58 |= 0x110;
    w->base.unk5C |= 0x100;
    Enemy_Action_StartPopOnContact(w);
    return w;
}
