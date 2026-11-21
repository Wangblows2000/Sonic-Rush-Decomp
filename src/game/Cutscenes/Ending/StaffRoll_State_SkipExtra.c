#include <nitro/types.h>

extern u16 data_0208926c[];
extern u16 data_02087d44[];
void StaffRoll_SetState(void *p, void (*f)(void));
void StaffRoll_State_ExtraShowFinalPicture(void);

#pragma thumb on
void StaffRoll_State_SkipExtra(void *p) {
    data_0208926c[4] = 0;
    data_02087d44[4] = 0;
    StaffRoll_SetState(p, StaffRoll_State_ExtraShowFinalPicture);
}
#pragma thumb reset
