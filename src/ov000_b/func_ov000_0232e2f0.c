#include <ov000_b/stageobj.h>

extern void *gActComArchive;
extern u8 data_ov000_0236811c[], data_ov000_0236aa10[];

void Math_RotateVec2S16(s16 *x, s16 *y, u8 a);
void func_ov000_02334264(UnkStruct_EffObj *w);

UnkStruct_EffObj *func_ov000_0232e2f0(UnkStruct_StageObj *arg, s16 x, s16 y, s32 unused) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_0236811c, 0x20, data_ov000_0236aa10, gActComArchive);
    StageObj_SetAnimation(w, 2);
    StageObj_AllocSpritePalette(w, 0, 0xc);
    StageObj_SetOAMOrder(w, 9);
    if (arg->unk5 != 0) {
        w->spr0.unk2C |= 0x200;
        w->spr1.unk2C |= 0x200;
    }
    if (arg->unk5C & 1)
        w->unk5C |= 1;
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk1 = arg->unk5;
    Math_RotateVec2S16(&x, &y, arg->unk5);
    w->unk10 += x << 8;
    w->unk14 += y << 8;
    w->unk2E = 0;
    w->unk30 = 0;
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    w->unk5C |= 0x100;
    func_ov000_02334264(w);
    return w;
}
