#include <nitro/types.h>

extern u8 *data_ov001_0231f5bc;
u8 *func_ov001_0230c0a4(void);
void AnimatorSprite_Draw(void *p);

void func_ov001_02308720(void) {
    u8 *g = data_ov001_0231f5bc;
    s32 v = *(s32 *)(func_ov001_0230c0a4() + 0x148);
    if (v >= 100) {
        AnimatorSprite_Draw(g + 0x198);
        AnimatorSprite_Draw(g + 0x1e0);
        AnimatorSprite_Draw(g + 0x228);
    } else if (v >= 10) {
        AnimatorSprite_Draw(g + 0x1e0);
        AnimatorSprite_Draw(g + 0x228);
    } else {
        AnimatorSprite_Draw(g + 0x228);
    }
}
