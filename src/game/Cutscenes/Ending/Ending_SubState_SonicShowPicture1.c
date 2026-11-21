#include <nitro/types.h>

void Ending_FadeIn(void *p, s32 a, s32 b, s32 c, s32 d);
BOOL Ending_FadeOut(void *p, s32 a, s32 b, s32 c, s32 d);
void Ending_SetSubState(void *p, void (*f)(void));
void Ending_SubState_SonicNextPicture2(void);

void Ending_SubState_SonicShowPicture1(u8 *p) {
    if (*(s32 *)(p + 0xc) >= 5) {
        Ending_FadeIn(p, *(s32 *)(p + 0xc) - 5, 0x3c, 1, 0);
    }
    if (*(s32 *)(p + 0xc) < 0x1a9) {
        return;
    }
    if (!Ending_FadeOut(p, *(s32 *)(p + 0xc) - 0x1a9, 0x3c, 1, 0)) {
        return;
    }
    Ending_SetSubState(p, Ending_SubState_SonicNextPicture2);
}
