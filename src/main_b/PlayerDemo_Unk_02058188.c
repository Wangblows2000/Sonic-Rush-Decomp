#include <nitro/types.h>

#pragma thumb on

void PlayerDemo_Unk_0205817c(void (**p)(void));
void PlayerDemo_Unk_02058188(void (**p)(void)) { *p = (void (*)(void))PlayerDemo_Unk_0205817c; }

#pragma thumb reset
