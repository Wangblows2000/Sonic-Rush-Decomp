#include <game/Stage/stageobj_alt.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x2CC - 0x20C];
} UnkStruct_0231b5c8;

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
} UnkStruct_022c4560;

extern void *data_ov000_02369ba0;
extern UnkStruct_022c4560 gGameState;
extern u8 data_ov000_023671e8[], data_ov000_0236a348[], data_ov000_0236a360[], data_ov000_0236a350[], data_ov000_0236a370[];
extern u8 data_ov000_023644c4[], data_ov000_023661c0[];

void MapObject_Destructor(void);
void WallBreak_OnHit(void);
void PaletteAnim_Create(s32 a, void *b, u32 c, s32 d);
BOOL Stage_IsGravityFlipped(s32 x, s32 y);
void WallBreak_Action_Idle(UnkStruct_0231b5c8 *w);

UnkStruct_0231b5c8 *WallBreak_Create(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_0231b5c8 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0xf6, 2, sizeof(UnkStruct_0231b5c8));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0231b5c8));
    MapObject_Init(w, a, b, c);
    StageObj_Load2dBAC(w, data_ov000_023671e8, 0, data_ov000_0236a348, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 6);
    switch (w->base.unk1E0->unk4 & 3) {
    case 0:
        StageObj_SetSharedSpriteVram(w, 0x10, data_ov000_0236a360);
        break;
    case 1:
        StageObj_SetSharedSpriteVram(w, 0x10, data_ov000_0236a350);
        break;
    case 2:
    default:
        StageObj_SetSharedSpriteVram(w, 0x10, data_ov000_0236a370);
        break;
    }
    if (gGameState.unk1C == 3) {
        PaletteAnim_Create(w->base.spr0.unk44, data_ov000_023644c4, *(u32 *)data_ov000_0236a348, 0);
        w->base.unk54 |= 0x800;
    }
    w->base.spr0.unk46 = 0x14;
    w->base.spr1.unk46 = 0x14;
    ObjRect_SetAtkDefFlags(&w->base.coll0, 0, -0x10);
    w->base.coll0.unk1C = WallBreak_OnHit;
    w->base.coll0.unkA |= 0x800;
    w->base.unk1B8 = w;
    w->base.unk1C8 = data_ov000_023661c0;
    w->base.unk1DA = 6;
    w->base.unk1D6 = -0x1c;
    switch (w->base.unk1E0->unk4 & 3) {
    case 0:
        w->base.unk1D8 = 1;
        w->base.unk1D4 = -3;
        w->base.unk1B8 = NULL;
        break;
    case 1:
        w->base.unk1D8 = 4;
        w->base.unk1D4 = -0x10;
        break;
    case 2:
    default:
        w->base.unk1D8 = 4;
        w->base.unk1D4 = -0x10;
        break;
    }
    if (Stage_IsGravityFlipped(w->base.unk10, w->base.unk14))
        w->base.unk5 = 0x80;
    w->base.unk58 |= 0x2100;
    WallBreak_Action_Idle(w);
    return w;
}
