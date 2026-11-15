#include <ov000_b/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366e14[];
extern u8 data_ov000_0236a1e4[];

void func_ov000_022fb9c8(void);
void func_ov000_02313b00(void);
void func_ov000_02313ac0(void);
BOOL func_ov000_02345758(s32 x, s32 y);
void func_ov000_02313ec8(UnkStruct_StageObj *w);

UnkStruct_StageObj *func_ov000_02313eec(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_StageObj *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_ov000_022fb9c8, 0, 0xf6, 2, sizeof(UnkStruct_StageObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_StageObj));
    func_ov000_022fba50(w, a, b, c);
    func_02069f1c(w, data_ov000_02366e14, 0xffff, data_ov000_0236a1e4, data_ov000_02369ba0);
    func_0206a054(w, 0, 0xb);
    w->spr0.unk46 = 0x14;
    w->spr1.unk46 = 0x14;
    w->spr0.unk2C |= 0x200;
    w->spr1.unk2C |= 0x200;
    func_02068e40(w, -0xc, -0xc, 0xc, 0xc);
    func_0206e2b4(&w->coll0, 0, -0x10);
    w->coll0.unk1C = func_ov000_02313b00;
    w->coll0.unkA |= 0x800;
    func_0206e2b4(&w->coll1, 0, -0x10);
    w->coll1.unk1C = func_ov000_02313ac0;
    w->coll1.unkA |= 0x800;
    w->coll1.unk28 = 0x21;
    if (w->unk1E0->unk4 & 1)
        w->unk5C ^= 1;
    w->unk58 |= 0x2110;
    w->unk58 |= 0x80000;
    w->unk58 &= ~0x80;
    w->unk5C |= 0x100;
    if (func_ov000_02345758(w->unk10, w->unk14))
        w->unk5 = 0x80;
    w->unk1DC = func_0203663c();
    func_ov000_02313ec8(w);
    return w;
}
