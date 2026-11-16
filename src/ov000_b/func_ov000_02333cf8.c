#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0xA];
    u8 unkA;
    u8 unkB;
    u8 padC[0xDC - 0xC];
    u32 unkDC;
    u8 padE0[0x104 - 0xE0];
} UnkStruct_02333cf8_Sub;

typedef struct {
    UnkStruct_EffObj base;
    UnkStruct_02333cf8_Sub unk134;
} UnkStruct_02333cf8;

extern void *gZoneActArchive;
extern u8 data_ov000_02368574[];
extern u8 data_ov000_0236a9d0[];

void Math_RotateVec2S16(s16 *x, s16 *y, u8 a);
void StageObj_Load3dBAC(void *w, UnkStruct_02333cf8_Sub *s, void *a, s32 b, s32 c, void *d, void *e);
void func_ov000_02333bd0(UnkStruct_02333cf8 *w);

UnkStruct_02333cf8 *func_ov000_02333cf8(UnkStruct_StageObj *arg, s16 x, s16 y, s32 type) {
    UnkStruct_Task *task;
    UnkStruct_02333cf8 *w;
    vu16 zero;

    if (arg->unk0 != 1)
        return NULL;
    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_02333cf8));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02333cf8));
    StageObj_Load3dBAC(w, &w->unk134, data_ov000_02368574, 0x800, 0x10, data_ov000_0236a9d0, gZoneActArchive);
    w->unk134.unkA = 4;
    w->unk134.unkB = 0;
    w->unk134.unkDC |= 0x80;
    w->unk134.unkDC |= 0x40;
    if (arg->unk5C & 1)
        w->base.unk5C |= 1;
    if (type != 1)
        StageObj_SetAnimation(w, 0);
    else
        StageObj_SetAnimation(w, 2);
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
    w->base.unk58 |= 0x2000;
    w->base.unk54 |= 2;
    w->base.unk5C |= 0x100;
    func_ov000_02333bd0(w);
    return w;
}
