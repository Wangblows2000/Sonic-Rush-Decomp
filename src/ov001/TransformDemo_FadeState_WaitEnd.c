#include <nitro/types.h>

void BrightnessFade_Create(s32 a, s32 b);
void TransformDemo_FadeState_FadeOut(void);

void TransformDemo_FadeState_WaitEnd(u8 *p, void (**out)(void)) {
    u32 f = *(u32 *)(p + 4);
    if (f & 0x40000000) {
        return;
    }
    if (!(f & 0x200) && *(s32 *)(p + 0xbd8) < 0x64000) {
        return;
    }
    *(u32 *)(p + 4) &= ~0x200;
    *(u32 *)(p + 4) |= 0x420;
    BrightnessFade_Create(5, 0x44);
    *out = TransformDemo_FadeState_FadeOut;
}
