#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x324 - 0x18];
    u8 unk324;
} UnkStruct_0232fde0_Arg;

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
} UnkStruct_022c4560;

extern void *gActComArchive;
extern UnkStruct_022c4560 data_022c4560;
extern u8 data_ov000_02368254[], data_ov000_0236a9b0[];

void func_ov000_0232fda4(void);

void PlayerIcon_Create(UnkStruct_0232fde0_Arg *arg) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_02368254, 0xffff, data_ov000_0236a9b0, gActComArchive);
    StageObj_SetAnimation(w, arg->unk324);
    w->spr0.unk44 = 0;
    w->spr1.unk44 = 0;
    if (data_022c4560.unk18 & 0x20) {
        w->spr0.unk2C |= 0x10;
        w->spr1.unk2C |= 0x10;
    }
    w->spr0.unk2C |= 0x80000;
    w->unk5C |= 4;
    StageObj_SetOAMOrder(w, 0xa);
    StageObj_SetOAMPriority(w, 2);
    w->unk5C |= 1;
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk88 = arg;
    w->unk58 |= 0x100;
    w->unk58 |= 0x2000;
    w->unk54 |= 2;
    w->unk54 |= 0x10;
    w->unk5C |= 0x20;
    w->unk68 = func_ov000_0232fda4;
}
