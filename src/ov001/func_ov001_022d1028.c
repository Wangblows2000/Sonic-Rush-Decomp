#include <nitro/types.h>

void StaffRoll_SetPictureState(void *p, void (*f)(void));
void func_ov001_022d0f98(void);

#pragma thumb on
void func_ov001_022d1028(u8 *p) {
    if (*(s32 *)(p + 0xbdc) > 0x3c) {
        StaffRoll_SetPictureState(p, func_ov001_022d0f98);
    }
}
#pragma thumb reset
