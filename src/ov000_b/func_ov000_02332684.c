#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
} UnkStruct_022c4560;

extern void *gActComArchive;
extern UnkStruct_022c4560 data_022c4560;
extern u8 data_ov000_023684d8[];
extern u8 data_ov000_0236a9f8[];

BOOL func_ov000_023270d8(UnkStruct_EffObj *a);
s32 FX_Atan2(s32 y, s32 x);
void Math_RotateVec2S16(s16 *x, s16 *y, u8 a);
void func_ov000_02334264(UnkStruct_EffObj *w);

#define ABS(x) ((x) < 0 ? -(x) : (x))

UnkStruct_EffObj *func_ov000_02332684(UnkStruct_EffObj *arg) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    s16 v8;
    s16 vA;
    s16 vC;
    s16 vE;
    vu16 zero;

    if (func_ov000_023270d8(arg))
        return NULL;
    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_023684d8, 6, data_ov000_0236a9f8, gActComArchive);
    StageObj_AllocSpritePalette(w, 2, 0x6e);
    StageObj_SetAnimation(w, 2);
    StageObj_SetOAMOrder(w, 9);
    if (data_022c4560.unk18 & 0x20)
        w->spr1.unk2C |= 0x80008;
    w->spr0.unk2C |= 0x400;
    w->spr1.unk2C |= 0x400;
    if (Rand() & 1)
        StageObj_SetOAMPriority(w, 1);
    if (arg->unk5C & 1)
        w->unk5C |= 1;
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk1 = FX_Atan2(arg->unk2A, arg->unk28) >> 8;
    if (ABS(arg->unk28) > ABS(arg->unk2A))
        w->unk2E = (0x7f - (Rand() & 0xfe)) + ABS(arg->unk28 >> 1);
    else
        w->unk2E = (0x7f - (Rand() & 0xfe)) + ABS(arg->unk2A >> 1);
    w->unk30 = 0;
    Math_RotateVec2S16(&w->unk2E, &w->unk30, w->unk1);
    v8 = -0x1100 - (Rand() & 0xe00);
    vA = 0xf00 - (Rand() & 0x1e00);
    vC = 0x3f - (Rand() & 0x7e);
    vE = -vA >> 9;
    Math_RotateVec2S16(&vC, &vE, w->unk1);
    w->unkC = (vC << 16) | (vE & 0xffff);
    Math_RotateVec2S16(&v8, &vA, w->unk1);
    w->unk10 += v8;
    w->unk14 += vA;
    w->unk58 |= 0x100;
    w->unk54 |= 2;
    func_ov000_02334264(w);
    return w;
}
