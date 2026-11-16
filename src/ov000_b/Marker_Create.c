#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;

extern void *data_ov000_02369ba0;
extern UnkStruct_022c4560 gGameState;
extern u8 data_ov000_02366cac[], data_ov000_0236a0cc[];

void MapObject_Destructor(void);
void Marker_OnHit(void);
BOOL Stage_IsGravityFlipped(s32 x, s32 y);
void func_ov000_023106b8(UnkStruct_StageObj *w);
void Marker_Action_Unk_ov000_023107c4(UnkStruct_StageObj *w);

UnkStruct_StageObj *Marker_Create(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_StageObj *w;
    vu16 zero;
    BOOL flag;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0x500, 2, sizeof(UnkStruct_StageObj));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_StageObj));
    MapObject_Init(w, a, b, c);
    StageObj_Load2dBAC(w, data_ov000_02366cac, 0x10, data_ov000_0236a0cc, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 2);
    w->spr0.unk46 = 0x14;
    w->spr1.unk46 = 0x14;
    ObjRect_SetAtkDefFlags(&w->coll0, 0, -0x10);
    w->coll0.unk1C = Marker_OnHit;
    w->unk58 |= 0x2100;
    if (Stage_IsGravityFlipped(w->unk10, w->unk14))
        w->unk5C |= 2;
    flag = FALSE;
    if (gGameState.unk1C >= 7 && gGameState.unk1C < 9 && gGameState.unk1D < 2)
        flag = TRUE;
    if (flag)
        func_ov000_023106b8(w);
    else
        Marker_Action_Unk_ov000_023107c4(w);
    return w;
}
