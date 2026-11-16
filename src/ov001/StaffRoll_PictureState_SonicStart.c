#include <nitro/types.h>

void StaffRoll_SetPictureState(void *p, void (*f)(void));
void func_ov001_022d1350(void);

#pragma thumb on
void StaffRoll_PictureState_SonicStart(void *p) {
    StaffRoll_SetPictureState(p, func_ov001_022d1350);
}
#pragma thumb reset
