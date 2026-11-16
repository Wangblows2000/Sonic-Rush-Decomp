#include <nitro/types.h>

#pragma thumb on

void PlayerDemo_State_Finish(void);
void PlayerDemo_Unk_0205817c(void (**p)(void)) { *p = PlayerDemo_State_Finish; }

#pragma thumb reset
