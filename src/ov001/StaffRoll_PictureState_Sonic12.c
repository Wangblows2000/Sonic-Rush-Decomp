#include <nitro/types.h>

void StaffRoll_SetPictureState(void *p, void (*f)(void));
void StaffRoll_PictureState_Sonic13(void);

#pragma thumb on
void StaffRoll_PictureState_Sonic12(u8 *p) {
    if (*(s32 *)(p + 0xbdc) > 0x28) {
        StaffRoll_SetPictureState(p, StaffRoll_PictureState_Sonic13);
    }
}
#pragma thumb reset
