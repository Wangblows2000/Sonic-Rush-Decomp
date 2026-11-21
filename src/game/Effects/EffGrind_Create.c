#include <game/Stage/stageobj_alt.h>

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
} UnkStruct_022c4560;

extern void *gActComArchive;
extern UnkStruct_022c4560 gGameState;
extern u8 data_ov000_0236829c[];
extern u8 data_ov000_0236a9e0[];

BOOL Player_IsPlayer2(UnkStruct_StageObj *a);
void Math_RotateVec2S16(s16 *x, s16 *y, u8 a);
void EffGrind_State_FollowParent(void);

UnkStruct_EffObj *EffGrind_Create(UnkStruct_StageObj *arg, s16 x, s16 y, s32 unused) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    if (Player_IsPlayer2(arg))
        return NULL;
    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_0236829c, 0xffff, data_ov000_0236a9e0, gActComArchive);
    StageObj_AllocSpritePalette(w, 0, 0xc);
    StageObj_SetOAMOrder(w, 9);
    StageObj_SetOAMPriority(w, 1);
    if (gGameState.unk18 & 0x20)
        w->spr1.unk2C |= 0x80008;
    if (arg->unk5 != 0) {
        w->spr0.unk2C |= 0x200;
        w->spr1.unk2C |= 0x200;
    }
    w->unk88 = arg;
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk1 = arg->unk5;
    w->unk2E = x;
    w->unk30 = y;
    Math_RotateVec2S16(&x, &y, arg->unk5 + arg->unk1);
    w->unk10 += x << 8;
    w->unk14 += y << 8;
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    w->unk5C |= 0x100;
    w->unk5C |= 4;
    w->unk68 = EffGrind_State_FollowParent;
    return w;
}
