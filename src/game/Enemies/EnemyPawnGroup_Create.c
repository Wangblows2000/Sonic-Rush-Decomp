#include <game/Stage/stageobj.h>

typedef struct {
    u8 pad0[0xA];
    u8 unkA;
    u8 unkB;
    u8 padC[0x104 - 0xC];
} UnkStruct_022fecf4_Sub;

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_Coll unk20C;
    u8 pad238[0x240 - 0x238];
    UnkStruct_022fecf4_Sub unk240;
} UnkStruct_022fecf4;

typedef struct {
    u8 pad0[8];
    u32 unk8;
} UnkStruct_022c4680;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[0x1C];
    s32 unk1C;
} UnkStruct_022c4534;

extern void *data_ov000_02369ba0;
extern UnkStruct_022c4680 gSaveData;
extern UnkStruct_022c4560 gGameState;
extern UnkStruct_022c4534 gStageMapCollision;
extern u8 data_ov000_023663c8[], data_ov000_02369de4[], data_ov000_02369dac[], data_ov000_023663e0[];
extern u8 data_ov000_023663fc[], data_ov000_02366414[], data_ov000_02369dd4[], data_ov000_02366430[];
extern u8 data_ov000_02369ddc[], data_ov000_0236644c[], data_ov000_02369dcc[], data_ov000_02369da4[];
extern u8 data_ov000_02366468[], data_ov000_02366488[], data_ov000_023664a4[], data_ov000_02369dc4[];
extern u8 data_ov000_023664c0[], data_ov000_02369dbc[], data_ov000_023664d8[], data_ov000_02369db4[];
extern u8 data_ov000_023664f4[], data_ov000_02369d9c[], data_ov000_02366510[], data_ov000_02369d94[];
extern u8 data_ov000_02366528[], data_ov000_02369d8c[];

void EnemyPawnGroup_Destructor(void);
void MapObject_Destructor(void);
void EnemyPawnGroup_OnHit_Box2_PushPlayer(void);
void EnemyPawnGroup_OnHit_Box2_Bounce(void);
void EnemyPawnGroup_OnDetect(void);
void StageObj_Load3dBAC(void *w, UnkStruct_022fecf4_Sub *s, void *a, s32 b, s32 c, void *d, void *e);
BOOL Stage_IsGravityFlipped(s32 x, s32 y);
void EnemyPawnGroup_Action_StartDropIn(UnkStruct_022fecf4 *w);
void EnemyPawnGroup_Action_StartEntrance(UnkStruct_022fecf4 *w);
void EnemyPawnGroup_Action_Walk(UnkStruct_022fecf4 *w);

