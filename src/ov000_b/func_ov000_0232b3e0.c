#include <ov000_b/stageobj.h>

extern void *data_022c448c;
extern UnkStruct_EffObj *data_ov000_0236a978;
extern u8 data_ov000_02367fd0[];

void func_ov000_0232b3a4(void);
void func_ov000_0232b1cc(void);
void func_ov000_0232b348(void);
s32 func_ov000_0233766c(s32 a);
void func_0203344c(UnkStruct_Spr *s, s32 a, s32 b);
void func_ov000_02336cec(s32 *out, s32 n);

void func_ov000_0232b3e0(s32 unused, s32 n) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_ov000_0232b3a4, 0, 0xff, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    data_ov000_0236a978 = w;
    func_02069f1c(w, data_ov000_02367fd0, 0, NULL, data_022c448c);
    w->spr0.unk34 = func_ov000_0233766c(0);
    w->spr1.unk34 = func_ov000_0233766c(1);
    w->spr0.unk44 = 1;
    w->spr1.unk44 = 1;
    w->spr0.unk2C |= 0x10;
    w->spr1.unk2C |= 0x10;
    func_02069918(w, 3);
    func_020698f0(w, 0);
    w->unk54 |= 0x10;
    w->unk5C |= 0x1000;
    w->unk5C |= 1;
    w->unk5C |= 0x4000;
    func_0203344c(&w->spr0, 0, 0);
    func_0203344c(&w->spr1, 0, 0);
    w->unk70 = func_ov000_0232b1cc;
    w->unk10 = 0xf800;
    w->unk14 = 0x1400;
    w->unk68 = func_ov000_0232b348;
    w->unkC = 0;
    func_ov000_02336cec(&w->unk8, n);
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    w->unk5C |= 0x80;
}
