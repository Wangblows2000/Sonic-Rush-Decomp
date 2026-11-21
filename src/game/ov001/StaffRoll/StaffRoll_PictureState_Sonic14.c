#include <nitro/types.h>

void StaffRoll_SetPictureState(void *p, void (*f)(void));
void StaffRoll_PictureState_Sonic15(void);

#pragma thumb on
void StaffRoll_PictureState_Sonic14(u8 *p) {
    if (*(s32 *)(p + 0xcbc) >= 0x11d000) {
        StaffRoll_SetPictureState(p, StaffRoll_PictureState_Sonic15);
    }
}
#pragma thumb reset
