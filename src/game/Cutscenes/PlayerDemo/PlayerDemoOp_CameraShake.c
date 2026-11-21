#include <nitro/types.h>

#pragma thumb on

void PlayerDemo_StartCameraShake(void *a, s32 b);
void PlayerDemoOp_CameraShake(void *a, void *b) { PlayerDemo_StartCameraShake(b, 0); }

#pragma thumb reset
