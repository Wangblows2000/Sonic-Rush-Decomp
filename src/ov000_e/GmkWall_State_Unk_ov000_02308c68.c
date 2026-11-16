#include <ov000_c/stageobj.h>

void *Gimmick_Unk_ov000_02307e84(void);
void *EnemyRoom_Get(s32 a, void *b);
BOOL Gimmick_Unk_ov000_02307dfc(void *a, s32 b);
BOOL Stage_IsGravityFlipped(s32 x, s32 y);
void GmkWall_Action_Unk_ov000_02308c20(UnkStruct_StageObj *s);

void GmkWall_State_Unk_ov000_02308c68(UnkStruct_StageObj *s) {
    if (Gimmick_Unk_ov000_02307dfc(EnemyRoom_Get(1, Gimmick_Unk_ov000_02307e84()), 0)) {
        s->unk1F4 &= ~0x10;
        s->unk54 |= 4;
        return;
    }
    s->unkC--;
    if (!Stage_IsGravityFlipped(s->unk10, s->unk14)) {
        s->unk14 = s->unk8 + ((0x28 - s->unkC) << 14) / 40;
        s->unk1D6 = (s8)((s->unkC << 6) / 40 - 0x20);
    } else {
        s->unk14 = s->unk8 - ((0x28 - s->unkC) << 14) / 40;
        s->unk1D6 = (s8)(-0x20 - (s->unkC << 6) / 40);
    }
    if (s->unkC == 0)
        GmkWall_Action_Unk_ov000_02308c20(s);
}
