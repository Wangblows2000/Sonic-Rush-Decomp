#include <ov000_b/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x2AC - 0x20C];
} UnkStruct_0231a66c;

extern void *data_ov000_02369ba0;
extern void *data_ov000_0236ac60;
extern u8 data_ov000_02367180[], data_ov000_0236a328[];

void func_ov000_022fb9c8(void);
void Sound_PlaySeqArcEx(s32 a, s32 b, s32 c);
void Sound_SetPositional(s32 a, void *w, void *b);
void func_ov000_0231a608(UnkStruct_0231a66c *w);

UnkStruct_0231a66c *func_ov000_0231a66c(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_0231a66c *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, func_ov000_022fb9c8, 0, 0x500, 2, sizeof(UnkStruct_0231a66c));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0231a66c));
    func_ov000_022fba50(w, a, b, c);
    StageObj_Load2dBAC(w, data_ov000_02367180, 2, data_ov000_0236a328, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0xc);
    w->base.spr0.unk46 = 0x12;
    w->base.spr1.unk46 = 0x12;
    ObjRect_SetAtkDefFlags(&w->base.coll0, 0, -0x10);
    w->base.unk58 |= 0x100;
    func_02068e40(w, -2, -2, 2, 2);
    w->base.coll0.unk1C = NULL;
    w->base.spr0.unk2C &= ~0x60000;
    w->base.spr1.unk2C &= ~0x60000;
    w->base.unk1DC = Sound_AllocHandle();
    Sound_PlaySeqArcEx(w->base.unk1DC, 0, 0x68);
    Sound_SetPositional(w->base.unk1DC, w, data_ov000_0236ac60);
    func_ov000_0231a608(w);
    return w;
}
