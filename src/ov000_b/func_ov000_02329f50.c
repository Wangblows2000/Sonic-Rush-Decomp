#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0x30 - 0x18];
    s16 unk30;
    u8 pad32[0x54 - 0x32];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void *unk68;
    u8 pad6C[0x134 - 0x6C];
} UnkStruct_02329f50;

extern u32 data_0208fe70;
extern void *data_022c4488;
extern u8 data_ov000_0236aa08[];
extern u8 data_ov000_02367f60[];

void func_ov000_02329c90(void);


void func_ov000_02329f50(UnkStruct_StageObj *arg, s16 x, s16 y, s32 type, s32 t) {
    UnkStruct_Task *task;
    UnkStruct_02329f50 *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0xa00, 4, sizeof(UnkStruct_02329f50));
    if (task == func_02006abc(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02329f50));
    func_02069f1c(w, data_ov000_02367f60, 1, data_ov000_0236aa08, data_022c4488);
    func_0206a054(w, 0, 0x17);
    func_02069818(w, (u16)(type + 2));
    func_02069918(w, 9);
    if (arg->unk5C & 1)
        w->unk5C |= 1;
    w->unk10 = arg->unk10 + (x << 8) + ((3 - (Rand() & 6)) << 8);
    w->unk14 = arg->unk14 + (y << 8);
    w->unk30 = 0xff - (Rand() & 0xfe);
    w->unkC = t + 6;
    w->unk8 = 0x2bf - (Rand() & 0xfe);
    w->unk58 |= 0x100;
    w->unk54 |= 2;
    w->unk68 = func_ov000_02329c90;
}
