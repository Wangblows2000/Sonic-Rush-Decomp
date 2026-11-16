#include <nitro/types.h>

void StaffRoll_SetPictureState(void *p, void (*f)(void));
void func_ov001_022d0c84(void);

#pragma thumb on
void StaffRoll_PictureState_BlazeStart(void *p) {
    StaffRoll_SetPictureState(p, func_ov001_022d0c84);
}
#pragma thumb reset
