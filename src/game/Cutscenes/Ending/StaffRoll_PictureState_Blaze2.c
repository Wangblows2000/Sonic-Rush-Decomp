#include <nitro/types.h>

void StaffRoll_SetPictureState(void *p, void (*f)(void));
void StaffRoll_PictureState_Blaze3(void);

#pragma thumb on
void StaffRoll_PictureState_Blaze2(u8 *p) {
    if (*(s32 *)(p + 0xbdc) > 0xb4) {
        StaffRoll_SetPictureState(p, StaffRoll_PictureState_Blaze3);
    }
}
#pragma thumb reset
