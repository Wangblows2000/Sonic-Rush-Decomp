#include <nitro/types.h>

void StaffRoll_SetPictureState(void *p, void (*f)(void));

#pragma thumb on
void func_ov001_022d0d44(u8 *p) {
    if (*(s32 *)(p + 0xcc0) >= 0x131000) {
        *(s32 *)(p + 0xcd8) = 0;
        *(s32 *)(p + 0xcf0) = 0;
        StaffRoll_SetPictureState(p, NULL);
    }
}
#pragma thumb reset
