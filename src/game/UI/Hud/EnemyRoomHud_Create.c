#include <game/Stage/stageobj_alt.h>

typedef struct {
    UnkStruct_EffObj base;
    UnkStruct_Spr s0;
    UnkStruct_Spr s1;
    UnkStruct_Spr s2;
    UnkStruct_Spr s3;
    u8 pad254[0x268 - 0x254];
    s32 unk268;
    s32 unk26C;
    u32 unk270;
    u8 pad274[4];
} UnkStruct_0232d620;

typedef struct {
    u8 pad0[0x20C];
    u32 unk20C;
    u8 pad210[2];
    s8 unk212;
    s8 unk213;
} UnkStruct_0232d620_Ply;

extern void *gZoneActArchive;
extern UnkStruct_Task *gEnemyRoomHudTask;
extern u8 data_ov000_02368058[], data_ov000_0236a990[], data_ov000_02368078[], data_ov000_02368098[];

void Task_Destroy(UnkStruct_Task *t);
void EnemyRoomHud_Destructor(void);
void EnemyRoomHud_Draw(void);
UnkStruct_0232d620_Ply *EnemyRoom_GetActive(s32 a);
void Hud_ToDecimalDigits(u32 *out, s32 a, s32 b);
void EnemyRoomHud_Action_Appear(UnkStruct_0232d620 *w);

void EnemyRoomHud_Create(void) {
    UnkStruct_Task *task;
    UnkStruct_0232d620 *w;
    UnkStruct_0232d620_Ply *p;
    vu16 zero;

    if (gEnemyRoomHudTask != NULL)
        Task_Destroy(gEnemyRoomHudTask);
    task = Task_Create(StageObj_Main, EnemyRoomHud_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_0232d620));
    if (task == OS_GetArenaLo(0))
        return;
    gEnemyRoomHudTask = task;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0232d620));
    StageObj_Load2dBAC(w, data_ov000_02368058, 0x20, data_ov000_0236a990, gZoneActArchive);
    StageObj_AllocSpritePalette(w, 0, 0xa);
    StageObj_SetOAMOrder(w, 3);
    StageObj_SetOAMPriority(w, 0);
    ObjAction_Load2dBAC(&w->s0, &w->s1, data_ov000_02368078, 0, data_ov000_0236a990, gZoneActArchive);
    SpritePair_SetOAMOrder(&w->s0, &w->s1, 3);
    SpritePair_SetOAMPriority(&w->s0, &w->s1, 0);
    w->s0.unk44 = w->base.spr0.unk44;
    w->s1.unk44 = w->base.spr0.unk44;
    w->s0.unk34 = w->base.spr0.unk34 + 0x280;
    w->s1.unk34 = w->base.spr1.unk34 + 0x280;
    w->s0.unk2C |= 0x200;
    w->s1.unk2C |= 0x200;
    ObjAction_Load2dBAC(&w->s2, &w->s3, data_ov000_02368098, 0, data_ov000_0236a990, gZoneActArchive);
    SpritePair_SetOAMOrder(&w->s2, &w->s3, 3);
    SpritePair_SetOAMPriority(&w->s2, &w->s3, 0);
    w->s2.unk44 = w->base.spr0.unk44;
    w->s3.unk44 = w->base.spr0.unk44;
    w->s2.unk34 = w->base.spr0.unk34 + 0x380;
    w->s3.unk34 = w->base.spr1.unk34 + 0x380;
    w->s2.unk2C |= 0x200;
    w->s3.unk2C |= 0x200;
    w->base.unk70 = EnemyRoomHud_Draw;
    w->base.unk10 = 0x8000;
    w->base.unk14 = 0x2800;
    w->base.unk5C |= 1;
    w->base.unk5C |= 0x80;
    w->base.unk58 |= 0x100;
    w->base.unk54 |= 0x12;
    p = EnemyRoom_GetActive(1);
    if (p != NULL) {
        w->unk26C = w->unk268 = p->unk212 - p->unk213;
        if (p->unk20C & 1)
            w->base.unk54 |= 0x4000;
    } else {
        w->unk26C = w->unk268 = 0;
    }
    Hud_ToDecimalDigits(&w->unk270, w->unk268, 100);
    AnimatorSprite_SetAnimation(&w->s0, (u16)(((w->unk270 & 0xf0) >> 4) + 1));
    AnimatorSprite_SetAnimation(&w->s1, (u16)(((w->unk270 & 0xf0) >> 4) + 1));
    AnimatorSprite_SetAnimation(&w->s2, (u16)((w->unk270 & 0xf) + 1));
    AnimatorSprite_SetAnimation(&w->s3, (u16)((w->unk270 & 0xf) + 1));
    EnemyRoomHud_Action_Appear(w);
}
