#include <nitro/types.h>

#pragma thumb on

void PlayerDemo_StartFade(void *a, s32 b);
void PlayerDemoOp_FadeToWhiteMain(void *a, void *b) { PlayerDemo_StartFade(b, 2); }

#pragma thumb reset
