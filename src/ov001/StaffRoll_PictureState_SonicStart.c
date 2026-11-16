#include <nitro/types.h>

void StaffRoll_SetPictureState(void *p, void (*f)(void));
void StaffRoll_PictureState_Sonic1(void);

#pragma thumb on
void StaffRoll_PictureState_SonicStart(void *p) {
    StaffRoll_SetPictureState(p, StaffRoll_PictureState_Sonic1);
}
#pragma thumb reset
