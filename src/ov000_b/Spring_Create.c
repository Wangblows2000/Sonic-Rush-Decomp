#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;

extern void *data_ov000_02369ba0;
extern UnkStruct_022c4560 data_022c4560;
extern u8 data_ov000_02366f78[], data_ov000_0236a254[];

void MapObject_Destructor(void);
void Spring_OnHit(void);
void Spring_Action_Idle(UnkStruct_StageObj *w);

UnkStruct_StageObj *Spring_Create(s32 a, s32 b, s32 c) {
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
    StageObj_Load2dBAC(w, data_ov000_02366f78, 0xffff, data_ov000_0236a254, data_ov000_02369ba0);
    if (data_022c4560.unk1C == 3 || (data_022c4560.unk1C == 8 && data_022c4560.unk1D == 0))
        StageObj_AllocSpritePalette(w, 0, 0x5b);
    else
        StageObj_AllocSpritePalette(w, 0, 2);
    w->spr0.unk46 = 0x14;
    w->spr1.unk46 = 0x14;
    ObjRect_SetAtkDefFlags(&w->coll0, 0, -0x10);
    w->coll0.unk1C = Spring_OnHit;
    w->coll0.unkA |= 0x800;
    w->unk58 |= 0x2100;
    if (w->unk1E0->unk2 == 0x44 || (u16)(w->unk1E0->unk2 + 0xffb7) <= 1)
        w->unk5C |= 2;
    if (w->unk1E0->unk2 != 0x46 && w->unk1E0->unk2 != 0x48 && w->unk1E0->unk2 != 0x4a && w->unk1E0->unk2 != 0x4c)
        w->unk5C &= ~1;
    if (w->unk1E0->unk2 <= 0x4c) {
        if (w->unk1E0->unk2 >= 0x4b)
            w->unk8 = 6;
        else if (w->unk1E0->unk2 >= 0x47)
            w->unk8 = 4;
        else if (w->unk1E0->unk2 >= 0x45)
            w->unk8 = 2;
    }
    Spring_Action_Idle(w);
    return w;
}
