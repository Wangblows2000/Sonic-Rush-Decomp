#include <nitro/types.h>

s32 SpecialStageMap_GetScrollSpeed(void);
s32 func_0200225c(s32 a, s32 b);
s32 func_ov001_02311098(s32 a);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void SpecialStagePlayer_ChangeState(void *p, s32 v);

void func_ov001_023110bc(u8 *p) {
    s32 v = SpecialStageMap_GetScrollSpeed();
    func_0200225c(v, 0x3000);
    *(s32 *)(*(u8 **)(p + 0xac) + 0x114) = func_ov001_02311098(v);
    if (v >= 0xc00) {
        return;
    }
    (*(u8 **)(p + 0xac))[0x10c] &= ~4;
    Sound_PlaySeqArc(0, 0x10, 0x10);
    SpecialStagePlayer_ChangeState(p, 0x17);
}
