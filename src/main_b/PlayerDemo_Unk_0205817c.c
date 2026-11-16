#include <nitro/types.h>

#pragma thumb on

void PlayerDemo_Unk_02058168(void);
void PlayerDemo_Unk_0205817c(void (**p)(void)) { *p = PlayerDemo_Unk_02058168; }

#pragma thumb reset
