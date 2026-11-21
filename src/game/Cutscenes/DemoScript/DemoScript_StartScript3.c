#include <nitro/types.h>

#pragma thumb on

void DemoScript_Create(s32 a);
void DemoScript_StartScript3(void) { DemoScript_Create(3); }

#pragma thumb reset
