#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_Blk blk[9];
} UnkStruct_0230ba48;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366984[], data_ov000_02369f64[], data_ov000_02369f5c[], data_ov000_0236699c[];
extern u8 data_ov000_023661c0[], data_ov000_023661a4[];

void Hand2d_Destructor(void);
void Hand2d_Action_Init(UnkStruct_0230ba48 *w);

UnkStruct_0230ba48 *Hand2d_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_0230ba48 *w;
    vu16 zero;
    u16 i;

    task = Task_Create(StageObj_Main, Hand2d_Destructor, 0, 0x500, 2, sizeof(UnkStruct_0230ba48));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0230ba48));
    MapObject_Init(w, (s32)p, b, c);
    StageObj_Load2dBAC(w, data_ov000_02366984, 0xffff, data_ov000_02369f64, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x4e);
    StageObj_SetOAMOrder(w, 0x14);
    for (i = 0; i < 9; i++)
        StageObj_LoadModelSimple(w, &w->blk[i], data_ov000_0236699c, 0, i, i, data_ov000_02369f5c, data_ov000_02369ba0);
    w->base.unk5C |= 0x200;
    Obj_SetVec(&w->base.unk128->unk18, 0x34cc, 0x34cc, 0x34cc);
    ObjRect_SetBox2D(&w->base.coll0, -0xa4, -0x50, -0x80, 0x10);
    w->base.unk1B8 = w;
    w->base.unk1C8 = data_ov000_023661c0;
    w->base.unk1D0 = data_ov000_023661a4;
    w->base.unk1D8 = 6;
    w->base.unk1DA = 3;
    w->base.unk1D4 = -0x18;
    w->base.unk1D6 = 2;
    w->base.unk58 |= 0x2100;
    w->base.unk204 = 0;
    Hand2d_Action_Init(w);
    return w;
}
