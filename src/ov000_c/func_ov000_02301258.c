#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x240 - 0x20C];
} UnkStruct_02301258;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366670[];
extern u8 data_ov000_02369e3c[];

void func_ov000_02300e94();
void func_ov000_02301234();

UnkStruct_02301258 *func_ov000_02301258(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_02301258 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, func_ov000_02301234, 0, 0x501, 2, sizeof(UnkStruct_02301258));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02301258));
    func_ov000_022fba50(w, (s32)p, b, c);
    func_02068e40(w, -6, -4, 6, 4);
    StageObj_Load2dBAC(w, data_ov000_02366670, 2, data_ov000_02369e3c, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x1a);
    StageObj_SetOAMOrder(w, 0x15);
    ObjRect_SetAtkDefFlags(&w->base.coll1, 0x13, -16);
    w->base.coll1.unk1C = func_ov000_02300e94;
    w->base.unk58 |= 0x100;
    w->base.unk1DC = Sound_AllocHandle();
    return w;
}
