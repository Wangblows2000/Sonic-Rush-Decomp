#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0xA];
    u8 unkA;
    u8 unkB;
    u8 padC[0x104 - 0xC];
} UnkStruct_02333520_Sub;

typedef struct {
    UnkStruct_EffObj base;
    UnkStruct_02333520_Sub unk134;
} UnkStruct_02333520;

extern void *gZoneActArchive;
extern u8 data_ov000_02368538[];
extern u8 data_ov000_0236aa30[];

void StageObj_Load3dBAC(void *w, UnkStruct_02333520_Sub *s, void *a, s32 b, s32 c, void *d, void *e);
void func_ov000_02334264(UnkStruct_02333520 *w);

UnkStruct_02333520 *func_ov000_02333520(UnkStruct_StageObj *arg, s32 x, s32 y) {
    UnkStruct_Task *task;
    UnkStruct_02333520 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_02333520));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02333520));
    StageObj_Load3dBAC(w, &w->unk134, data_ov000_02368538, 0x80, 0x10, data_ov000_0236aa30, gZoneActArchive);
    w->unk134.unkA = 0x14;
    w->unk134.unkB = 7;
    if (arg->unk5C & 1)
        w->base.unk5C |= 1;
    w->base.unk10 = arg->unk10;
    w->base.unk14 = arg->unk14;
    w->base.unk2E = -0x80;
    w->base.unk30 = 0;
    w->base.unkC = 0x10fffa;
    if (w->base.unk5C & 1) {
        w->base.unk10 -= x << 8;
        w->base.unk2E = -w->base.unk2E;
    } else {
        w->base.unk10 += x << 8;
    }
    w->base.unk14 += y << 8;
    w->base.unk58 |= 0x100;
    w->base.unk54 |= 2;
    func_ov000_02334264(w);
    return w;
}
