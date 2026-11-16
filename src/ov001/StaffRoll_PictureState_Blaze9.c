#include <nitro/types.h>

void func_ov001_022d0220(void *p);
void StaffRoll_SetPictureState(void *p, void (*f)(void));
void StaffRoll_PictureState_Blaze10(void);

#pragma thumb on
void StaffRoll_PictureState_Blaze9(u8 *p) {
    func_ov001_022d0220(p);
    if (*(s32 *)(p + 0xbdc) > 0x78) {
        StaffRoll_SetPictureState(p, StaffRoll_PictureState_Blaze10);
    }
}
#pragma thumb reset
