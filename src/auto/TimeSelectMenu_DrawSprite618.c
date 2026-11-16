/* paramclone of VsMenu_Unk_ov001_022d9cb0 (src/ov001/VsMenu_Unk_ov001_022d9cb0.c); immediates and relocations remapped. */
#include <nitro/types.h>

void AnimatorSprite_ProcessAnimation(void *p, s32 a, s32 b);
void AnimatorSprite_Draw(void *p);

#pragma thumb on
void TimeSelectMenu_DrawSprite618(u8 *p, u16 a, u16 b) {
    u8 *q = p + 0x618;
    *(u16 *)(q + 4) = a;
    *(u16 *)(q + 6) = b;
    AnimatorSprite_ProcessAnimation(q, 0, 0);
    AnimatorSprite_Draw(q);
}
#pragma thumb reset
