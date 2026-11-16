#include <ov000_b/stageobj.h>
#include <ov000_b/res.h>

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_022c4560;

extern void *data_ov000_02369ba0;
extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_Res *data_ov000_0236a6f0[];
extern u8 data_ov000_023672d8[], data_ov000_0236a740[];

s32 ObjVram_Alloc(s32 a, s32 b);
void StageObj_Draw2D(void *w, UnkStruct_Spr *a, UnkStruct_Spr *b);
void func_ov000_023202f8(void);


UnkStruct_EffObj *DecSonicNeonPart_Create(UnkStruct_StageObj *arg, u16 idx) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0x4ff, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_023672d8, 0, data_ov000_0236a740, data_ov000_02369ba0);
    w->spr0.unk34 = ObjVram_Alloc(0, Res_GetElemVal(data_ov000_0236a6f0[20], idx));
    w->spr1.unk34 = ObjVram_Alloc(1, Res_GetElemVal(data_ov000_0236a6f0[20], idx));
    StageObj_SetOAMOrder(w, 0x16);
    StageObj_SetAnimation(w, idx);
    if (idx >= 0xd) {
        StageObj_AllocSpritePalette(w, 0xd, 0x62);
        idx -= 0xd;
        w->unk8 = idx + 0x10;
        if (!(data_022c4560.unk20 & 1))
            StageObj_Draw2D(w, &w->spr0, &w->spr1);
    } else {
        StageObj_AllocSpritePalette(w, 0, 0x61);
        w->unk8 = idx;
        if (data_022c4560.unk20 & 1)
            StageObj_Draw2D(w, &w->spr0, &w->spr1);
    }
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14 + (idx << 11) - 0x3000;
    w->unk88 = arg;
    w->unk5C |= 1;
    w->unk58 |= 0x2100;
    w->unk54 |= 0x12;
    w->unk68 = func_ov000_023202f8;
    return w;
}
