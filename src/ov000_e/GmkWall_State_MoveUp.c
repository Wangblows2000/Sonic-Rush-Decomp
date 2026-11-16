#include <ov000_c/stageobj.h>

void *Gimmick_GetRoomIndex(void);
void *EnemyRoom_Get(s32 a, void *b);
BOOL Gimmick_Unk_ov000_02307dfc(void *a, s32 b);
BOOL Stage_IsGravityFlipped(s32 x, s32 y);
void GmkWall_Action_Unk_ov000_02308a6c(UnkStruct_StageObj *s);

void GmkWall_State_MoveUp(UnkStruct_StageObj *s) {
    if (Gimmick_Unk_ov000_02307dfc(EnemyRoom_Get(1, Gimmick_GetRoomIndex()), 0)) {
        s->unk1F4 &= ~0x10;
        s->unk54 |= 4;
        return;
    }
    s->unkC--;
    if (!Stage_IsGravityFlipped(s->unk10, s->unk14))
        s->unk14 = s->unk8 + (s->unkC << 14) / 60;
    else
        s->unk14 = s->unk8 - (s->unkC << 14) / 60;
    if (s->unkC == 0)
        GmkWall_Action_Unk_ov000_02308a6c(s);
}
