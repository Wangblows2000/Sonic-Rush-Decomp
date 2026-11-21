#include <nitro/types.h>

#pragma thumb on

void PlayerDemo_DisableTouch(void);
void PlayerDemo_ReleaseSkippable(void) { PlayerDemo_DisableTouch(); }

#pragma thumb reset
