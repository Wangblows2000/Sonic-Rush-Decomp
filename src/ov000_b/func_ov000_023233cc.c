#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 data_022c4560;

void func_ov000_022fb9c8(void);
UnkStruct_EffObj *func_ov000_02320944(u32 a);

UnkStruct_StageObj *func_ov000_023233cc(UnkStruct_ObjParam *p, s32 x, s32 y) {
    UnkStruct_Task *task;
    UnkStruct_StageObj *w;
    vu16 zero;
    u16 i;

    task = Task_Create(StageObj_Main, func_ov000_022fb9c8, 0, 0x500, 2, sizeof(UnkStruct_StageObj));
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_StageObj));
    w->unk0 = 3;
    w->unk1E8 = x >> 8;
    w->unk1EC = y >> 8;
    w->unk1E0 = p;
    w->unk1E4 = w->unk1E0->unk0;
    w->unk1E0->unk0 = 0xff;
    w->unk58 |= 0x2100;
    w->unk54 |= 0x10;
    if (data_022c4560.unk1C == 5) {
        for (i = 0; i < 0x10; i++)
            func_ov000_02320944(i);
    }
    return NULL;
}
