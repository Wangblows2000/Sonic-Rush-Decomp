#include <ov000_b/stageobj.h>

typedef struct {
    s32 unk0;
    u8 pad4[0x18 - 4];
    u32 unk18;
} UnkStruct_022c4560;

extern void *gActComArchive;
extern UnkStruct_022c4560 data_022c4560;
extern u8 data_ov000_023684c0[];
extern u8 data_ov000_0236a9f8[];

BOOL func_ov000_023270d8(UnkStruct_StageObj *a);
void func_ov000_02334264(UnkStruct_EffObj *w);
void func_ov000_023323e8(void);

void func_ov000_02332448(UnkStruct_StageObj *arg, s32 x, s32 y) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    if (func_ov000_023270d8(arg))
        return;
    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == OS_GetArenaLo(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_023684c0, 0xffff, data_ov000_0236a9f8, gActComArchive);
    if (data_022c4560.unk18 & 0x20)
        w->spr1.unk2C |= 0x80008;
    if (data_022c4560.unk0 != 1) {
        StageObj_AllocSpritePalette(w, 0, 0x17);
        StageObj_SetAnimation(w, 5);
    } else {
        StageObj_AllocSpritePalette(w, 1, 0xc);
        StageObj_SetAnimation(w, 8);
    }
    StageObj_SetOAMOrder(w, 9);
    StageObj_SetOAMPriority(w, 1);
    w->spr0.unk2C |= 0x400;
    w->spr1.unk2C |= 0x400;
    w->spr0.unk2C |= 0x200;
    w->spr1.unk2C |= 0x200;
    w->unk70 = func_ov000_023323e8;
    if (arg->unk5C & 1)
        w->unk5C |= 1;
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk18 = arg->unk18;
    w->unk2E = 0;
    w->unk30 = 0;
    w->unk1 = arg->unk1 + arg->unk5;
    if (!(w->unk5C & 1)) {
        w->unk10 += x << 8;
    } else {
        w->unk10 -= x << 8;
        w->unk2E = -w->unk2E;
    }
    w->unk14 += y << 8;
    w->unk58 |= 0x100;
    w->unk5C |= 0x100;
    w->unk54 |= 2;
    func_ov000_02334264(w);
}
