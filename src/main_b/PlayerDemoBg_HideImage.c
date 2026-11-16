#include <nitro/types.h>

#pragma thumb on

void PlayerDemoBg_SetBg3Position(s32 a, s32 b);
void PlayerDemoBg_HideImage(void) { PlayerDemoBg_SetBg3Position(0, 0x30); }

#pragma thumb reset
