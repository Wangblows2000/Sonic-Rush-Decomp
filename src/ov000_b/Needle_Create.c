#include <ov000_b/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366cc4[], data_ov000_0236a0d4[], data_ov000_0236a0dc[], data_ov000_023661c0[];

void MapObject_Destructor(void);
BOOL Stage_IsGravityFlipped(s32 x, s32 y);
void Needle_Action_Unk_ov000_02310c08(UnkStruct_StageObj *w);

UnkStruct_StageObj *Needle_Create(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_StageObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0x500, 2, sizeof(UnkStruct_StageObj));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_StageObj));
    MapObject_Init(w, a, b, c);
    StageObj_Load2dBAC(w, data_ov000_02366cc4, 0, data_ov000_0236a0d4, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 3);
    StageObj_SetSharedSpriteVram(w, 0x40, data_ov000_0236a0dc);
    w->spr0.unk46 = 0x14;
    w->spr1.unk46 = 0x14;
    ObjRect_SetAtkDefFlags(&w->coll0, 0x53, -1);
    w->coll0.unk1C = NULL;
    w->unk1B8 = w;
    if ((u16)(w->unk1E0->unk2 + 0xffb1) <= 1) {
        w->unk1C8 = data_ov000_023661c0;
        w->unk1D8 = 3;
        w->unk1DA = 4;
        w->unk1D4 = -2;
        w->unk1D6 = -0x10;
        w->unk1F8 = 3;
    } else {
        w->unk1C8 = data_ov000_023661c0;
        w->unk1D8 = 4;
        w->unk1DA = 3;
        w->unk1D4 = -0x10;
        w->unk1D6 = -0x14;
        if (!Stage_IsGravityFlipped(w->unk10, w->unk14))
            w->unk1CC = -1;
    }
    w->unk58 |= 0x2100;
    w->unk5C |= 1;
    if (w->unk1E0->unk2 == 0x4e || w->unk1E0->unk2 == 0x5e)
        w->unk5C |= 2;
    if (w->unk1E0->unk2 == 0x50)
        w->unk5C &= ~1;
    Needle_Action_Unk_ov000_02310c08(w);
    w->unkC = 0x79;
    w->unkC -= w->unk1E0->unk6;
    return w;
}
