#include <nitro/types.h>

void Boss3_UpdateCameraShake(void *p, s32 a, s32 b);

void Boss3PartCC_State_Idle_ov000_022d8bb8(u8 *p) {
    Boss3_UpdateCameraShake(p + 0x264, 1, 5);
}
