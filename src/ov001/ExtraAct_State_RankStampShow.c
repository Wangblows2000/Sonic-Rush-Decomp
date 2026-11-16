#include <nitro/types.h>

extern u8 *data_ov001_0231f084;
void AnimatorSprite_Draw(void *p);

void ExtraAct_State_RankStampShow(void) {
    u8 *q = data_ov001_0231f084 + 0xb10;
    *(u16 *)(q + 4) = 0x50;
    *(u16 *)(q + 6) = 0x8a;
    AnimatorSprite_Draw(q);
}
