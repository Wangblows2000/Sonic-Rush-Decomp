#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_02316170_Tgt;

typedef struct {
    u8 pad0[5];
    u8 unk5;
    u8 pad6[0x10 - 6];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x3E8 - 0x18];
    UnkStruct_02316170_Tgt *unk3E8;
} UnkStruct_02316170_Arg;

extern void *data_022c448c;
extern u8 data_ov000_02366f30[], data_ov000_0236a24c[];

void func_ov000_02316148(void);
void func_ov000_02315e84(void);
BOOL func_ov000_02345758(s32 x, s32 y);

void func_ov000_02316170(UnkStruct_02316170_Arg *arg) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_ov000_02316148, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    func_02069f1c(w, data_ov000_02366f30, 0xffff, data_ov000_0236a24c, data_022c448c);
    func_0206a054(w, 0, 0x4a);
    func_02069918(w, 0xa);
    if (func_ov000_02345758(arg->unk10, arg->unk14))
        w->unk5C |= 2;
    w->unk88 = arg;
    if (arg->unk3E8 != NULL) {
        w->unk10 = arg->unk3E8->unk10;
        w->unk14 = arg->unk3E8->unk14;
    }
    w->unkC = 0;
    w->unk5C |= 1;
    if (arg->unk5 != 0)
        w->unk14 += 0x4800;
    else
        w->unk14 -= 0x4800;
    w->unk5C |= 4;
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    w->spr0.unk2C |= 0x200;
    w->spr1.unk2C |= 0x200;
    w->unk5C |= 0x20;
    w->unk44 = 8;
    w->unk46 = 0x180;
    w->unk68 = func_ov000_02315e84;
}
