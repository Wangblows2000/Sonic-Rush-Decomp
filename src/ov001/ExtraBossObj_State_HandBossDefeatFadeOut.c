#include <nitro/types.h>

void BrightnessLerp_Create(s32 a, s32 b, s32 c, s32 d, void *e);
void Sound_PlayVoice(s32 a, s32 b, s32 c);
void ExtraBossObj_State_HandBossDefeatFinish(void);

void ExtraBossObj_State_HandBossDefeatFadeOut(u8 *p) {
    *(u32 *)(p + 0x14) ^= 0x30000;
    *(s32 *)(p + 0x1c8) -= 1;
    if (*(s32 *)(p + 0x1c8) <= 0) {
        *(u16 *)(p + 0x1c8) = 0;
        BrightnessLerp_Create(0x20, 0, 0x10, 3, p + 0x1c8);
        *(void (**)(void))(p + 0x74) = ExtraBossObj_State_HandBossDefeatFinish;
        return;
    }
    if (*(s32 *)(p + 0x1c8) == 0xc1) {
        Sound_PlayVoice(0, 0, 0x24);
    }
}
