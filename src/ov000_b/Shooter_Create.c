#include <ov000_b/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366fc0[], data_ov000_0236a25c[], data_ov000_023661c0[];

void MapObject_Destructor(void);
void Shooter_CreatePart2(UnkStruct_StageObj *w, s32 a);
void Shooter_CreatePart1(UnkStruct_StageObj *w, s32 a);
void func_ov000_023174e4(UnkStruct_StageObj *w);

UnkStruct_StageObj *Shooter_Create(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_StageObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0xf6, 2, sizeof(UnkStruct_StageObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_StageObj));
    MapObject_Init(w, a, b, c);
    StageObj_Load2dBAC(w, data_ov000_02366fc0, 0x10, data_ov000_0236a25c, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x59);
    if (w->unk1E0->unk2 == 0xbb)
        StageObj_SetAnimation(w, 1);
    else
        StageObj_SetAnimation(w, 6);
    StageObj_SetOAMOrder(w, 0x14);
    w->unk1B8 = w;
    w->unk1C8 = data_ov000_023661c0;
    if (w->unk1E0->unk2 == 0xbb) {
        w->unk1D8 = 4;
        w->unk1DA = 8;
        w->unk1D4 = -0xf;
        w->unk1D6 = -0x38;
        Shooter_CreatePart2(w, 0);
        Shooter_CreatePart1(w, 0);
    } else {
        w->unk1D8 = 7;
        w->unk1DA = 3;
        w->unk1D4 = -0x22;
        w->unk1D6 = -0x1c;
        Shooter_CreatePart2(w, 1);
        Shooter_CreatePart1(w, 1);
    }
    w->unk48 = 0x100;
    if (!(w->unk1E0->unk4 & 1))
        w->unk5C ^= 1;
    w->unk58 |= 0x100;
    w->unk54 |= 2;
    w->unk1DC = Sound_AllocHandle();
    func_ov000_023174e4(w);
    return w;
}
