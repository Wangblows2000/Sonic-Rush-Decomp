#include <nitro/types.h>

#pragma thumb on

void DemoScript_Create(s32 a);
void DemoScript_StartScript4(void) { DemoScript_Create(4); }

#pragma thumb reset
