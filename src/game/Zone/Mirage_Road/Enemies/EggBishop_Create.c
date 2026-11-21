#include <game/Stage/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x240 - 0x20C];
} UnkStruct_022fc0bc;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366378[];
extern u8 data_ov000_02369dcc[];

void MapObject_Destructor();
void EggBishop_Action_Spawned();

UnkStruct_022fc0bc *EggBishop_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_022fc0bc *w;
    vu16 zero;
    s32 k;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0x500, 2, sizeof(UnkStruct_022fc0bc));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_022fc0bc));
    MapObject_Init(w, (s32)p, b, c);
    StageObj_Load2dBAC(w, data_ov000_02366378, 5, data_ov000_02369dcc, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0xe);
    StageObj_SetOAMOrder(w, 0x15);
    if (!(w->base.unk1E0->unk4 & 1))
        w->base.unk5C &= ~1;
    w->base.unk2E = 0x300;
    if (w->base.unk5C & 1)
        w->base.unk2E = -w->base.unk2E;
    k = w->base.unk1E0->unk4 & 0xc;
    if (k != 0)
        w->base.unk2E = (w->base.unk2E * (((k >> 2) << 7) + 0x100)) >> 8;
    w->base.unk58 |= 0x10;
    w->base.unk58 |= 0x100;
    w->base.unk5C |= 0x100;
    EggBishop_Action_Spawned(w);
    return w;
}
