#include <ov000_b/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366cdc[], data_ov000_0236a0ec[], data_ov000_0236a0f4[], data_ov000_0236a104[];

void MapObject_Destructor(void);
void func_ov000_02310e7c(UnkStruct_StageObj *w);

UnkStruct_StageObj *NeedleG_Create(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_StageObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0x500, 2, sizeof(UnkStruct_StageObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_StageObj));
    MapObject_Init(w, a, b, c);
    StageObj_Load2dBAC(w, data_ov000_02366cdc, 0, data_ov000_0236a0ec, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x3b);
    if (w->unk1E0->unk2 == 0xb1) {
        StageObj_SetSharedSpriteVram(w, 6, data_ov000_0236a0f4);
    } else {
        StageObj_SetSharedSpriteVram(w, 5, data_ov000_0236a104);
        StageObj_SetAnimation(w, 1);
    }
    StageObj_SetOAMOrder(w, 0x16);
    ObjRect_SetAtkDefFlags(&w->coll1, 0x13, -1);
    w->coll1.unk1C = NULL;
    if (w->unk1E0->unk4 & 1)
        w->unk5C ^= 1;
    w->unk1F4 |= 0x20;
    func_ov000_02310e7c(w);
    return w;
}
