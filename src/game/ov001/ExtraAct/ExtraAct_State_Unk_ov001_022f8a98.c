#include <nitro/types.h>

extern u8 *data_ov001_0231f084;
void AnimatorSprite_Draw(void *p);

void ExtraAct_State_Unk_ov001_022f8a98(void) {
    u8 *o = data_ov001_0231f084 + 0xac8;
    *(s16 *)(o + 4) = 0;
    *(s16 *)(o + 6) = 0;
    *(u16 *)(o + 0x44) = 0;
    *(u32 *)(o + 0x2c) &= ~0x100;
    AnimatorSprite_Draw(o);
    *(s16 *)(o + 6) = 0xc0;
    *(u16 *)(o + 0x44) = 1;
    *(u32 *)(o + 0x2c) |= 0x100;
    AnimatorSprite_Draw(o);
}
