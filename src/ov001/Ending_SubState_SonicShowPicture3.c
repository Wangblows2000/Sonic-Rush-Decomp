#include <nitro/types.h>

void Ending_FadeIn(void *p, s32 a, s32 b, s32 c, s32 d);
BOOL Ending_FadeOut(void *p, s32 a, s32 b, s32 c, s32 d);
void Ending_SetSubState(void *p, void (*f)(void));
void Ending_SubState_SonicNextPicture4(void);
void Ending_UpdatePictureZoomShake(void *p);

void Ending_SubState_SonicShowPicture3(u8 *p) {
    if (*(s32 *)(p + 0xc) >= 5) {
        Ending_FadeIn(p, *(s32 *)(p + 0xc) - 5, 0x1e, 1, 0);
    }
    if (*(s32 *)(p + 0xc) >= 0x14f && Ending_FadeOut(p, *(s32 *)(p + 0xc) - 0x14f, 0x3c, 1, 0)) {
        Ending_SetSubState(p, Ending_SubState_SonicNextPicture4);
        return;
    }
    Ending_UpdatePictureZoomShake(p);
}
