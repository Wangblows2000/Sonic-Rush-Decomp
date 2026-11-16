#include <ov000_b/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366ddc[], data_ov000_0236a1bc[], data_ov000_0236a1c4[], data_ov000_0236a1d4[];

void RockBreakPart_Action_Unk_ov000_023136dc(UnkStruct_EffObj *w);

void RockBreak_CreatePart_ov000_023136ec(UnkStruct_StageObj *arg, s16 x, s16 y, s32 type) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0x500, 2, sizeof(UnkStruct_EffObj));
    if (task == OS_GetArenaLo(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_02366ddc, 0, data_ov000_0236a1bc, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 6);
    if (type != 0) {
        StageObj_SetSharedSpriteVram(w, 1, data_ov000_0236a1d4);
        StageObj_SetAnimation(w, 2);
    } else {
        StageObj_SetSharedSpriteVram(w, 2, data_ov000_0236a1c4);
        StageObj_SetAnimation(w, 1);
    }
    w->spr0.unk46 = 0x14;
    w->spr1.unk46 = 0x14;
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk2E = x;
    w->unk30 = y;
    w->unk40 = 0x2a;
    w->unk42 = 0xf00;
    w->unk54 |= 1;
    w->unk58 |= 0x180;
    RockBreakPart_Action_Unk_ov000_023136dc(w);
}
