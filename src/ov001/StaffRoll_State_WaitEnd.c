#include <nitro/types.h>

void StaffRoll_SetState(void *p, void (*f)(void));
void StaffRoll_State_End(void);

#pragma thumb on
void StaffRoll_State_WaitEnd(u8 *p) {
    if (*(s32 *)(p + 0x14) >= 0x5a) {
        StaffRoll_SetState(p, StaffRoll_State_End);
    }
}
#pragma thumb reset
