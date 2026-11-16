#include <nitro/types.h>

void SpecialStagePlayer_ChangeState(void *p, s32 v);

void func_ov001_02310fe8(u8 *p) {
    if (*(s32 *)(p + 0x15c) < 2) {
        u16 old = *(u16 *)(p + 0x12);
        *(u16 *)(p + 0x12) = old + 0x6c0;
        if (old < 0x8000 && *(u16 *)(p + 0x12) >= 0x8000) {
            *(s32 *)(p + 0x15c) += 1;
            if (*(s32 *)(p + 0x15c) >= 2) {
                *(u16 *)(p + 0x12) = 0x8000;
            }
        }
    }
    if ((*(u8 **)(p + 0xac))[0x10c] & 2) {
        SpecialStagePlayer_ChangeState(p, 2);
    }
}
