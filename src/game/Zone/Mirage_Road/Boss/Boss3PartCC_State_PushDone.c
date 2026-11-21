#include <nitro/types.h>

void Boss3_UpdateCameraShake(void *p, s32 a, s32 b);

void Boss3PartCC_State_PushDone(u8 *p) {
    Boss3_UpdateCameraShake(p + 0x264, 1, 5);
}
