#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
} UnkStruct_022c4560;

extern void *gActComArchive;
extern UnkStruct_022c4560 gGameState;
extern u8 data_ov000_02368520[];
extern u8 data_ov000_0236a9f8[];

void Math_RotateVec2S16(s16 *x, s16 *y, u8 a);
void Effect_SetMoveUntilAnimEndState(UnkStruct_EffObj *w);

UnkStruct_EffObj *EffBlazeRunFlame_Create(UnkStruct_EffObj *arg, s16 x, s16 y, s32 unused) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;
    u8 n;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_02368520, 6, data_ov000_0236a9f8, gActComArchive);
    StageObj_AllocSpritePalette(w, 2, 0x6e);
    StageObj_SetAnimation(w, 2);
    StageObj_SetOAMOrder(w, 9);
    if (gGameState.unk18 & 0x20)
        w->spr1.unk2C |= 0x80008;
    w->spr0.unk2C |= 0x400;
    w->spr1.unk2C |= 0x400;
    if (Rand() & 1)
        StageObj_SetOAMPriority(w, 1);
    if (arg->unk5C & 1)
        w->unk5C |= 1;
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk2E = (0x7f - (Rand() & 0xfe)) + (arg->unk28 >> 1);
    w->unk30 = 0x9f - (Rand() & 0x3e);
    w->unkC = ((0x3f - (Rand() & 0x7e)) << 16) | ((-0x11 - (Rand() & 0x1e)) & 0xffff);
    n = arg->unk1 + arg->unk5;
    if (w->unk5C & 1)
        x = -x;
    Math_RotateVec2S16(&x, &y, n);
    x += 3 - (Rand() & 6);
    y += 3 - (Rand() & 6);
    w->unk10 += x << 8;
    w->unk14 += y << 8;
    w->unk58 |= 0x100;
    w->unk54 |= 2;
    Effect_SetMoveUntilAnimEndState(w);
    return w;
}
