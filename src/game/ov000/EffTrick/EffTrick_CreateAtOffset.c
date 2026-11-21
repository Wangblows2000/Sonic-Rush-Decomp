#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
} UnkStruct_022c4560;

extern void *gActComArchive;
extern UnkStruct_022c4560 gGameState;
extern u8 data_ov000_02368284[], data_ov000_0236aa18[];

BOOL Player_IsPlayer2(UnkStruct_StageObj *a);
void Effect_SetMoveUntilAnimEndState(UnkStruct_EffObj *w);

void EffTrick_CreateAtOffset(UnkStruct_StageObj *arg, s32 x, s32 y) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    if (Player_IsPlayer2(arg))
        return;
    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == OS_GetArenaLo(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_02368284, 5, data_ov000_0236aa18, gActComArchive);
    StageObj_AllocSpritePalette(w, 0xc, 0xc);
    StageObj_SetAnimation(w, 0xc);
    StageObj_SetOAMOrder(w, 9);
    StageObj_SetOAMPriority(w, 1);
    if (gGameState.unk18 & 0x20)
        w->spr1.unk2C |= 0x80008;
    if (arg->unk5C & 1)
        w->unk5C |= 1;
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk2E = 0;
    w->unk30 = 0;
    if (!(w->unk5C & 1)) {
        w->unk10 += x << 8;
    } else {
        w->unk10 -= x << 8;
        w->unk2E = -w->unk2E;
    }
    w->unk14 += y << 8;
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    Effect_SetMoveUntilAnimEndState(w);
}
