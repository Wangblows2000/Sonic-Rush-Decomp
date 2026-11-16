#include <nitro/types.h>

void StaffRoll_SetPictureState(void *p, void (*f)(void));
void func_ov001_022d0d78(void);

#pragma thumb on
void func_ov001_022d0e10(u8 *p) {
    if (*(s32 *)(p + 0xcbc) >= 0x11d000) {
        StaffRoll_SetPictureState(p, func_ov001_022d0d78);
    }
}
#pragma thumb reset
