#include <nitro/types.h>

#pragma thumb on

void PlayerDemo_Create(void);
void PlayerDemo_Start(void) { PlayerDemo_Create(); }

#pragma thumb reset
