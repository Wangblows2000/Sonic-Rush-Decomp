#include <game/Stage/stageobj_alt.h>

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;

extern void *gActComArchive;
extern UnkStruct_022c4560 gGameState;
extern u8 data_ov000_02368460[];
extern u8 data_ov000_0236a9f8[];

s32 FX_Atan2(s32 y, s32 x);
void Math_RotateVec2S16(s16 *x, s16 *y, u8 a);
void Effect_SetFollowParentState(UnkStruct_EffObj *w);

UnkStruct_EffObj *EffBoostJet_Create(UnkStruct_StageObj *arg, s16 x, s16 y, s32 unused) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    if (gGameState.unk1C >= 9 || gGameState.unk1D >= 2)
        return NULL;
    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_02368460, 8, data_ov000_0236a9f8, gActComArchive);
    StageObj_AllocSpritePalette(w, 1, 0xc);
    StageObj_SetAnimation(w, 9);
    StageObj_SetOAMOrder(w, 9);
    if (gGameState.unk18 & 0x20)
        w->spr1.unk2C |= 0x80008;
    w->unk88 = arg;
    if (arg->unk5C & 1)
        x = -x;
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk1 = FX_Atan2(-0x800, ((UnkStruct_EffObj *)w->unk88)->unk2E) >> 8;
    w->unk1 += arg->unk5 + 0x40;
    Math_RotateVec2S16(&x, &y, arg->unk1 + arg->unk5);
    w->unk2E = x;
    w->unk30 = y;
    w->unk10 += x << 8;
    w->unk14 += y << 8;
    w->unk58 |= 0x2100;
    w->unk5C |= 0x100;
    w->unk54 |= 2;
    Effect_SetFollowParentState(w);
    return w;
}
