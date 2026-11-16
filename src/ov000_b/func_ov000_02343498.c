#include <ov000_b/stageobj.h>

typedef struct {
    u8 unk0;
    u8 pad1[0x10 - 1];
    UnkStruct_Vec unk10;
    u8 pad1C[0x5C - 0x1C];
    u16 unk5C;
} UnkStruct_02343498_Parent;

typedef struct {
    u8 unk0;
    u8 pad1[0xC - 1];
    s32 unkC;
    UnkStruct_Vec unk10;
    u8 pad1C[0x54 - 0x1C];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void *unk68;
    u8 pad6C[0x88 - 0x6C];
    UnkStruct_02343498_Parent *unk88;
    u8 pad8C[0x160 - 0x8C];
    UnkStruct_Coll coll;
} UnkStruct_02343498;

void func_ov000_023433d4(void);

UnkStruct_02343498 *func_ov000_02343498(UnkStruct_02343498_Parent *parent, s16 a, s16 b, s16 c, s16 d, s16 e) {
    UnkStruct_Task *task;
    UnkStruct_02343498 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0x101, 1, sizeof(UnkStruct_02343498));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02343498));
    w->unk0 = 5;
    w->unk54 |= 0x10;
    w->unk10.x = parent->unk10.x;
    w->unk10.y = parent->unk10.y;
    w->unk10.z = parent->unk10.z;
    w->unk88 = parent;
    w->unk58 |= 0x100;
    w->unk58 |= 0x2000;
    w->unk54 |= 0x200;
    if (parent->unk0 == 1)
        w->coll.unk28 = 0x21;
    else
        w->coll.unk28 = 0x12;
    w->coll.unk1C = NULL;
    w->coll.unkA |= 0x20;
    ObjRect_SetAtkDefFlags(&w->coll, 0x13, 0xffff);
    ObjRect_SetBox2D(&w->coll, a, b, c, d);
    if (w->unk88->unk5C & 1)
        w->unk5C |= 1;
    w->unk68 = func_ov000_023433d4;
    w->unkC = e;
    return w;
}
