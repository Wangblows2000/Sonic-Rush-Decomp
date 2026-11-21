#include <game/Stage/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_Blk blk;
    u8 pad2BC[0xA2C - 0x2BC];
} UnkStruct_023054e8;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_023667e4[], data_ov000_02369e9c[], data_ov000_02369e94[], data_ov000_023667fc[];

void MapObject_Destructor(void);
void CranePartEE_Draw(void);

UnkStruct_023054e8 *CranePartEE_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_023054e8 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0x500, 2, sizeof(UnkStruct_023054e8));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_023054e8));
    MapObject_Init(w, (s32)p, b, c);
    StageObj_Load2dBAC(w, data_ov000_023667e4, 0x10, data_ov000_02369e9c, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x36);
    StageObj_SetOAMOrder(w, 0x14);
    StageObj_SetAnimation(w, 3);
    w->base.spr0.unk2C |= 0x80000;
    StageObj_LoadModelSimple(w, &w->blk, data_ov000_023667fc, 0, 2, 0xb, data_ov000_02369e94, data_ov000_02369ba0);
    w->base.unk5C |= 0x200;
    Obj_SetVec(&w->base.unk128->unk18, 0x34cc, 0x34cc, 0x34cc);
    w->base.unk70 = CranePartEE_Draw;
    w->base.unk58 |= 0x2100;
    w->base.unk54 |= 0x10;
    return w;
}
