#include <nitro/types.h>

#pragma thumb on

void TalkDemoScene_State_RockPageMotion(void);
void TalkDemoScene_State_RockPage(void (**p)(void)) { *p = TalkDemoScene_State_RockPageMotion; }

#pragma thumb reset
