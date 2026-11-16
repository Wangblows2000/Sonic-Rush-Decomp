#include <nitro/types.h>

s32 StaffRoll_CreateCreditTask(s32 a);
void StaffRoll_SetState(void *p, void (*f)(void));
void StaffRoll_State_WaitFinalCredit(void);

#pragma thumb on
void StaffRoll_State_StartFinalCredit(u8 *p) {
    *(s32 *)(p + 0xbd0) = StaffRoll_CreateCreditTask(2);
    StaffRoll_SetState(p, StaffRoll_State_WaitFinalCredit);
}
#pragma thumb reset
