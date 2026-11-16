#include <ov000_c/stageobj.h>

void *func_ov000_02307e84(void);
void *func_ov000_0234237c(s32 a, void *b);
BOOL func_ov000_02307dfc(void *a, s32 b);
BOOL Stage_IsGravityFlipped(s32 x, s32 y);
void func_ov000_02308c20(UnkStruct_StageObj *s);

void GmkWall_State_Unk_ov000_02308c68(UnkStruct_StageObj *s) {
    if (func_ov000_02307dfc(func_ov000_0234237c(1, func_ov000_02307e84()), 0)) {
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
        func_ov000_02308c20(s);
}
