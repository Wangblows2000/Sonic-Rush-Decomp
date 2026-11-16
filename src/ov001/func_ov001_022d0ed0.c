#include <nitro/types.h>

void StaffRoll_SetPictureState(void *p, void (*f)(void));
void func_ov001_022d0e38(void);

#pragma thumb on
void func_ov001_022d0ed0(u8 *p) {
    if (*(s32 *)(p + 0xbdc) > 0x28) {
        StaffRoll_SetPictureState(p, func_ov001_022d0e38);
    }
}
#pragma thumb reset
