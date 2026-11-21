#include <ov000_b/stageobj.h>

typedef struct {
    s32 unk0;
    u8 pad4[0x18 - 4];
    u32 unk18;
} UnkStruct_022c4560;

extern void *gActComArchive;
extern UnkStruct_022c4560 gGameState;
extern u8 data_ov000_02368490[];
extern u8 data_ov000_0236a9f8[];

BOOL Player_IsPlayer2(UnkStruct_StageObj *a);
void EffBoost_SetFollowState(UnkStruct_EffObj *w);

void EffBoostAura_Create(UnkStruct_StageObj *arg, s32 x, s32 y) {
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
    StageObj_Load2dBAC(w, data_ov000_02368490, 0x20, data_ov000_0236a9f8, gActComArchive);
    if (gGameState.unk18 & 0x20)
        w->spr1.unk2C |= 0x80008;
    if (gGameState.unk0 != 1) {
        StageObj_AllocSpritePalette(w, 0, 0x17);
        StageObj_SetAnimation(w, 4);
    } else {
        StageObj_AllocSpritePalette(w, 1, 0xc);
        StageObj_SetAnimation(w, 7);
    }
    StageObj_SetOAMOrder(w, 9);
    StageObj_SetOAMPriority(w, 1);
    w->spr0.unk2C |= 0x400;
    w->spr1.unk2C |= 0x400;
    w->spr0.unk2C |= 0x200;
    w->spr1.unk2C |= 0x200;
    if (arg->unk5C & 1)
        w->unk5C |= 1;
    w->unk88 = arg;
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk2E = 0;
    w->unk30 = 0;
    w->unk1 = arg->unk1 + arg->unk5;
    if (!(w->unk5C & 1)) {
        w->unk10 += x << 8;
    } else {
        w->unk10 -= x << 8;
        w->unk2E = -w->unk2E;
    }
    w->unk14 += y << 8;
    w->unk58 |= 0x100;
    w->unk5C |= 0x100;
    w->unk5C |= 4;
    w->unk54 |= 2;
    w->unk54 |= 0x10;
    EffBoost_SetFollowState(w);
}
