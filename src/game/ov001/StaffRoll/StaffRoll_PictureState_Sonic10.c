#include <nitro/types.h>

void StaffRoll_SetPictureState(void *p, void (*f)(void));
void StaffRoll_PictureState_Sonic11(void);

#pragma thumb on
void StaffRoll_PictureState_Sonic10(u8 *p) {
    if (*(s32 *)(p + 0xbdc) > 0x28) {
        StaffRoll_SetPictureState(p, StaffRoll_PictureState_Sonic11);
    }
}
#pragma thumb reset
