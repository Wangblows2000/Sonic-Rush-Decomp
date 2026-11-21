#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
} UnkStruct_022c4560;

extern void *gActComArchive;
extern UnkStruct_022c4560 gGameState;
extern UnkStruct_Task *gFixVsPushIconTask;
extern u8 data_ov000_02368240[], data_ov000_0236a9a8[];

void FixVsPushIcon_Destructor(void);
void FixVsPushIcon_State_FollowPlayer(void);

void FixVsPushIcon_Create(UnkStruct_StageObj *arg) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    if (gFixVsPushIconTask != NULL)
        return;
    task = Task_Create(StageObj_Main, FixVsPushIcon_Destructor, 0, 0xf6, 4, sizeof(UnkStruct_EffObj));
    if (task == OS_GetArenaLo(0))
        return;
    gFixVsPushIconTask = task;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_02368240, 0xffff, data_ov000_0236a9a8, gActComArchive);
    w->spr0.unk44 = 1;
    w->spr1.unk44 = 1;
    StageObj_SetOAMOrder(w, 3);
    StageObj_SetOAMPriority(w, 0);
    StageObj_SetAnimation(w, 3);
    if (gGameState.unk18 & 0x20)
        w->spr1.unk2C |= 0x80008;
    w->unk5C |= 4;
    w->unk5C |= 1;
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14 - 0x2000;
    w->unk88 = arg;
    w->unk58 |= 0x100;
    w->unk58 |= 0x2000;
    w->unk54 |= 2;
    w->unk54 |= 0x10;
    w->unk68 = FixVsPushIcon_State_FollowPlayer;
}
