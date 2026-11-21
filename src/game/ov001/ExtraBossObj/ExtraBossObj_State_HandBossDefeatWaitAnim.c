#include <nitro/types.h>

void Sound_PlayVoice(s32 a, s32 b, s32 c);
void ExtraBossObj_State_HandBossDefeatFadeOut(void);

void ExtraBossObj_State_HandBossDefeatWaitAnim(u8 *p) {
    u8 *q = *(u8 **)(p + 0x84);
    *(u32 *)(p + 0x14) ^= 0x30000;
    *(s32 *)(p + 0x1c8) -= 1;
    if (*(s32 *)(p + 0x1c8) == 0) {
        Sound_PlayVoice(0, 0, 0x2d);
    }
    if (q[0x10c] & 2) {
        *(void (**)(void))(p + 0x74) = ExtraBossObj_State_HandBossDefeatFadeOut;
        *(s32 *)(p + 0x1c8) = 0xfd;
    }
}
