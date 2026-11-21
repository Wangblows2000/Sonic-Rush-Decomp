#include <game/Stage/stageobj.h>

typedef struct {
    u8 pad0[8];
    u32 unk8;
} UnkStruct_022c4680;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
} UnkStruct_022c4560;

extern void *data_ov000_02369ba0;
extern UnkStruct_022c4680 gSaveData;
extern UnkStruct_022c4560 gGameState;
extern u8 data_ov000_023665a8[], data_ov000_023665c0[], data_ov000_023665dc[], data_ov000_02366600[], data_ov000_0236661c[], data_ov000_02366638[], data_ov000_02366658[], data_ov000_02369df4[], data_ov000_02369dfc[], data_ov000_02369e04[], data_ov000_02369e0c[], data_ov000_02369e14[], data_ov000_02369e1c[], data_ov000_02369e24[];

void MapObject_Destructor(void);
void EnemyFlapperGroup_OnHit_Box2(void);
BOOL Stage_IsGravityFlipped(s32 x, s32 y);
void EffEntrance_Create(UnkStruct_StageObj *w, s32 a, s32 b);
void EnemyFlapperGroup_Action_Hover(UnkStruct_StageObj *w);
void EnemyFlapperGroup_Action_Patrol(UnkStruct_StageObj *w);

typedef struct {
    UnkStruct_StageObj base;
} UnkStruct_023008d0;

UnkStruct_023008d0 *EnemyFlapperGroup_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_023008d0 *w;
    vu16 zero;

    if (p == NULL || p->unk0 != 0xff || p->unk1 != 0xff) {
        if (p->unk4 & 0x80) {
            if (gSaveData.unk8 & 1) {
                if (gGameState.unk10 != 2 && gGameState.unk10 != 3)
                    return NULL;
            }
        }
    }
    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0x500, 2, sizeof(UnkStruct_023008d0));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_023008d0));
    MapObject_Init(w, (s32)p, b, c);
    switch (w->base.unk1E0->unk2) {
    case 0:
    case 1:
    case 3:
        break;
    case 2:
        StageObj_Load2dBAC(w, data_ov000_023665a8, 0xffff, data_ov000_02369e1c, data_ov000_02369ba0);
        StageObj_AllocSpritePalette(w, 0, 0xe);
        break;
    case 4:
        StageObj_Load2dBAC(w, data_ov000_023665c0, 0xffff, data_ov000_02369e14, data_ov000_02369ba0);
        StageObj_AllocSpritePalette(w, 0, 0x15);
        break;
    case 11:
        StageObj_Load2dBAC(w, data_ov000_023665dc, 0xffff, data_ov000_02369e0c, data_ov000_02369ba0);
        StageObj_AllocSpritePalette(w, 0, 0x29);
        ObjRect_SetBox2D(&w->base.coll2, -45, 0, 45, 400);
        w->base.coll2.unkA |= 0xc0;
        ObjRect_SetAtkDefFlags(&w->base.coll2, 0, -16);
        w->base.coll2.unk28 = 0x12;
        w->base.coll2.unk1C = EnemyFlapperGroup_OnHit_Box2;
        w->base.coll2.unk18 = w;
        break;
    case 12:
        StageObj_Load2dBAC(w, data_ov000_02366600, 0xffff, data_ov000_02369e04, data_ov000_02369ba0);
        StageObj_AllocSpritePalette(w, 0, 0x30);
        ObjRect_SetBox2D(&w->base.coll2, -45, 0, 45, 400);
        w->base.coll2.unkA |= 0xc0;
        ObjRect_SetAtkDefFlags(&w->base.coll2, 0, -16);
        w->base.coll2.unk28 = 0x12;
        w->base.coll2.unk1C = EnemyFlapperGroup_OnHit_Box2;
        w->base.coll2.unk18 = w;
        w->base.unk1F4 |= 0x20;
        break;
    case 16:
        StageObj_Load2dBAC(w, data_ov000_0236661c, 0xffff, data_ov000_02369dfc, data_ov000_02369ba0);
        StageObj_AllocSpritePalette(w, 0, 0x38);
        if (w->base.unk1E0->unk9 == 0)
            ObjRect_SetBox2D(&w->base.coll2, -45, -45, 45, 45);
        else
            ObjRect_SetBox2D(&w->base.coll2, -45, p->unk7, 45, p->unk9 + p->unk7);
        w->base.coll2.unkA |= 0xc0;
        ObjRect_SetAtkDefFlags(&w->base.coll2, 0, -16);
        w->base.coll2.unk28 = 0x12;
        w->base.coll2.unk1C = EnemyFlapperGroup_OnHit_Box2;
        w->base.coll2.unk18 = w;
        break;
    case 17:
        StageObj_Load2dBAC(w, data_ov000_02366638, 0xffff, data_ov000_02369df4, data_ov000_02369ba0);
        StageObj_AllocSpritePalette(w, 0, 0x45);
        ObjRect_SetBox2D(&w->base.coll2, -100, 0, 0, 100);
        w->base.coll2.unkA |= 0xc0;
        ObjRect_SetAtkDefFlags(&w->base.coll2, 0, -16);
        w->base.coll2.unk28 = 0x12;
        w->base.coll2.unk1C = EnemyFlapperGroup_OnHit_Box2;
        w->base.coll2.unk18 = w;
        break;
    case 18:
        StageObj_Load2dBAC(w, data_ov000_02366658, 0xffff, data_ov000_02369e24, data_ov000_02369ba0);
        StageObj_AllocSpritePalette(w, 0, 0x46);
        ObjRect_SetBox2D(&w->base.coll2, -60, 10, 0, 80);
        w->base.coll2.unkA |= 0xc0;
        ObjRect_SetAtkDefFlags(&w->base.coll2, 0, -16);
        w->base.coll2.unk28 = 0x12;
        w->base.coll2.unk1C = EnemyFlapperGroup_OnHit_Box2;
        w->base.coll2.unk18 = w;
        if (w->base.unk1E0->unk6 == 0 && w->base.unk1E0->unk8 == 0) {
            w->base.unk1E0->unk6 = -0x28;
            w->base.unk1E0->unk8 = 0x50;
        }
        break;
    }
    StageObj_SetOAMOrder(w, 0x14);
    if (w->base.unk1E0->unk4 & 0x10)
        w->base.unk5C &= ~1;
    w->base.unk58 |= 0x140;
    if (Stage_IsGravityFlipped(w->base.unk10, w->base.unk14)) {
        w->base.unk5 = 0x80;
        w->base.unk5C |= 0x100;
    }
    if (p->unk4 & 0x40) {
        w->base.unk1F4 |= 0x40;
        EffEntrance_Create(&w->base, 0, -16);
    }
    if (w->base.unk1E0->unk6 == 0 && w->base.unk1E0->unk8 == 0)
        EnemyFlapperGroup_Action_Hover(&w->base);
    else
        EnemyFlapperGroup_Action_Patrol(&w->base);
    w->base.unk1DC = Sound_AllocHandle();
    return w;
}
