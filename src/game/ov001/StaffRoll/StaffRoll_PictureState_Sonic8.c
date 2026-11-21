#include <nitro/types.h>

void StaffRoll_SetPictureState(void *p, void (*f)(void));
void StaffRoll_PictureState_Sonic9(void);

#pragma thumb on
void StaffRoll_PictureState_Sonic8(u8 *p) {
    if (*(s32 *)(p + 0xbdc) > 0x3c) {
        StaffRoll_SetPictureState(p, StaffRoll_PictureState_Sonic9);
    }
}
#pragma thumb reset
