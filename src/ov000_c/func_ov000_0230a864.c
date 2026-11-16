#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;

extern void *data_ov000_02369ba0;
extern UnkStruct_022c4560 data_022c4560;
extern u8 data_ov000_02366930[], data_ov000_02369f54[];

void func_ov000_0230a840(UnkStruct_EffObj *w);

void func_ov000_0230a864(UnkStruct_StageObj *par, s32 dx, s32 dy, s16 vx, s16 vy) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_02366930, 6, data_ov000_02369f54, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 4);
    w->spr0.unk46 = 7;
    w->spr1.unk46 = 7;
    if (data_022c4560.unk1C == 2 && data_022c4560.unk1D != 0)
        StageObj_SetOAMPriority(w, 1);
    w->unk10 = par->unk10;
    w->unk14 = par->unk14;
    w->unk2E = vx;
    w->unk30 = vy;
    if (par->unk5C & 1)
        w->unk5C |= 1;
    if (w->unk5C & 1) {
        w->unk10 -= dx << 8;
        w->unk2E = -w->unk2E;
    } else {
        w->unk10 += dx << 8;
    }
    w->unk14 += dy << 8;
    w->unk58 |= 0x100;
    w->unk54 |= 2;
    if (vx == 0 && vy == 0) {
        w->unk58 |= 0x80;
        w->unk40 = 4;
        w->unk42 = 0x100;
    }
    func_ov000_0230a840(w);
}
