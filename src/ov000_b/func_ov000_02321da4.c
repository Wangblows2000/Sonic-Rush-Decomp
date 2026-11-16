#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x54];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x94 - 0x5E];
    UnkStruct_Spr spr0;
    UnkStruct_Spr spr1;
    u8 pad124[0x134 - 0x124];
    UnkStruct_Coll coll0;
    u8 pad160[0x19C - 0x160];
} UnkStruct_02321da4;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02367354[], data_ov000_0236a708[];

void func_ov000_023265d8(void);
void func_ov000_02321d90(void);
void func_ov000_02326638(void *w, UnkStruct_ObjParam *p, s32 a, s32 b);

UnkStruct_02321da4 *func_ov000_02321da4(UnkStruct_ObjParam *p, s32 a, s32 b) {
    UnkStruct_Task *task;
    UnkStruct_02321da4 *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_ov000_023265d8, 0, 0x500, 4, sizeof(UnkStruct_02321da4));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02321da4));
    func_ov000_02326638(w, p, a, b);
    func_0206e3c0(&w->coll0, -0x28, -0x18, 0x28, 0x30);
    func_0206e2b4(&w->coll0, 0, -0x10);
    w->coll0.unkA |= 0xc0;
    w->coll0.unk1C = func_ov000_02321d90;
    w->coll0.unk18 = w;
    func_02069f1c(w, data_ov000_02367354, 2, data_ov000_0236a708, data_ov000_02369ba0);
    func_02069918(w, 0x16);
    func_020698f0(w, 2);
    func_0206a054(w, 0, 0x1e);
    w->unk58 |= 0x2100;
    if (p->unk2 == 0x37)
        w->unk54 |= 2;
    if (p->unk2 == 0x39)
        w->unk5C ^= 1;
    return w;
}
