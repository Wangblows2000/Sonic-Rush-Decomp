#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;

extern void *gActComArchive;
extern UnkStruct_022c4560 gGameState;
extern u8 data_ov000_02368434[];
extern u8 data_ov000_0236a9f8[];

BOOL func_ov000_023270d8(UnkStruct_StageObj *a);
void Math_RotateVec2S16(s16 *x, s16 *y, u8 a);
void func_ov000_023312f0(void);

UnkStruct_EffObj *func_ov000_02331398(UnkStruct_StageObj *arg, s16 x, s16 y, s32 unused) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    if (func_ov000_023270d8(arg))
        return NULL;
    if (gGameState.unk1C >= 9 || gGameState.unk1D >= 2)
        return NULL;
    if (arg->unk0 != 1)
        return NULL;
    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_02368434, 0x14, data_ov000_0236a9f8, gActComArchive);
    StageObj_AllocSpritePalette(w, 0, 0x17);
    StageObj_SetAnimation(w, 0xa);
    StageObj_SetOAMOrder(w, 9);
    StageObj_SetOAMPriority(w, 1);
    if (gGameState.unk18 & 0x20)
        w->spr1.unk2C |= 0x80008;
    w->spr0.unk2C |= 0x400;
    w->spr1.unk2C |= 0x400;
    w->unk88 = arg;
    if (arg->unk5C & 1) {
        w->unk5C |= 1;
        x = -x;
    }
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk5 = arg->unk5;
    Math_RotateVec2S16(&x, &y, arg->unk1 + arg->unk5);
    w->unk2E = x;
    w->unk30 = y;
    w->unk10 += x << 8;
    w->unk14 += y << 8;
    w->unk58 |= 0x2100;
    w->unk5C |= 0x100;
    w->unk5C |= 4;
    w->unk54 |= 2;
    w->unk68 = func_ov000_023312f0;
    return w;
}
