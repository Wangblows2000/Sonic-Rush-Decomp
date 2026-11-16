#include <ov000_b/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366d30[], data_ov000_0236a15c[];

void func_ov000_022fb9c8(void);
void func_ov000_02312014(void);
void func_ov000_02312070(UnkStruct_StageObj *w);

UnkStruct_StageObj *func_ov000_02312080(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_StageObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, func_ov000_022fb9c8, 0, 0x500, 2, sizeof(UnkStruct_StageObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_StageObj));
    func_ov000_022fba50(w, a, b, c);
    StageObj_Load2dBAC(w, data_ov000_02366d30, 0xffff, data_ov000_0236a15c, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 3, 0x44);
    StageObj_SetAnimation(w, 3);
    StageObj_SetOAMOrder(w, 6);
    StageObj_SetOAMPriority(w, 1);
    ObjRect_SetAtkDefFlags(&w->coll0, 0, -0x10);
    w->coll0.unk1C = func_ov000_02312014;
    w->coll0.unkA |= 0x800;
    w->coll1.unk18 = NULL;
    if (w->unk1E0->unk4 & 1)
        w->unk5C ^= 1;
    w->unk58 |= 0x2100;
    func_ov000_02312070(w);
    return w;
}
