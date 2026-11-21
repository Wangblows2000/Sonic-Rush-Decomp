#include <nitro/types.h>

#pragma thumb on

void Touch_Disable(void);
void PlayerDemo_DisableTouch(void) { Touch_Disable(); }

#pragma thumb reset
