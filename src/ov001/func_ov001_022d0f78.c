#include <nitro/types.h>

void StaffRoll_SetPictureState(void *p, void (*f)(void));
void func_ov001_022d0ef0(void);

#pragma thumb on
void func_ov001_022d0f78(u8 *p) {
    if (*(s32 *)(p + 0xbdc) > 0x28) {
        StaffRoll_SetPictureState(p, func_ov001_022d0ef0);
    }
}
#pragma thumb reset
