#include <nitro/types.h>

extern s16 data_0208926c[];
extern s16 data_02087d44[];
void StaffRoll_SetState(void *p, void (*f)(void));
void StaffRoll_State_BeginFadeIn(void);

#pragma thumb on
void StaffRoll_State_Start(s32 *p) {
    if (*p == 2) {
        data_0208926c[4] = -0x10;
        data_02087d44[4] = -0x10;
    } else {
        data_0208926c[4] = 0x10;
        data_02087d44[4] = 0x10;
    }
    StaffRoll_SetState(p, StaffRoll_State_BeginFadeIn);
}
#pragma thumb reset
