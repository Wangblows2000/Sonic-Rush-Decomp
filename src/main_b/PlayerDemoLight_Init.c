#include <nitro/types.h>

#pragma thumb on

void PlayerDemoLight_Apply(void *a);
void PlayerDemoLight_Action_Idle(void *a, void *b);
void PlayerDemoLight_Init(void *a, void *b) { PlayerDemoLight_Apply(a); PlayerDemoLight_Action_Idle(a, b); }

#pragma thumb reset
