#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
} UnkStruct_022c4560;

extern void *data_ov000_02369ba0;
extern UnkStruct_022c4560 data_022c4560;
extern u8 data_ov000_02366340[], data_ov000_02369dec[];

void func_ov000_022fbbb8(void);

void func_ov000_022fbcac(UnkStruct_StageObj *par, s32 dx, s32 dy) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    if (data_022c4560.unk18 & 0x20)
        return;
    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_02366340, 0xffff, data_ov000_02369dec, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0xf);
    StageObj_SetOAMOrder(w, 0x15);
    w->spr0.unk2C |= 0x200;
    w->spr1.unk2C |= 0x200;
    w->unk88 = par;
    w->unk10 = par->unk10 + (dx << 8);
    w->unk14 = par->unk14 + (dy << 8);
    w->unk44 = 0;
    w->unk46 = 0x200;
    w->unk5C |= 4;
    w->unk58 |= 0x2100;
    w->unk54 |= 0x202;
    w->unk68 = func_ov000_022fbbb8;
}
