#include <nitro/types.h>

void StaffRoll_AdvancePictureAnim(void *p);
void StaffRoll_SetPictureState(void *p, void (*f)(void));
void StaffRoll_PictureState_Blaze10(void);

#pragma thumb on
void StaffRoll_PictureState_Blaze9(u8 *p) {
    StaffRoll_AdvancePictureAnim(p);
    if (*(s32 *)(p + 0xbdc) > 0x78) {
        StaffRoll_SetPictureState(p, StaffRoll_PictureState_Blaze10);
    }
}
#pragma thumb reset
