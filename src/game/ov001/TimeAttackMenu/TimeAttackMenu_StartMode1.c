#include <nitro/types.h>

void TimeAttackMenu_Create(s32 a);

#pragma thumb on
void TimeAttackMenu_StartMode1(void) {
    TimeAttackMenu_Create(1);
}
#pragma thumb reset