UnkStruct_022fecf4 *EnemyPawnGroup_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_022fecf4 *w;
    UnkStruct_022fecf4 *big;
    vu16 zero;
    vu16 zero2;

    if (p == NULL || p->unk0 != 0xff || p->unk1 != 0xff) {
        if (p->unk4 & 0x80) {
            if (gSaveData.unk8 & 1) {
                if (gGameState.unk10 != 2 && gGameState.unk10 != 3)
                    return NULL;
            }
        }
    }
    if (p->unk9 != 0 && (p->unk2 == 0 || (u16)(p->unk2 + 0xffec) <= 1)) {
        if ((b >> 8) < gStageMapCollision.unk1C + 0xc4)
            return NULL;
        task = Task_Create(StageObj_Main, EnemyPawnGroup_Destructor, 0, 0x500, 2, 0x344);
        if (task == OS_GetArenaLo(0))
            return NULL;
        w = task->work;
        zero = 0;
        big = w;
        MIi_CpuClear16(zero, w, 0x344);
    } else {
        task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0x500, 2, 0x240);
        if (task == OS_GetArenaLo(0))
            return NULL;
        w = task->work;
        zero2 = 0;
        MIi_CpuClear16(zero2, w, 0x240);
    }
    MapObject_Init(w, (s32)p, b, c);
    StageObj_SetMapHitbox(w, -8, -10, 8, -3);
    switch (w->base.unk1E0->unk2) {
    case 20:
        w->base.unk4E = -0x100;
    case 0:
        if (w->base.unk1E0->unk9 != 0) {
            StageObj_Load2dBAC(w, data_ov000_023663c8, 0xffff, data_ov000_02369de4, data_ov000_02369ba0);
            StageObj_AllocSpritePalette(w, 0, 0xe);
            w->base.spr0.unk2C |= 0x80000;
            StageObj_Load3dBAC(w, &big->unk240, data_ov000_023663e0, 0x800, 0, data_ov000_02369dac, data_ov000_02369ba0);
            big->unk240.unkA = 4;
            big->unk240.unkB = 0;
        } else {
            StageObj_Load2dBAC(w, data_ov000_023663fc, 0xffff, data_ov000_02369de4, data_ov000_02369ba0);
            StageObj_AllocSpritePalette(w, 0, 0xe);
        }
        break;
    case 3:
        StageObj_Load2dBAC(w, data_ov000_02366414, 0xffff, data_ov000_02369dd4, data_ov000_02369ba0);
        StageObj_AllocSpritePalette(w, 0, 0x14);
        break;
    case 1:
        StageObj_Load2dBAC(w, data_ov000_02366430, 0xffff, data_ov000_02369ddc, data_ov000_02369ba0);
        StageObj_AllocSpritePalette(w, 0, 0x10);
        break;
    case 21:
        w->base.unk4E = -0x100;
    case 7:
        if (w->base.unk1E0->unk9 != 0) {
            StageObj_Load2dBAC(w, data_ov000_0236644c, 0xffff, data_ov000_02369dcc, data_ov000_02369ba0);
            StageObj_AllocSpritePalette(w, 0, 0xe);
            w->base.spr0.unk2C |= 0x80000;
            StageObj_Load3dBAC(w, &big->unk240, data_ov000_02366468, 0x880, 0, data_ov000_02369da4, data_ov000_02369ba0);
            big->unk240.unkA = 4;
            big->unk240.unkB = 0;
        } else {
            StageObj_Load2dBAC(w, data_ov000_02366488, 0xffff, data_ov000_02369dcc, data_ov000_02369ba0);
            StageObj_AllocSpritePalette(w, 0, 0xe);
        }
        break;
    case 8:
        StageObj_Load2dBAC(w, data_ov000_023664a4, 0xffff, data_ov000_02369dc4, data_ov000_02369ba0);
        StageObj_AllocSpritePalette(w, 0, 0x1f);
        w->base.unk1E5 = 2;
        w->base.unk1E5 -= w->base.unk1E0->unkB;
        break;
    case 9:
        StageObj_Load2dBAC(w, data_ov000_023664c0, 0xffff, data_ov000_02369dbc, data_ov000_02369ba0);
        StageObj_AllocSpritePalette(w, 0, 0x27);
        break;
    case 10:
        StageObj_Load2dBAC(w, data_ov000_023664d8, 0xffff, data_ov000_02369db4, data_ov000_02369ba0);
        StageObj_AllocSpritePalette(w, 0, 0x28);
        ObjRect_SetAtkDefFlags(&w->base.coll2, 0x13, 8);
        w->base.coll2.unkA |= 0x800;
        w->base.coll2.unk1C = EnemyPawnGroup_OnHit_Box2_PushPlayer;
        break;
    case 13:
        StageObj_Load2dBAC(w, data_ov000_023664f4, 0xffff, data_ov000_02369d9c, data_ov000_02369ba0);
        StageObj_AllocSpritePalette(w, 0, 0x30);
        w->base.unk1F4 |= 0x20;
        break;
    case 14:
        StageObj_Load2dBAC(w, data_ov000_02366510, 0xffff, data_ov000_02369d94, data_ov000_02369ba0);
        StageObj_AllocSpritePalette(w, 0, 0x30);
        w->base.unk1F4 |= 0x20;
        w->base.unk1E5 = 2;
        w->base.unk1E5 -= w->base.unk1E0->unkB;
        ObjRect_SetAtkDefFlags(&w->base.coll2, 0, -16);
        w->base.coll2.unk1C = EnemyPawnGroup_OnHit_Box2_Bounce;
        w->base.coll2.unkA |= 0x800;
        break;
    case 15:
        StageObj_Load2dBAC(w, data_ov000_02366528, 0xffff, data_ov000_02369d8c, data_ov000_02369ba0);
        StageObj_AllocSpritePalette(w, 0, 0x37);
        StageObj_SetMapHitbox(w, -8, -10, 8, 0x20);
        break;
    }
    StageObj_SetOAMOrder(w, 0x14);
    ObjRect_SetBox2D(&w->unk20C, -0x78, -0x30, -16, -16);
    w->unk20C.unkA |= 0xc0;
    ObjRect_SetAtkDefFlags(&w->unk20C, 0, -16);
    w->unk20C.unk28 = 0x12;
    w->unk20C.unk1C = EnemyPawnGroup_OnDetect;
    w->unk20C.unk18 = w;
    w->base.unk58 |= 0x40;
    if (w->base.unk1E0->unk4 & 0x10)
        w->base.unk5C &= ~1;
    if (w->base.unk1E0->unk6 == 0 && w->base.unk1E0->unk8 == 0) {
        w->base.unk1E0->unk6 = -0x28;
        w->base.unk1E0->unk8 = 0x50;
    }
    if (Stage_IsGravityFlipped(w->base.unk10, w->base.unk14)) {
        w->base.unk5 = 0x80;
        w->base.unk5C |= 0x100;
    }
    w->base.unkC = 200;
    if (w->base.unk1E0->unk9 != 0) {
        EnemyPawnGroup_Action_StartDropIn(w);
    } else if (p->unk4 & 0x40) {
        EnemyPawnGroup_Action_StartEntrance(w);
        w->base.unk1F4 |= 0x40;
    } else {
        w->base.unk58 |= 0x80;
        if (w->base.unk1E0->unk2 == 7 && (w->base.unk1E0->unk4 & 0x20))
            w->base.unk58 &= ~0x80;
        EnemyPawnGroup_Action_Walk(w);
    }
    w->base.unk1DC = Sound_AllocHandle();
    return w;
}
