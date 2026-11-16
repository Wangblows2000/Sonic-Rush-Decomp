#include <nitro/types.h>

#pragma thumb on

void PlayerDemoBg_Unk_02056b74(void);
void PlayerDemoBg_Unk_02056c9c(void);
void PlayerDemoBg_Unk_02056ce0(void (**p)(void)) { PlayerDemoBg_Unk_02056b74(); *p = PlayerDemoBg_Unk_02056c9c; }

#pragma thumb reset
