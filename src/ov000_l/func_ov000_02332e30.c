#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
} UnkStruct_022c4560;

extern void *gActComArchive;
extern UnkStruct_022c4560 data_022c4560;
extern u8 data_ov000_02368508[], data_ov000_0236a9f8[];
extern s16 data_0206e470[];

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]

BOOL func_ov000_023270d8(void);
void func_ov000_02334264(UnkStruct_EffObj *w);

UnkStruct_EffObj *func_ov000_02332e30(UnkStruct_StageObj *arg, u32 f) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    s32 d;
    vu16 zero;

    if (func_ov000_023270d8())
        return NULL;
    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_02368508, 6, data_ov000_0236a9f8, gActComArchive);
    StageObj_AllocSpritePalette(w, 2, 0x6e);
    StageObj_SetAnimation(w, 2);
    if (data_022c4560.unk18 & 0x20)
        w->spr1.unk2C |= 0x80008;
    StageObj_SetOAMOrder(w, 9);
    w->spr0.unk2C |= 0x400;
    w->spr1.unk2C |= 0x400;
    if (f & 8)
        StageObj_SetOAMPriority(w, 1);
    if (arg->unk5C & 1)
        w->unk5C |= 1;
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk30 = -0x61 - (Rand() & 0x3e);
    w->unkC = ((0xf - (Rand() & 0x1e)) << 16) | ((-1 - (Rand() & 0x3e)) & 0xffff);
    d = ((SIN((u16)(f << 12)) * 12) << 4) >> 16;
    if (w->unk5C & 1)
        d = (s16)-d;
    w->unk10 += d << 8;
    w->unk58 |= 0x100;
    w->unk54 |= 2;
    func_ov000_02334264(w);
    return w;
}
