#include <nitro/types.h>

extern u8 *data_ov001_0231f5bc;
void AnimatorSprite_Draw(void *p);
void AnimatorSprite_DrawRotScale(void *p, s32 a, s32 b, s32 c);

void func_ov001_023091a0(void) {
    u8 *g = data_ov001_0231f5bc;
    if (*(s32 *)(g + 0xbc4) >= 0x96 && *(s32 *)(g + 0xbc4) < 0x118) {
        AnimatorSprite_Draw(g + 0x780);
        return;
    }
    if (*(s32 *)(g + 0xbc4) < 0x118) {
        return;
    }
    if (*(s32 *)(g + 0xbc4) > 0x168) {
        return;
    }
    AnimatorSprite_DrawRotScale(g + 0x7c8, *(s32 *)(g + 0xbc0), *(s32 *)(g + 0xbc0), 0);
}
