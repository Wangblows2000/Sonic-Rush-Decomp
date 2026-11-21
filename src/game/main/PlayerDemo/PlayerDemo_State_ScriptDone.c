#include <nitro/types.h>

#pragma thumb on

void PlayerDemo_State_ScriptDoneWait(void (**p)(void));
void PlayerDemo_State_ScriptDone(void (**p)(void)) { *p = (void (*)(void))PlayerDemo_State_ScriptDoneWait; }

#pragma thumb reset
