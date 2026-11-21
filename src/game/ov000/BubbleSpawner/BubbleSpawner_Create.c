#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x218 - 0x20C];
} UnkStruct_02302cb4;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366718[];
extern u8 data_ov000_02369e54[];

void MapObject_Destructor();
void BubbleSpawner_OnHit_Box1();
void BubbleSpawner_Action_Init();

UnkStruct_02302cb4 *BubbleSpawner_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_02302cb4 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0x500, 2, sizeof(UnkStruct_02302cb4));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02302cb4));
    MapObject_Init(w, (s32)p, b, c);
    StageObj_SetMapHitbox(w, -6, -6, 6, 6);
    StageObj_Load2dBAC(w, data_ov000_02366718, 8, data_ov000_02369e54, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x1c);
    w->base.spr0.unk46 = 0x14;
    w->base.spr1.unk46 = 0x14;
    w->base.coll0.unk20 = 0;
    w->base.coll0.unk24 = 7;
    ObjRect_SetBox2D(&w->base.coll1, -0x50, -0x180, 0x50, 0xc0);
    ObjRect_SetAtkDefFlags(&w->base.coll1, 0, -16);
    w->base.coll1.unkA |= 0xc0;
    w->base.coll1.unk1C = BubbleSpawner_OnHit_Box1;
    w->base.coll1.unk18 = w;
    w->base.unk58 |= 0x100;
    BubbleSpawner_Action_Init(w);
    return w;
}
