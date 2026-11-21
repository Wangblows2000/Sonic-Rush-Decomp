#include <nitro/types.h>

#pragma thumb on

void PlayerDemoBg_HideImage(void);
void PlayerDemoBg_State_WaitNext(void);
void PlayerDemoBg_State_Hide(void (**p)(void)) { PlayerDemoBg_HideImage(); *p = PlayerDemoBg_State_WaitNext; }

#pragma thumb reset
