#include <nitro/types.h>

void StaffRoll_Unk_ov001_022d0220(void *p);
void StaffRoll_SetPictureState(void *p, void (*f)(void));
void StaffRoll_PictureState_Blaze5(void);

#pragma thumb on
void StaffRoll_PictureState_Blaze4(u8 *p) {
    StaffRoll_Unk_ov001_022d0220(p);
    if (*(u32 *)(p + 0xc58) & 2) {
        StaffRoll_SetPictureState(p, StaffRoll_PictureState_Blaze5);
    }
}
#pragma thumb reset
