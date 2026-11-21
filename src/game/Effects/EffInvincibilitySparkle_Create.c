#include <game/Stage/stageobj_alt.h>

extern void *gActComArchive;
extern u8 data_ov000_023682b4[], data_ov000_0236aa18[];

void EffInvincibilitySparkle_State_Orbit(void);

void EffInvincibilitySparkle_Create(s32 x, s32 y, s32 t) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == OS_GetArenaLo(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_023682b4, 5, data_ov000_0236aa18, gActComArchive);
    StageObj_AllocSpritePalette(w, 0xc, 0xc);
    StageObj_SetAnimation(w, 0xc);
    StageObj_SetOAMOrder(w, 9);
    StageObj_SetOAMPriority(w, 1);
    w->unk10 = x << 8;
    w->unk14 = y << 8;
    w->unkC = t;
    w->unk58 |= 0x100;
    w->unk54 |= 2;
    w->unk68 = EffInvincibilitySparkle_State_Orbit;
}
