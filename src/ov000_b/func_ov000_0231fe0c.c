#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[4];
    s32 unk4;
    s32 unk8;
} UnkStruct_0236aad8;

extern void *data_022c448c;
extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_0236aad8 data_ov000_0236aad8;
extern u8 data_ov000_023672bc[];
extern u8 data_ov000_0236a6f8[];

void func_ov000_0231fdec(void);

UnkStruct_EffObj *func_ov000_0231fe0c(void) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    if (data_022c4560.unk18 & 0x20)
        return NULL;
    task = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    func_02069f1c(w, data_ov000_023672bc, 0xffff, data_ov000_0236a6f8, data_022c448c);
    func_0206a054(w, 0, 0xc);
    func_02069818(w, (u16)(Rand() & 3));
    w->unk10 = data_ov000_0236aad8.unk4 - 0x1800;
    w->unk14 = data_ov000_0236aad8.unk8 + ((Rand() & 0xff) << 8);
    w->unk2E = (Rand() & 0x3ff) + 0x700;
    w->unk30 = (Rand() & 0x3ff) - 0x200;
    func_020698f0(w, 1);
    func_02069918(w, 6);
    w->unk5C |= 0x20;
    w->unk5C |= 1;
    w->unk58 |= 0x100;
    w->unk54 |= 2;
    w->unk68 = func_ov000_0231fdec;
    return w;
}
