#include <ov000_c/stageobj.h>

void *func_ov000_02307e84(void);
void *func_ov000_0234237c(s32 a, void *b);
BOOL func_ov000_02307dfc(void *a, s32 b);
BOOL Stage_IsGravityFlipped(s32 x, s32 y);
void GmkWall_Action_Unk_ov000_02308a6c(UnkStruct_StageObj *s);

void GmkWall_State_Unk_ov000_02308a7c(UnkStruct_StageObj *s) {
    if (func_ov000_02307dfc(func_ov000_0234237c(1, func_ov000_02307e84()), 0)) {
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
