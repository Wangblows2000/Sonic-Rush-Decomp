#include <nitro/types.h>

void TimeAttackMenu_Create(s32 a);

#pragma thumb on
void TimeAttackMenu_StartMode0(void) {
    TimeAttackMenu_Create(0);
}
#pragma thumb reset
