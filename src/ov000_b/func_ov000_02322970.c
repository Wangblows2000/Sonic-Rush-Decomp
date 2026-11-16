#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[8];
    u32 unk8;
    u8 padC[0x58 - 0xC];
    u32 unk58;
    u8 pad5C[0x68 - 0x5C];
    void *unk68;
    u8 pad6C[0x134 - 0x6C];
    UnkStruct_Coll coll0;
    u8 pad160[0x19C - 0x160];
} UnkStruct_02322970;

void func_ov000_023265d8(void);
void func_ov000_02322638(void);
void func_ov000_02322934(void);
void func_ov000_02326638(void *w, void *p, s32 a, s32 b);

UnkStruct_02322970 *func_ov000_02322970(UnkStruct_ObjParam *p, s32 a, s32 b) {
    UnkStruct_Task *task;
    UnkStruct_02322970 *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_ov000_023265d8, 0, 0x500, 4, sizeof(UnkStruct_02322970));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02322970));
    func_ov000_02326638(w, p, a, b);
    if (p->unk2 == 0x36)
        func_0206e3c0(&w->coll0, -0x20, -0x30, 0x20, 0);
    else
        func_0206e3c0(&w->coll0, -0x20, -0x18, 0x20, 0);
    func_0206e2b4(&w->coll0, 0, -0x10);
    w->coll0.unkA |= 0xc0;
    w->coll0.unk1C = func_ov000_02322638;
    w->coll0.unk18 = w;
    w->unk8 = 0x20;
    if ((u16)(p->unk2 + 0xffaf) <= 1)
        w->unk8 = 4;
    w->unk58 |= 0x2100;
    w->unk68 = func_ov000_02322934;
    return w;
}
