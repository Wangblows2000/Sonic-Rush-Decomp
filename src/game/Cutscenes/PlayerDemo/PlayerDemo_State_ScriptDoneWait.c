#include <nitro/types.h>

#pragma thumb on

void PlayerDemo_State_Finish(void);
void PlayerDemo_State_ScriptDoneWait(void (**p)(void)) { *p = PlayerDemo_State_Finish; }

#pragma thumb reset
