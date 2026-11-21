#include <nitro/types.h>

void Vram_SetBankForBGExtPltt(s32 a);

void Camera_InitFixedVram(void) {
    Vram_SetBankForBGExtPltt(0);
}
