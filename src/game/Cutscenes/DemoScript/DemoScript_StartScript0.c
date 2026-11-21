#include <nitro/types.h>

#pragma thumb on

void DemoScript_Create(s32 a);
void DemoScript_StartScript0(void) { DemoScript_Create(0); }

#pragma thumb reset
