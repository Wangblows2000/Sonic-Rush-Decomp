#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[8];
    u32 unk8;
} UnkStruct_022c4680;

extern UnkStruct_022c4560 gGameState;
extern UnkStruct_022c4680 gSaveData;

void EnemyFlapperGroup_State_Unk_ov000_022fff44(void);

void EnemyFlapperGroup_Action_Unk_ov000_023000dc(UnkStruct_StageObj *w) {
    u16 *f;

    if (w->unk1E0->unk2 != 0x12)
        StageObj_SetAnimation(w, 2);
    else
        StageObj_SetAnimation(w, 0);
    w->unk5C |= 4;
    w->unk68 = EnemyFlapperGroup_State_Unk_ov000_022fff44;
    f = &w->unk5C;
    w->unk1F4 &= ~1;
    w->unk34 = 0;
    w->unk2E = 0;
    w->unk30 = 0;
    w->unk8 = 0;
    w->unk58 |= 0x2000;
    if (w->unk204 != NULL) {
        if (w->unk10 > ((UnkStruct_StageObj *)w->unk204)->unk10)
            *f |= 1;
        else
            *f &= ~1;
        w->unk8 = (((UnkStruct_StageObj *)w->unk204)->unk10 >> 8) | ((((UnkStruct_StageObj *)w->unk204)->unk14 >> 8) << 16);
        w->unk204 = NULL;
    }
    w->unk1FC = w->unk10;
    w->unk200 = w->unk14;
    if (w->unk1E0->unk2 != 0x12) {
        w->unk30 = 0x10;
        w->unk34 = 0x18;
    } else {
        w->unk30 = 0x20;
        w->unk34 = 8;
    }
    if (!(gSaveData.unk8 & 1))
        return;
    if (gGameState.unk10 == 2)
        return;
    if (gGameState.unk10 == 3)
        return;
    w->unk30 += w->unk30 >> 1;
    w->unk34 += w->unk34 >> 1;
}
