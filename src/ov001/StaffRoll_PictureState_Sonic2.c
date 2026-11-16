#include <nitro/types.h>

void StaffRoll_SetPictureState(void *p, void (*f)(void));
void StaffRoll_PictureState_Sonic3(void);

#pragma thumb on
void StaffRoll_PictureState_Sonic2(u8 *p) {
    if (*(s32 *)(p + 0xbe0) == 1) {
        StaffRoll_SetPictureState(p, StaffRoll_PictureState_Sonic3);
    }
}
#pragma thumb reset
