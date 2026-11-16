#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0xA];
    u8 unkA;
    u8 unkB;
    u8 padC[0xDC - 0xC];
    u32 unkDC;
    u8 padE0[0x104 - 0xE0];
} UnkStruct_02334380_Sub;

typedef struct {
    UnkStruct_EffObj base;
    UnkStruct_02334380_Sub unk134;
} UnkStruct_02334380;

extern void *gZoneActArchive;
extern u8 data_ov000_023685b0[];
extern u8 data_ov000_0236a9f0[];

void Math_RotateVec2S16(s16 *x, s16 *y, u8 a);
void StageObj_Load3dBAC(void *w, UnkStruct_02334380_Sub *s, void *a, s32 b, s32 c, void *d, void *e);
void func_ov000_02334550(void);

UnkStruct_02334380 *func_ov000_02334380(UnkStruct_StageObj *arg, s16 x, s16 y, s32 type) {
    UnkStruct_Task *task;
    UnkStruct_02334380 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_02334380));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02334380));
    StageObj_Load3dBAC(w, &w->unk134, data_ov000_023685b0, 0x200, 0x10, data_ov000_0236a9f0, gZoneActArchive);
    if (arg->unk5C & 1)
        w->base.unk5C |= 1;
    w->unk134.unkA = 4;
    w->unk134.unkB = 0;
    w->unk134.unkDC |= 0x80;
    w->unk134.unkDC |= 0x40;
    if (type != 1)
        StageObj_SetAnimation(w, 0);
    else
        StageObj_SetAnimation(w, 1);
    w->base.unk88 = arg;
    w->base.unk10 = arg->unk10;
    w->base.unk14 = arg->unk14;
    w->base.unk1 = arg->unk5;
    Math_RotateVec2S16(&x, &y, arg->unk5);
    w->base.unk2E = x;
    w->base.unk30 = y;
    if (w->base.unk5C & 1)
        w->base.unk10 -= x << 8;
    else
        w->base.unk10 += x << 8;
    w->base.unk14 += y << 8;
    w->base.unk58 |= 0x100;
    w->base.unk5C |= 0x100;
    w->base.unk54 |= 2;
    w->base.unk68 = func_ov000_02334550;
    return w;
}
