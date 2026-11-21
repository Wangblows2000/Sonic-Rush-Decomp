#include <nitro/types.h>

void StaffRoll_SetState(void *p, void (*f)(void));
void StaffRoll_State_RevealBanner(void);

#pragma thumb on
void StaffRoll_State_WaitFinalCredit(u8 *p) {
    if (*(s32 *)(p + 0x14) > 0x1d0) {
        StaffRoll_SetState(p, StaffRoll_State_RevealBanner);
    }
}
#pragma thumb reset
