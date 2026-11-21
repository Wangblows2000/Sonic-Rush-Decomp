#include <game/Stage/stageobj_alt.h>

typedef struct {
    s16 a;
    s16 b;
} UnkStruct_02367b0c;

extern void *gZoneActArchive;
extern UnkStruct_02367b0c data_ov000_02367b0c;
extern u8 data_ov000_02367e38[], data_ov000_02367e58[], data_ov000_0236a990[];

void EffEnemyRoomFix_Draw(void);
void EffEnemyRoomFix_State_Main(void);

void EffEnemyRoomFix_Create(s32 x, s16 y, s32 flip, s32 c) {
    UnkStruct_Task *task1;
    UnkStruct_Task *task2;
    UnkStruct_EffObj *w1;
    UnkStruct_EffObj *w2;
    UnkStruct_02367b0c off;
    vu16 zero1;
    vu16 zero2;

    off = data_ov000_02367b0c;
    task1 = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    task2 = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task1 == OS_GetArenaLo(0))
        return;
    if (task2 == OS_GetArenaLo(0))
        return;
    w1 = task1->work;
    w2 = task2->work;
    zero1 = 0;
    MIi_CpuClear16(zero1, w1, sizeof(UnkStruct_EffObj));
    zero2 = 0;
    MIi_CpuClear16(zero2, w2, sizeof(UnkStruct_EffObj));
    w1->unkC = c;
    w2->unkC = c;
    StageObj_Load2dBAC(w1, data_ov000_02367e38, 0x10, data_ov000_0236a990, gZoneActArchive);
    StageObj_Load2dBAC(w2, data_ov000_02367e58, 0x10, data_ov000_0236a990, gZoneActArchive);
    StageObj_AllocSpritePalette(w1, 0xb, 0x52);
    StageObj_AllocSpritePalette(w2, 0xc, 0x52);
    StageObj_SetOAMOrder(w1, 3);
    StageObj_SetOAMOrder(w2, 3);
    StageObj_SetOAMPriority(w1, 0);
    StageObj_SetOAMPriority(w2, 0);
    StageObj_SetAnimation(w1, 0xb);
    StageObj_SetAnimation(w2, 0xc);
    if (flip) {
        w1->unk5C |= 1;
        w2->unk5C |= 1;
        off.a = -off.a;
        off.b = -off.b;
    }
    w1->spr0.unk4 = x + off.a;
    w1->spr0.unk6 = y;
    w1->spr0.unk2C |= 4;
    w1->spr1.unk2C |= 0x80000;
    w2->spr0.unk4 = x + off.b;
    w2->spr0.unk6 = y;
    w2->spr0.unk2C |= 4;
    w2->spr1.unk2C |= 0x80000;
    w1->unk58 |= 0x2100;
    w2->unk58 |= 0x2100;
    w1->unk54 |= 2;
    w2->unk54 |= 2;
    w1->unk70 = EffEnemyRoomFix_Draw;
    w2->unk70 = EffEnemyRoomFix_Draw;
    w1->unk68 = EffEnemyRoomFix_State_Main;
    w2->unk68 = EffEnemyRoomFix_State_Main;
}
