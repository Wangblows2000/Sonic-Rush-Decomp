#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0xA];
    u8 unkA;
    u8 unkB;
    u8 padC[0xDC - 0xC];
    u32 unkDC;
    u8 padE0[0x104 - 0xE0];
} UnkStruct_023277d4_Sub;

typedef struct {
    UnkStruct_EffObj base;
    UnkStruct_023277d4_Sub unk134;
} UnkStruct_023277d4;

extern void *data_022c4488;
extern u8 data_ov000_02367bfc[];

void func_02069934(void *w, UnkStruct_023277d4_Sub *s, void *a, s32 b, s32 c, void *d, void *e);
void func_ov000_023277ac(void);

UnkStruct_023277d4 *func_ov000_023277d4(UnkStruct_StageObj *arg, s32 type, s32 x, s32 y, s16 z) {
    UnkStruct_Task *task;
    UnkStruct_023277d4 *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0xa00, 4, sizeof(UnkStruct_023277d4));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_023277d4));
    if (type == 0)
        func_02069934(w, &w->unk134, data_ov000_02367bfc, 0x480, 0x10, NULL, data_022c4488);
    w->unk134.unkA = 7;
    w->unk134.unkB = 0;
    w->unk134.unkDC |= 0x80;
    w->unk134.unkDC |= 0x40;
    w->unk134.unkDC = (w->unk134.unkDC & ~0x1f0000) | 0x180000;
    if (arg->unk5C & 1)
        w->base.unk5C |= 1;
    func_02069818(w, 0);
    w->base.unk88 = arg;
    w->base.unk10 = arg->unk10;
    w->base.unk14 = arg->unk14;
    w->base.unk2E = 0;
    w->base.unk30 = 0;
    if (w->base.unk5C & 1)
        w->base.unk10 -= x << 8;
    else
        w->base.unk10 += x << 8;
    w->base.unk14 += y << 8;
    w->base.unk18 += z << 8;
    w->base.unk58 |= 0x100;
    w->base.unk5C |= 0x104;
    w->base.unk54 |= 2;
    w->base.unk68 = func_ov000_023277ac;
    return w;
}
