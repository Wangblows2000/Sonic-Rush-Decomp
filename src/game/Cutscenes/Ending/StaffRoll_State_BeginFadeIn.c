#include <nitro/types.h>

void StaffRoll_SetState(void *p, void (*f)(void));
void StaffRoll_State_FadeInFromBlack(void);
void StaffRoll_State_FadeInFromWhite(void);

#pragma thumb on
void StaffRoll_State_BeginFadeIn(s32 *p) {
    if (*p == 2) {
        StaffRoll_SetState(p, StaffRoll_State_FadeInFromBlack);
    } else {
        StaffRoll_SetState(p, StaffRoll_State_FadeInFromWhite);
    }
}
#pragma thumb reset
