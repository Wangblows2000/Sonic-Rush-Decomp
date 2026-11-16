#include <ov000_b/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366efc[], data_ov000_0236a234[], data_ov000_023661c0[];

void func_ov000_022fb9c8(void);
void func_ov000_023153a4(void);
void func_ov000_0231527c(UnkStruct_StageObj *w);
void func_ov000_02315410(UnkStruct_StageObj *w);

UnkStruct_StageObj *func_ov000_02315420(UnkStruct_ObjParam *a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_StageObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, func_ov000_022fb9c8, 0, 0x500, 2, sizeof(UnkStruct_StageObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_StageObj));
    func_ov000_022fba50(w, (s32)a, b, c);
    StageObj_Load2dBAC(w, data_ov000_02366efc, 0xffff, data_ov000_0236a234, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x21);
    w->spr0.unk46 = 9;
    w->spr1.unk46 = 9;
    w->spr0.unk2C &= ~0x60000;
    w->spr1.unk2C &= ~0x60000;
    w->spr0.unk2C |= 0x20000;
    w->spr1.unk2C |= 0x20000;
    if (a->unk2 == 0x90) {
        w->unk1B8 = w;
        w->unk1C8 = data_ov000_023661c0;
        w->unk1D8 = 6;
        w->unk1DA = 5;
        w->unk1D4 = -0x18;
        w->unk1D6 = -0x14;
        func_ov000_0231527c(w);
    } else {
        w->unk18 = 0x4000;
    }
    ObjRect_SetAtkDefFlags(&w->coll0, 0, -16);
    w->coll0.unk1C = func_ov000_023153a4;
    w->coll0.unkA |= 0x800;
    w->unk58 |= 0x2100;
    func_ov000_02315410(w);
    return w;
}
