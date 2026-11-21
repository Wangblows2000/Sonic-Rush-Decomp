#include <ov000_b/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_Blk blk[3];
    u8 pad41C[0x42C - 0x41C];
} UnkStruct_0231a0f0;

extern void *data_ov000_02369ba0;
extern UnkStruct_0231a0f0 *data_ov000_0236a2fc;
extern u8 data_ov000_0236a308[];
extern u8 data_ov000_02367128[];
extern u8 data_ov000_0236713c[];
extern u8 data_ov000_02367150[];
extern u8 data_ov000_023661c0[];

void GmkTable_Destructor(void);
void GmkTable_Action_Init(UnkStruct_0231a0f0 *w);

UnkStruct_0231a0f0 *GmkTable_Create(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_0231a0f0 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, GmkTable_Destructor, 0, 0x500, 2, sizeof(UnkStruct_0231a0f0));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    data_ov000_0236a2fc = w;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0231a0f0));
    MapObject_Init(w, a, b, c);
    StageObj_LoadModelSimple(w, &w->blk[0], data_ov000_02367128, 0, 0, 0, data_ov000_0236a308, data_ov000_02369ba0);
    StageObj_LoadModelSimple(w, &w->blk[1], data_ov000_0236713c, 0, 1, 2, data_ov000_0236a308, data_ov000_02369ba0);
    StageObj_LoadModelSimple(w, &w->blk[2], data_ov000_02367150, 0, 2, 1, data_ov000_0236a308, data_ov000_02369ba0);
    w->blk[1].unkA8 |= 0x40;
    w->base.unk5C |= 0x200;
    Obj_SetVec(&w->base.unk128->unk18, 0x34cc, 0x34cc, 0x34cc);
    w->base.spr0.unk46 = 0x14;
    w->base.spr1.unk46 = 0x14;
    StageObj_SetMapHitbox(w, -8, -10, 8, 0xc);
    w->base.unk1B8 = w;
    w->base.unk1C8 = data_ov000_023661c0;
    w->base.unk1D8 = 0x20;
    w->base.unk1DA = 1;
    w->base.unk1D4 = -0x80;
    w->base.unk1D6 = -0x14;
    w->base.unk1C4 |= 1;
    w->base.unk58 |= 0x2040;
    w->base.unk58 |= 0x820080;
    w->base.unk5C &= ~1;
    w->base.unk58 |= 0x400;
    w->base.unk54 &= ~1;
    w->base.unk4 = 0;
    w->base.unk3C = 0x40;
    w->base.unk3E = 0x300;
    w->base.unk204 = 0;
    w->base.unk1DC = Sound_AllocHandle();
    GmkTable_Action_Init(w);
    return w;
}
