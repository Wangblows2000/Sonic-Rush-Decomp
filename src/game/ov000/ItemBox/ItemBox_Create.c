#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
} UnkStruct_0230de00;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
} UnkStruct_022c4560;

extern void *data_ov000_02369ba0;
extern UnkStruct_022c4560 gGameState;
extern u8 data_ov000_02366b74[], data_ov000_0236a01c[];

void MapObject_Destructor(void);
void ItemBox_OnHit(void);
BOOL Stage_IsGravityFlipped(s32 x, s32 y);
void ItemBox_CreatePart_ov000_0230d930(UnkStruct_0230de00 *w, s32 a, s32 b, u16 c);
void ItemBox_Action_Init(UnkStruct_0230de00 *w);

UnkStruct_0230de00 *ItemBox_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_0230de00 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0x500, 2, sizeof(UnkStruct_0230de00));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0230de00));
    MapObject_Init(w, (s32)p, b, c);
    StageObj_Load2dBAC(w, data_ov000_02366b74, 0xffff, data_ov000_0236a01c, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 2);
    StageObj_SetOAMOrder(w, 0x16);
    ObjRect_SetAtkDefFlags(&w->base.coll0, 0, -16);
    w->base.coll0.unk1C = ItemBox_OnHit;
    w->base.coll0.unkA |= 0x800;
    w->base.unk58 |= 0x2100;
    if (gGameState.unk10 == 2) {
        if (w->base.unk1E0->unk6 == 2)
            w->base.unk1E0->unk6 = 3;
    }
    if (w->base.unk1E0->unk2 < 0x30) {
        switch (0x30 - w->base.unk1E0->unk2) {
        case 1:
            w->base.unk1E0->unk6 = 2;
            break;
        case 2:
            w->base.unk1E0->unk6 = 5;
            break;
        case 3:
            w->base.unk1E0->unk6 = 6;
            break;
        case 4:
            w->base.unk1E0->unk6 = 7;
            break;
        case 5:
            w->base.unk1E0->unk6 = 3;
            break;
        }
    }
    if (Stage_IsGravityFlipped(w->base.unk10, w->base.unk14)) {
        w->base.unk5C |= 2;
        w->base.unk5 = 0x80;
    }
    if (w->base.unk1E0->unk2 <= 0x30) {
        ItemBox_CreatePart_ov000_0230d930(w, 0, 0, w->base.unk1E0->unk6);
    } else {
        StageObj_SetAnimation(w, 1);
        w->base.unk5C |= 4;
    }
    ItemBox_Action_Init(w);
    return w;
}
