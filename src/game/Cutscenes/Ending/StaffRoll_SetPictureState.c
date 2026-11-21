#include <nitro/types.h>

void StaffRoll_PictureState_None(void);

#pragma thumb on
void StaffRoll_SetPictureState(u8 *p, void (*f)(void)) {
    if (f == NULL) {
        *(void (**)(void))(p + 0xbd8) = StaffRoll_PictureState_None;
    } else {
        *(void (**)(void))(p + 0xbd8) = f;
    }
    *(s32 *)(p + 0xbdc) = 0;
}
#pragma thumb reset
